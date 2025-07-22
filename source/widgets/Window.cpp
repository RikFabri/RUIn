#include "Window.h"
#include "../UIManager.h"

RUIN::Window::Window(int width, int height) 
	: UIContainer(nullptr)
	, m_Width(width)
	, m_Height(height)
{
}

void RUIN::Window::Render(const RenderArea& targetArea)
{
	RenderArea ra = targetArea;
	ra.w = m_Width;
	ra.h = m_Height;
	//UIManager::GetInstance().DrawRectangle(ra);

	UIContainer::Render(ra);
}

RUIN::RenderArea RUIN::Window::GetAreaForChild(const RenderArea& availableArea, RenderArea& usedArea, RenderContext& ctx) const
{
	RenderArea ra{};
	ra.w = m_Width;
	ra.h = m_Height;

	return ra;
}
