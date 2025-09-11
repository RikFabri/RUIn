#pragma once
#include "MathLib.h"
#include "RenderArea.h"

namespace RUIN
{
	class AlignHelper
	{
	public:
		void SetVerticalFillMode(const char* fillMode);
		void SetHorizontalFillMode(const char* fillMode);

		Erm::vec2f GetOffsets(Erm::vec2f dimensions, Erm::vec2f availableArea);
		Erm::vec2f GetScales(Erm::vec2f dimensions, Erm::vec2f availableArea);

	private:

		enum class FillMode
		{
			Left,
			Right,
			Center,
			Stretch
		};

		FillMode m_HorizontalFillMode = FillMode::Left;
		FillMode m_VerticalFillMode = FillMode::Left;

		static FillMode GetFillMode(const char* fillMode);
	};
}
