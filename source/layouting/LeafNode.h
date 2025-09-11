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

		RenderArea CalculateUsedContentArea(const RenderArea& availableArea) final;
		size_t PatchAllDataFromBuffer(void* buffer, unsigned bufferSize, unsigned bindingContextId) final;
		void SetRowNumber(int row) final;
		int GetRowNumber() const final;

	private:
		virtual Erm::vec2f GetDimensions() = 0;

		AlignHelper m_AlignHelper;

		float m_MarginLeft = 0.f;
		float m_MarginTop = 0.f;
		float m_MarginRight = 0.f;
		float m_MarginBottom = 0.f;

		void InitializeVerticalFillmode(const char* mode);
		void InitializeHorizontalFillmode(const char* mode);

		int m_RowNumber = 0;

	};

}