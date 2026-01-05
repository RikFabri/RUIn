#pragma once
#include "RenderArea.h"
#include "MathLib.h"
#include "../widgets/databinding/DataBinding.h"
#include "../Events.h"

namespace RUIN
{
	class IRenderable
	{
	public:
		virtual ~IRenderable() = default;

		// Draw your content, constrained to targetArea
		virtual void Render(const RenderArea& targetArea) = 0;

		// Return what area you're using within the available area.
		virtual RenderArea CalculateUsedContentArea(const Erm::vec2f& availableArea) = 0;

		// Input events. Return true if you handled the input, false to let them propagate.
		virtual bool PropagateEvent(const Erm::vec2f& position, int cursorX, int cursorY, const Event& event);

		virtual size_t PatchAllDataFromBuffer(const void* buffer, unsigned bufferSize) = 0;

		// Ugly in an interface. Base class?
		virtual void SetRowNumber(int row) = 0;
		virtual int GetRowNumber() const;
	protected:
	};
}