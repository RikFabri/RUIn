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
	const bool setClipRect =
		m_VerticalOverflow != Overflow::Visible &&
		m_Overflowing;

	if (setClipRect)
	{
		UIManager::GetInstance().SetClipRect(renderArea);
	}

	int count = 0;
	for (const auto& renderable : m_Renderables)
	{
		RenderArea ra = m_RenderAreaPerRenderable[count++];
		ra.x += renderArea.x;
		ra.y += renderArea.y;

		renderable->Render(ra);
	}

	if (setClipRect)
	{
		UIManager::GetInstance().SetClipRect(std::nullopt);
	}
}

RUIN::RenderArea RUIN::UIContainer::CalculateUsedContentArea(const Erm::vec2f& availableArea)
{
	RenderContext ctx{};

	Erm::vec2f area = availableArea;

	m_Overflowing = false;

	// Currently used as vec2, but may be useful to keep as area for fancy content-alignment later
	RenderArea usedArea{};

	// Render areas are not exposed to leaf nodes, they're an implementation detail of containers.
	m_RenderAreaPerRenderable.clear();
	m_RenderAreaPerRenderable.reserve(m_Renderables.size());
	for (auto& renderable : m_Renderables)
	{
		// The canvas the child element is allowed to use (container behaviour)
		const auto availableRenderArea = GetAreaForChild(area, usedArea, ctx);
		auto availableDimensions = availableRenderArea.GetDimensions();
		const auto availablePosition = availableRenderArea.GetPosition();

		++ctx.childIndex;


		if (!RenderArea{0.f, 0.f, availableArea.x, availableArea.y}.Contains(availableRenderArea))
		{
			m_Overflowing = true;

			if (m_VerticalOverflow != Overflow::Hidden)
			{
				area.h = INFINITY;
				availableDimensions.h = INFINITY;
			}
		}

		// Traverse widget tree, let widgets take up all or part of the available space
		auto& usedByChild = m_RenderAreaPerRenderable.emplace_back(
			renderable->CalculateUsedContentArea(availableDimensions));

		usedByChild.OffsetBy(availablePosition);

		// Track the total area used
		usedArea = GetCombinedBounds(usedArea, usedByChild);
	}

	// // Content aware operations
	// Align helper does the same as the leaf node class in CalculateContentArea. This is really a renderArea-specific pass that we could call on all the children and then ourselves here.
	// AD: we also duplicated background between leaf and container node. Is this pointing to a base widget class?
	const auto offset = m_AlignHelper.GetOffsets({ usedArea.w, usedArea.h }, area);
	const auto scale = m_AlignHelper.GetScales({ usedArea.w, usedArea.h }, area);

	usedArea.x += offset.x;
	usedArea.y += offset.y - m_ScrollAmount;
	usedArea.w *= scale.x;
	usedArea.h *= scale.y;

	// Should scale really be applied directly to the final renderAreas?
	for (auto& renderArea : m_RenderAreaPerRenderable)
	{
		renderArea.w *= scale.w;
		renderArea.h *= scale.h;
	}

	usedArea.h = std::min(usedArea.h, availableArea.h);

	return usedArea;
}

void RUIN::UIContainer::ApplyContentAwareTransormations(const Erm::vec2f& , const Erm::vec2f& )
{
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

RUIN::RenderArea RUIN::UIContainer::GetCombinedBounds(RenderArea left, RenderArea right)
{
	left.w += left.x;
	left.h += left.y;

	right.w += right.x;
	right.h += right.y;

	RenderArea result;
	result.x = std::min(left.x, right.x);
	result.y = std::min(left.y, right.y);
	result.w = std::max(left.w, right.w);
	result.h = std::max(left.h, right.h);

	result.w -= result.x;
	result.h -= result.y;

	return result;
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
		m_VerticalOverflow = Overflow::Scroll;
 	}
	if (strcmp(mode, "visible") == 0)
	{
		m_VerticalOverflow = Overflow::Visible;
 	}
	if (strcmp(mode, "hidden") == 0)
	{
		m_VerticalOverflow = Overflow::Hidden;
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
	if (m_VerticalOverflow == Overflow::Scroll)
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
