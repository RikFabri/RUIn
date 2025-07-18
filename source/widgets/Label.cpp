#include "Label.h"

RUIN::Label::Label(tinyxml2::XMLElement* element)
	: LeafNode(element)
{
	m_Text = element->GetText();
}

Erm::vec2f RUIN::Label::GetDimensions()
{
	return Erm::vec2f(150, 10);
}