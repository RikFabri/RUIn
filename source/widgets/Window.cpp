#include "Window.h"
#include "../UIManager.h"

RUIN::Window::Window(int width, int height) 
	: UIContainer(nullptr)
	, m_Width((float)width)
	, m_Height((float)height)
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

void RUIN::Window::SetDimensions(unsigned width, unsigned height)
{
	m_Width = (float)width;
	m_Height = (float)height;
}

RUIN::RenderArea RUIN::Window::GetAreaForChild(const Erm::vec2f&, const RenderArea& , const RenderContext&) const
{
	RenderArea ra{};
	ra.w = m_Width;
	ra.h = m_Height;

	return ra;
}
