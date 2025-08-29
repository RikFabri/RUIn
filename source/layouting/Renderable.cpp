#include "Renderable.h"

bool RUIN::IRenderable::HandleMouseMoved(int cursorX, int cursorY)
{
	return false;
}

bool RUIN::IRenderable::HandleMouseDown(int cursorX, int cursorY)
{
	return false;
}

bool RUIN::IRenderable::HandleMouseUp(int cursorX, int cursorY)
{
	return false;
}

int RUIN::IRenderable::GetRowNumber() const
{
	return 0;
}
