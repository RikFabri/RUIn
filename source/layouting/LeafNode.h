#pragma once
#include "Renderable.h"
#include "tinyxml2.h"
#include "MathLib.h"
#include "AlignHelper.h"

namespace RUIN
{
	class LeafNode : public IRenderable
	{
	public:
		LeafNode(tinyxml2::XMLElement* e);

		RenderArea CalculateUsedContentArea(const Erm::vec2f& availableArea) final;
		void ApplyContentAwareTransormations(const Erm::vec2f& scales, const Erm::vec2f& offsets) final;
		size_t PatchAllDataFromBuffer(const void* buffer, unsigned bufferSize) final;
		void SetRowNumber(int row) final;
		int GetRowNumber() const final;

		void Render(const RenderArea& targetArea) override;
	private:
		virtual Erm::vec2f GetDimensions() = 0;

		AlignHelper m_AlignHelper;

		RUIN_Colour m_BackgroundColour = { 0,0,0,0 };

		float m_MarginLeft = 0.f;
		float m_MarginTop = 0.f;
		float m_MarginRight = 0.f;
		float m_MarginBottom = 0.f;

		void InitializeVerticalFillmode(const char* mode);
		void InitializeHorizontalFillmode(const char* mode);

		void InitializeBackgroundColour(const char* col);

		unsigned m_RowNumber = 0;

	};

}