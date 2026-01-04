#include "Container.h"
#include <cassert>

#include "../widgets/databinding/DataBinding.h"
#include "../widgets/Label.h"
#include "../UIManager.h"

#include "../widgets/VerticalBox.h"

#include "../utils/utils.h"

RUIN::UIContainer::UIContainer(tinyxml2::XMLElement* element)
{
	if (!element)
	{
		return; 
	}

	Bind_method_to_XML(InitializeHorizontalFillmode, element, "horizontal-fillMode");
	Bind_method_to_XML(InitializeVerticalFillmode, element, "vertical-fillMode");
	Bind_method_to_XML(InitializeVerticalOverflow, element, "vertical-overflow");
	Bind_method_to_XML(InitializeBackgroundColour, element, "background-colour");
	Bind_member_to_XML(m_DataSource, element, "datasource").OnChange(DataSourceChanged);

	for (auto* e = element->FirstChildElement(); e; e = e->NextSiblingElement())
	{
		const char* name = e->Name();

		if (strcmp(name, "ItemTemplate") == 0)
		{
			RASSERT(m_Renderables.size() == 0, "Can't mix ItemTemplate with normal widgets!");

			tinyxml2::XMLPrinter printer;
			e->Accept(&printer);
			m_ItemTemplate = printer.CStr();

			return;
		}

		AddChildWidget(e);
	}
}

void RUIN::UIContainer::Render(const RenderArea& renderArea)
{
	// TODO: This is duplicated in UIContainer and LeafNode
	UIManager::GetInstance().DrawRectangle(renderArea, m_BackgroundColour);

	// TODO: fix up how/when we apply cliprects
	if (m_ScrollVertical)
	{
		UIManager::GetInstance().SetClipRect(renderArea);
	}

	int count = 0;
	for (const auto& renderable : m_Renderables)
	{
		const auto& ra = m_RenderAreaPerRenderable[count++];

		//if (ra.IsEmpty())
		//	continue;

		renderable->Render(ra);
	}

	if (m_ScrollVertical)
	{
		UIManager::GetInstance().SetClipRect(std::nullopt);
	}
}

RUIN::RenderArea RUIN::UIContainer::CalculateUsedContentArea(const RenderArea& absoluteAvailableArea)
{
	RenderContext ctx{};

	RenderArea availableArea = absoluteAvailableArea;

	if (m_ScrollVertical)
	{
		availableArea.h = INFINITY;
	}

	// Render areas should be absolute, not relative. 
	// This means we need to "fill in" the start position for this area
	RenderArea usedArea{};
	usedArea.w = availableArea.x;
	usedArea.h = availableArea.y;

	m_Overflowing = false;

	// Render areas are not exposed to leaf nodes, they're an implementation detail of containers.
	m_RenderAreaPerRenderable.clear();
	m_RenderAreaPerRenderable.reserve(m_Renderables.size());
	for (auto& renderable : m_Renderables)
	{
		// The canvas the child element is allowed to use (container behaviour)
		const auto available = GetAreaForChild(availableArea, usedArea, ctx);
		++ctx.childIndex;

		//if (available.IsEmpty())
		//{
		//	//continue;
		//}

		// Traverse widget tree, let widgets take up all or part of the available space
		const auto& rendered = m_RenderAreaPerRenderable.emplace_back(renderable->CalculateUsedContentArea(available));

		// Track the total area used
		usedArea.w = std::max(usedArea.w, rendered.w + rendered.x);
		usedArea.h = std::max(usedArea.h, rendered.h + rendered.y);

		if (usedArea.h - m_ScrollAmount > absoluteAvailableArea.h + absoluteAvailableArea.y)
		{
			m_Overflowing = true;
		}
	}

	// Align helper does the same as the leaf node class in CalculateContentArea. This is really a renderArea-specific pass that we could call on all the children and then ourselves here.
	const auto offset = m_AlignHelper.GetOffsets({ usedArea.w, usedArea.h }, { availableArea.w, availableArea.h });
	const auto scale = m_AlignHelper.GetScales({ usedArea.w, usedArea.h }, { availableArea.w, availableArea.h });
	usedArea.w += offset.x;
	usedArea.h += offset.y;

	// Recursively apply content-aware operations to children // TODO: This needs rethinking. Should containers even own renderareas? Should they just be relative? 
	auto scrolledOffset = offset;
	scrolledOffset.y -= m_ScrollAmount;
	ApplyContentAwareTransormations(scale, scrolledOffset);
	//for (auto& renderableArea : m_RenderAreaPerRenderable)
	//{
	//	renderableArea.x += offset.x;
	//	renderableArea.y += offset.y;

	//	renderableArea.x *= scale.x;
	//	renderableArea.y *= scale.y;
	//	renderableArea.w *= scale.x;
	//	renderableArea.h *= scale.y;
	//}

	//m_RenderArea = usedArea;
	//m_RenderArea.y -= m_ScrollAmount;
	//return m_RenderArea;

	RenderArea finalArea;
	finalArea.x = absoluteAvailableArea.x;
	finalArea.y = absoluteAvailableArea.y;
	finalArea.w = std::min(usedArea.w - finalArea.x, absoluteAvailableArea.w);
	finalArea.h = std::min(usedArea.h - finalArea.y, absoluteAvailableArea.h);

	return finalArea;
}

