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

		float m_MarginLeft = 0.f;
		float m_MarginTop = 0.f;
		float m_MarginRight = 0.f;
		float m_MarginBottom = 0.f;

		void InitializeVerticalFillmode(const char* mode);
		void InitializeHorizontalFillmode(const char* mode);
		static FillMode GetFillMode(const char* fillMode);

	};

}