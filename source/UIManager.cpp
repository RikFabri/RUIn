#include "UIManager.h"

RUIN::UIManager::UIManager()
	: m_Window( 200, 100 )
	, m_DrawRectangle{ nullptr }
{
}

void RUIN::UIManager::Update()
{
	RenderArea ra{};
	m_Window.CalculateUsedContentArea(ra);
}

void RUIN::UIManager::Render()
{
	RenderArea ra{};
	m_Window.Render(ra);
}

void RUIN::UIManager::SetDrawRectangleCb(DrawRectangleCb cb)
{
	m_DrawRectangle = cb;
}

void RUIN::UIManager::DrawRectangle(const RenderArea& ra) const
{
	m_DrawRectangle(ra.x, ra.y, ra.w, ra.h);
}
