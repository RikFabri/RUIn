#include "Renderable.h"

bool RUIN::IRenderable::PropagateEvent(const Erm::vec2f&, int, int, const Event&)
{
	return false;
}

int RUIN::IRenderable::GetRowNumber() const
{
	return 0;
}
