#pragma once
#include <vector>
#include <memory>

#include "Renderable.h"

namespace RUIN
{

	class UIContainer : public IRenderable 
	{
	public:
		void Render(const RenderArea& targetArea) override;
		RenderArea CalculateUsedContentArea(const RenderArea& availableArea) override;

		void AddChildWidget(IRenderable* pWidget);

	protected:
		struct RenderContext
		{
			int childIndex;
		};


	private:
		virtual RenderArea GetAreaForChild(const RenderArea& availableArea, RenderArea& usedArea, RenderContext& ctx) const = 0;


		std::vector<IRenderable*> m_Renderables;
		std::vector<RenderArea> m_RenderAreaPerRenderable;
	};

}