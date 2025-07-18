#pragma once
#include <vector>
#include <memory>

#include "Renderable.h"
#include "tinyxml2.h"
#include "MathLib.h"

namespace RUIN
{
	class AlignHelper
	{
	public:
		AlignHelper(tinyxml2::XMLElement* e);
		Erm::vec2f GetOffsets(float w, float h, const Erm::vec2f& dims) const;
		Erm::vec2f GetScales(Erm::vec2f availableArea, Erm::vec2f usedArea) const;
	private:
		enum class Align
		{
			Left,
			Center,
			Right
		};
		Align m_HorizontalAlign;
		Align m_VerticalAlign;
		bool m_Stretch;
	};

	class UIContainer : public IRenderable 
	{
	public:
		UIContainer(tinyxml2::XMLElement* element);

		void Render(const RenderArea& targetArea) override;
		RenderArea CalculateUsedContentArea(const RenderArea& availableArea) override;

		void AddChildWidget(tinyxml2::XMLElement* element);

		size_t GetNumChildren() const;


	protected:
		struct RenderContext
		{
			int childIndex;
		};


	private:
		virtual RenderArea GetAreaForChild(const RenderArea& availableArea, RenderArea& usedArea, RenderContext& ctx) const = 0;

		AlignHelper m_AlignHelper;

		std::vector<std::unique_ptr<IRenderable>> m_Renderables;
		std::vector<RenderArea> m_RenderAreaPerRenderable;
	};

}