#pragma once
#include "../layouting/LeafNode.h"
#include "../UIManager.h"
#include "tinyxml2.h"
#include <string>
#include <iostream>

namespace RUIN
{
    // Placeholder widget that supports binding width and height
    // This may be deprecated later if it turns out dimensions should be generalized in leafnodes
	class Canvas : public LeafNode
	{
	public:
		Canvas(tinyxml2::XMLElement* element);

        void Render(const RenderArea& targetArea) override;

    private:
        Erm::vec2f GetDimensions() override;

        float m_X, m_Y, m_W, m_H;
    };
}