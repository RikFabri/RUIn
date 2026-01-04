#include "Button.h"
#include "../UIManager.h"
#include "databinding/DataBinding.h"


RUIN::Button::Button(tinyxml2::XMLElement* element)
	: UIContainer(element)
{
	Bind_member_to_XML(m_OnClick, element, "onclick");
}

void RUIN::Button::Render(const RenderArea& targetArea)
{
	UIContainer::Render(targetArea);

	if (m_ButtonState == ButtonState::Hovered)
	{
		UIManager::GetInstance().DrawRectangle(targetArea, { 0, 0, 0, 64 });

		//// TODO: this could get messy if client were to tick RUIN outside of system events, which might be needed for animations and transitions later on.
		//m_ButtonState = ButtonState::Default;
	}

	if (m_ButtonState == ButtonState::Down)
	{
		UIManager::GetInstance().DrawRectangle(targetArea, { 0, 0, 0, 128 });

		//// TODO: this could get messy if client were to tick RUIN outside of system events, which might be needed for animations and transitions later on.
		//m_ButtonState = ButtonState::Default;
	}
}

RUIN::RenderArea RUIN::Button::GetAreaForChild(const RenderArea& availableArea, const RenderArea& , const RenderContext& ) const
{
	RenderArea rc = availableArea;

	return rc;
}

bool RUIN::Button::HandleMouseMoved(int, int)
{
	m_ButtonState = ButtonState::Hovered;
	return true;
}

bool RUIN::Button::HandleMouseDown(int, int)
{
	m_ButtonState = ButtonState::Down;
	return true;
}

bool RUIN::Button::HandleMouseUp(int, int)
{
	if (m_OnClick.empty())
		return false;


	UIManager::GetInstance().InvokeNamedCallback(m_OnClick);

	m_ButtonState = ButtonState::Default;

	return true;
}
