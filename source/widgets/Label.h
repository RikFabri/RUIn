#pragma once
#include "../layouting/LeafNode.h"
#include "../UIManager.h"
#include "tinyxml2.h"
#include <string>
#include <iostream>

namespace RUIN
{
	class Label : public LeafNode
	{
	public:
		Label(tinyxml2::XMLElement* element);

		void Render(const RenderArea& targetArea) override;

	private:
		void OnTextChanged();
		Erm::vec2f GetDimensions() override;

		std::string m_Text;

		ClientTexture m_pTextImage;
	};

}