void RUIN::UIContainer::ApplyContentAwareTransormations(const Erm::vec2f& scales, const Erm::vec2f& offsets)
{
	int count = 0;
	for (const auto& renderable : m_Renderables)
	{
		auto& ra = m_RenderAreaPerRenderable[count++];
		ra.x += offsets.x;
		ra.y += offsets.y;

		ra.x *= scales.x;
		ra.y *= scales.y;
		ra.w *= scales.x;
		ra.h *= scales.y;

		//if (ra.IsEmpty())
		//	continue;

		renderable->ApplyContentAwareTransormations(scales, offsets);
	}
}

void RUIN::UIContainer::AddChildWidget(tinyxml2::XMLElement* element)
{
	auto* pWidget = UIManager::GetInstance().CreateWidgetFromType(element->Name(), element);
	m_Renderables.emplace_back(pWidget);
}

size_t RUIN::UIContainer::GetNumChildren() const
{
	return m_Renderables.size();
}

void RUIN::UIContainer::ClearWidgets()
{
	m_Renderables.clear();
	m_RenderAreaPerRenderable.clear();
}

void RUIN::UIContainer::DataSourceChanged()
{
	const auto bufferSize = m_DataSource.GetBufferSize();

	// We always assume buffers are valid and contain enough data for all template instantiations.
	// By that logic, anything over 0 is enough for us to start spawning child widgets
	if (bufferSize == 0)
	{
		// TODO: We should probably not clear, but hide widgets. Otherwise we'll clutter our binding databases.
		// Although, we should probably implement some cleanup functions in the database too.
		m_Renderables.clear();
		return;
	}

	size_t dataRead = 0;

	// Patch in data for existing widgets first.
	unsigned count = 0;
	for (auto& renderable : m_Renderables)
	{
		dataRead += renderable->PatchAllDataFromBuffer(m_DataSource.GetBuffer(dataRead), unsigned(bufferSize - dataRead));

		if (dataRead >= bufferSize)
		{
			break;
		}
	}

	// Resize in case we had too many.
	m_Renderables.resize(count);

	// Instantiate remaining ones
	while (dataRead < bufferSize)
	{
		// assign rather than increment, since dataRead is already added from the offset parameter.
		dataRead = InstantiateItemTemplate(dataRead);
	}
}

