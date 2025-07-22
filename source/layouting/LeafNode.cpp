#include "LeafNode.h"
#include <algorithm>

RUIN::LeafNode::LeafNode(tinyxml2::XMLElement* e)
	: IRenderable()
{
	const auto getFillMode = [](const char* stretchMode)
		{
			if (strcmp(stretchMode, "right") == 0)
			{
				return FillMode::Right;
			}
			else if (strcmp(stretchMode, "center") == 0)
			{
				return FillMode::Center;
			}
			else if (strcmp(stretchMode, "stretch") == 0)
			{
				return FillMode::Stretch;
			}

			return FillMode::Left;
		};

	const char* stretchMode = "left";
	e->QueryStringAttribute("horizontal-fillMode", &stretchMode);
	m_HorizontalFillMode = getFillMode(stretchMode);

	stretchMode = "left";
	e->QueryStringAttribute("vertical-fillMode", &stretchMode);
	m_VerticalFillMode = getFillMode(stretchMode);

	float marginLeft = 0;
	e->QueryFloatAttribute("margin-left", &marginLeft);

	float marginRight = 0;
	e->QueryFloatAttribute("margin-right", &marginRight);

	float marginTop = 0;
	e->QueryFloatAttribute("margin-top", &marginTop);

	float marginBottom = 0;
	e->QueryFloatAttribute("margin-bottom", &marginBottom);

	m_MarginLeftTop.x = marginLeft;
	m_MarginLeftTop.y = marginTop;
	m_MarginRightBottom.x = marginRight;
	m_MarginRightBottom.y = marginBottom;
}

RUIN::RenderArea RUIN::LeafNode::CalculateUsedContentArea(const RenderArea& availableArea)
{
	const auto dims = GetDimensions();

	RenderArea rc = availableArea;

	switch (m_HorizontalFillMode)
	{
	case LeafNode::FillMode::Center:
		rc.x += (availableArea.w - dims.x) / 2.f;
		break;
	case LeafNode::FillMode::Right:
		rc.x += availableArea.w - dims.x;
		break;
	}

	if (m_HorizontalFillMode != LeafNode::FillMode::Stretch)
	{
		rc.w = dims.x;
	}

	switch (m_VerticalFillMode)
	{
	case LeafNode::FillMode::Right:
		rc.y += availableArea.h - dims.y;
		break;
	case LeafNode::FillMode::Center:
		rc.y += (availableArea.h - dims.y) / 2.f;
		break;
	}

	if (m_VerticalFillMode != LeafNode::FillMode::Stretch)
	{
		rc.h = dims.y;
	}

	// Fit inside of area
	rc.w = std::min(rc.w, availableArea.w);
	rc.h = std::min(rc.h, availableArea.h);

	// Apply margins
	// We use pixel / size as a ratio and apply it to calculated area.
	rc.x += m_MarginLeftTop.x / dims.x * rc.w;
	rc.y += m_MarginLeftTop.y / dims.y * rc.h;
	rc.w -= m_MarginRightBottom.x / dims.x * rc.w;
	rc.h -= m_MarginRightBottom.y / dims.y * rc.h;
	// End margins

	return rc;
}