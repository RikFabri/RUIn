#include "Container.h"
#include <cassert>

#include "../widgets/databinding/DataBinding.h"
#include "../widgets/Label.h"
#include "../UIManager.h"


RUIN::AlignHelper::AlignHelper(tinyxml2::XMLElement* e)
{
	// TODO: Should these use binding macros?
	m_Stretch = false;
	if (e == nullptr)
	{
		m_HorizontalAlign = Align::Left;
		m_VerticalAlign = Align::Left;
		return;
	}

	const auto getAlignment = [](const char* str)
		{
			if (str == nullptr)
				return Align::Left;

			if (strcmp(str, "center") == 0)
			{
				return Align::Center;
			}
			else if (strcmp(str, "right") == 0)
			{
				return Align::Right;
			}
			else
			{
				return Align::Left;
			}
		};

	m_HorizontalAlign = getAlignment(e->Attribute("horizontal-align"));
	m_VerticalAlign = getAlignment(e->Attribute("vertical-align"));

	e->QueryBoolAttribute("scale-to-fit", &m_Stretch);
}

Erm::vec2f RUIN::AlignHelper::GetOffsets(float w, float h, const Erm::vec2f& dims) const
{
	const auto getOffset = [](Align a, float width, float maxWidth)
		{
			switch (a)
			{
			case Align::Center:
				return (maxWidth - width) / 2.f;
			case Align::Right:
				return maxWidth - width;
			case Align::Left:
			default:
				return 0.f;
			}
		};

	const auto hor = getOffset(m_HorizontalAlign, dims.x, w);
	const auto ver = getOffset(m_VerticalAlign, dims.y, h);

	return Erm::vec2f(hor, ver);
}

Erm::vec2f RUIN::AlignHelper::GetScales(Erm::vec2f availableArea, Erm::vec2f usedArea) const
{
	if (!m_Stretch)
	{
		return { 1.f, 1.f };
	}

	return availableArea / usedArea;
}

RUIN::UIContainer::UIContainer(tinyxml2::XMLElement* element)
	: m_AlignHelper(element)
{
	if (!element)
	{
		return; 
	}

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

void RUIN::UIContainer::Render(const RenderArea& targetArea)
{
	int count = 0;
	for (const auto& renderable : m_Renderables)
	{
		auto& ra = m_RenderAreaPerRenderable[count++];
		ra.x += targetArea.x;
		ra.y += targetArea.y;
		renderable->Render(ra);
	}
}

RUIN::RenderArea RUIN::UIContainer::CalculateUsedContentArea(const RenderArea& availableArea)
{
	RenderContext ctx{};
	RenderArea usedArea{};

	m_RenderAreaPerRenderable.clear();
	m_RenderAreaPerRenderable.reserve(m_Renderables.size());
	for (auto& renderable : m_Renderables)
	{
		// The canvas the child element is allowed to use
		const auto available = GetAreaForChild(availableArea, usedArea, ctx);

		// The area within the canvas actually used
		m_RenderAreaPerRenderable.emplace_back(renderable->CalculateUsedContentArea(available));
		const auto& rendered = m_RenderAreaPerRenderable[ctx.childIndex];
		assert(ctx.childIndex == m_RenderAreaPerRenderable.size() - 1);

		// Track the total area used within this container
		usedArea.w = std::max(usedArea.w, rendered.w + rendered.x);
		usedArea.h = std::max(usedArea.h, rendered.h + rendered.y);
		++ctx.childIndex;
	}

	// Align helper does the same as the leaf node class in CalculateContentArea. This is really a renderArea-specific pass that we could call on all the children and then ourselves here.
	const auto offset = m_AlignHelper.GetOffsets(availableArea.w, availableArea.h, { usedArea.w, usedArea.h });
	const auto scale = m_AlignHelper.GetScales({ availableArea.w, availableArea.h }, { usedArea.w, usedArea.h });
	usedArea.w += offset.x;
	usedArea.h += offset.y;

	for (auto& renderableArea : m_RenderAreaPerRenderable)
	{
		renderableArea.x += offset.x;
		renderableArea.y += offset.y;

		renderableArea.x *= scale.x;
		renderableArea.y *= scale.y;
		renderableArea.w *= scale.x;
		renderableArea.h *= scale.y;
	}

	return usedArea;
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

bool RUIN::UIContainer::HandleMouseEventGeneric(int cursorX, int cursorY, std::function<bool(IRenderable*, int, int)> func)
{
	int count = 0;
	for (auto& renderable : m_Renderables)
	{
		if (count >= m_RenderAreaPerRenderable.size())
		{
			return false;
		}

		const auto& renderArea = m_RenderAreaPerRenderable[count++];

		if (renderArea.ContainsPoint(cursorX, cursorY))
		{
			const bool handled = func(renderable.get(), cursorX, cursorY);
			if (handled)
			{
				return true;
			}
		}

	}

	return false;
}

void RUIN::UIContainer::DataSourceChanged()
{
	// We always assume buffers are valid and contain enough data for all template instantiations.
	// By that logic, anything over 0 is enough for us to start spawning child widgets
	if (m_DataSource.GetBufferSize() == 0)
	{
		return;
	}

	InstantiateItemTemplate();
}

void RUIN::UIContainer::InstantiateItemTemplate()
{
	using namespace tinyxml2;
	XMLDocument doc;
	const auto result = doc.Parse(m_ItemTemplate.c_str());

	RASSERT(result == XML_SUCCESS, "ItemTemplate was not valid xml!");

	auto* pRoot = doc.RootElement();

	const char* rootElementName = pRoot->Name();
	RASSERT(strcmp(rootElementName, "ItemTemplate") == 0, "Instantiating item template, but xml doesn't have an itemtemplate root!");

	const auto contextId = UIManager::GetInstance().GetBindingDatabase().PushNewContext();
	m_ContextIdPerInstantiatedTemplate.emplace_back(contextId);

	size_t dataRead = 0;

	for (auto* e = pRoot->FirstChildElement(); e; e = e->NextSiblingElement())
	{
		AddChildWidget(e);

		auto* pWidget = m_Renderables.rbegin()->get();
		dataRead += UIManager::GetInstance().GetBindingDatabase().PatchWidgetDataFromBuffer(m_DataSource.GetBuffer(dataRead), m_DataSource.GetBufferSize(), pWidget, contextId);
	}

	UIManager::GetInstance().GetBindingDatabase().PopContext();


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
	return HandleMouseEventGeneric(cursorX, cursorY, [](IRenderable* obj, int x, int y) -> bool { return obj->HandleMouseUp(x, y); });
}