size_t RUIN::UIContainer::InstantiateItemTemplate(size_t sourceStreamOffset)
{
	using namespace tinyxml2;
	XMLDocument doc;
	const auto result = doc.Parse(m_ItemTemplate.c_str());

	RASSERT(result == XML_SUCCESS, "ItemTemplate was not valid xml!");

	auto* pRoot = doc.RootElement();

	const char* rootElementName = pRoot->Name();
	RASSERT(strcmp(rootElementName, "ItemTemplate") == 0, "Instantiating item template, but xml doesn't have an itemtemplate root!");

	std::ignore = UIManager::GetInstance().GetBindingDatabase().PushNewContext();

	size_t dataRead = sourceStreamOffset;

	for (auto* e = pRoot->FirstChildElement(); e; e = e->NextSiblingElement())
	{
		AddChildWidget(e);

		auto* pWidget = m_Renderables.rbegin()->get();
		dataRead += pWidget->PatchAllDataFromBuffer(m_DataSource.GetBuffer(dataRead), m_DataSource.GetBufferSize());
		pWidget->SetRowNumber(unsigned(m_Renderables.size() - 1));
	}

	UIManager::GetInstance().GetBindingDatabase().PopContext();

	return dataRead;
}

void RUIN::UIContainer::InitializeVerticalFillmode(const char* mode)
{
	m_AlignHelper.SetVerticalFillMode(mode);
}

void RUIN::UIContainer::InitializeHorizontalFillmode(const char* mode)
{
	m_AlignHelper.SetHorizontalFillMode(mode);
}

void RUIN::UIContainer::InitializeVerticalOverflow(const char* mode)
{
	if (!mode)
		return;

	if (strcmp(mode, "scroll") == 0)
	{
		m_ScrollVertical = true;
 	}
}

void RUIN::UIContainer::InitializeBackgroundColour(const char* col)
{
	m_BackgroundColour = utils::ColourFromHexString(col);
}

bool RUIN::UIContainer::HandleMouseMoved(int cursorX, int cursorY)
{
	return HandleMouseEventGeneric(cursorX, cursorY, [](IRenderable* obj, int x, int y) -> bool { return obj->HandleMouseMoved(x, y); });
}

bool RUIN::UIContainer::HandleMouseDown(int cursorX, int cursorY)
{
	return HandleMouseEventGeneric(cursorX, cursorY, [](IRenderable* obj, int x, int y) -> bool { return obj->HandleMouseDown(x, y); });
}

bool RUIN::UIContainer::HandleMouseUp(int cursorX, int cursorY)
{
	//auto* ptr = dynamic_cast<VerticalBox*>(this);
	//if (!ptr)
		return HandleMouseEventGeneric(cursorX, cursorY, [](IRenderable* obj, int x, int y) -> bool { return obj->HandleMouseUp(x, y); });

	//(void)cursorX;
	//(void)cursorY;
	//m_ScrollAmount += 5;

	//return true;
}

bool RUIN::UIContainer::HandleMouseScroll(float distance, int cursorX, int cursorY)
{
	if (m_ScrollVertical)
	{
		if (distance > 0.f)
		{
			m_ScrollAmount = std::max(m_ScrollAmount - distance, 0.f);
			return true;
		} else if (distance < 0.f && m_Overflowing)
		{
			m_ScrollAmount = std::max(m_ScrollAmount - distance, 0.f);
			return true;
		}
	}

	return HandleMouseEventGeneric(cursorX, cursorY, [](IRenderable* obj, int x, int y, float distance) -> bool { return obj->HandleMouseScroll(distance, x, y); }, distance);
}

size_t RUIN::UIContainer::PatchAllDataFromBuffer(const void* buffer, unsigned bufferSize)
{
	auto dataRead = UIManager::GetInstance().GetBindingDatabase().PatchWidgetDataFromBuffer(buffer, bufferSize, this);

	for (auto& renderable : m_Renderables)
	{
		dataRead += renderable->PatchAllDataFromBuffer((const char*)buffer + dataRead, unsigned(bufferSize - dataRead));
	}

	return dataRead;
}

void RUIN::UIContainer::SetRowNumber(int row)
{
	for (auto& renderable : m_Renderables)
	{
		renderable->SetRowNumber(row);
	}

	m_RowNumber = row;
}

int RUIN::UIContainer::GetRowNumber() const
{
	return m_RowNumber;
}
