#pragma once
#include "Renderable.h"
#include "tinyxml2.h"
#include "MathLib.h"

namespace RUIN
{
	class LeafNode : public IRenderable
	{
	public:
		LeafNode(tinyxml2::XMLElement* e);

		RenderArea CalculateUsedContentArea(const RenderArea& availableArea) final;

	private:
		virtual Erm::vec2f GetDimensions() = 0;

		enum class FillMode
		{
			Left,
			Right,
			Center,
			Stretch
		};
		FillMode m_HorizontalFillMode;
		FillMode m_VerticalFillMode;
		Erm::vec2f m_MarginLeftTop;
		Erm::vec2f m_MarginRightBottom;
	};

}