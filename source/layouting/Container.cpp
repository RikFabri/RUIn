#include "Container.h"
#include <cassert>

#include "../widgets/Label.h"
#include "../UIManager.h"

RUIN::AlignHelper::AlignHelper(tinyxml2::XMLElement* e)
{
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

	for (auto* e = element->FirstChildElement(); e; e = e->NextSiblingElement())
	{
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

	m_RenderAreaPerRenderable.clear();
}

RUIN::RenderArea RUIN::UIContainer::CalculateUsedContentArea(const RenderArea& availableArea)
{
	RenderContext ctx{};
	RenderArea usedArea{};

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
