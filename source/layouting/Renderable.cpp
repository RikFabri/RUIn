#include "Renderable.h"

bool RUIN::IRenderable::HandleMouseMoved(int, int)
{
	return false;
}

bool RUIN::IRenderable::HandleMouseDown(int, int)
{
	return false;
}

bool RUIN::IRenderable::HandleMouseUp(int, int)
{
	return false;
}

int RUIN::IRenderable::GetRowNumber() const
{
	return 0;
}
