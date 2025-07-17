#include "Container.h"
#include <cassert>

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

RenderArea RUIN::UIContainer::CalculateUsedContentArea(const RenderArea& availableArea)
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

	//// Align helper does the same as the leaf node class in CalculateContentArea. This is really a renderArea-specific pass that we could call on all the children and then ourselves here.
	//const auto offset = m_AlignHelper.GetOffsets(availableArea.w, availableArea.h, { usedArea.w, usedArea.h });
	//const auto scale = m_AlignHelper.GetScales({ availableArea.w, availableArea.h }, { usedArea.w, usedArea.h });
	//usedArea.w += offset.x;
	//usedArea.h += offset.y;

	//for (auto& renderableArea : m_RenderAreaPerRenderable)
	//{
	//	renderableArea.x += offset.x;
	//	renderableArea.y += offset.y;

	//	renderableArea.x *= scale.x;
	//	renderableArea.y *= scale.y;
	//	renderableArea.w *= scale.x;
	//	renderableArea.h *= scale.y;
	//}

	return usedArea;
}

void RUIN::UIContainer::AddChildWidget(IRenderable* pWidget)
{
	m_Renderables.emplace_back(pWidget);
}
