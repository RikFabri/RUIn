#include "Label.h"
#include "../UIManager.h"

RUIN::Label::Label(tinyxml2::XMLElement* element)
	: LeafNode(element)
{
	m_Text = element->GetText();
	m_pTextImage = UIManager::GetInstance().AllocateTextureFromText(m_Text);
}

void RUIN::Label::Render(const RenderArea& targetArea)
{
	UIManager::GetInstance().DrawRectangle(targetArea);
	UIManager::GetInstance().DrawTexture(m_pTextImage, targetArea);
}

Erm::vec2f RUIN::Label::GetDimensions()
{
	uint32_t w, h;
	UIManager::GetInstance().QueryTextureDimensions(m_pTextImage, w, h);
	return Erm::vec2f((float)w, (float)h);
}