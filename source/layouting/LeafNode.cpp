#include "LeafNode.h"
#include "../UIManager.h"

#include <cctype>

RUIN::LeafNode::LeafNode(tinyxml2::XMLElement* e)
	: IRenderable()
{
	Bind_method_to_XML(InitializeHorizontalFillmode, e, "horizontal-fillMode");
	Bind_method_to_XML(InitializeVerticalFillmode, e, "vertical-fillMode");
	Bind_method_to_XML(InitializeBackgroundColour, e, "background-colour");

	Bind_member_to_XML(m_MarginLeft, e, "margin-left");
	Bind_member_to_XML(m_MarginRight, e, "margin-right");
	Bind_member_to_XML(m_MarginTop, e, "margin-top");
	Bind_member_to_XML(m_MarginBottom, e, "margin-bottom");

}

RUIN::RenderArea RUIN::LeafNode::CalculateUsedContentArea(const RenderArea& availableArea)
{
	const auto dims = GetDimensions();

	RenderArea rc = availableArea;
	rc.w = dims.x;
	rc.h = dims.y;

	const auto offsets = m_AlignHelper.GetOffsets(dims, { availableArea.w, availableArea.h });
	const auto scales = m_AlignHelper.GetScales(dims, { availableArea.w, availableArea.h });

	rc.w *= scales.x;
	rc.h *= scales.y;

	rc.x += offsets.x;
	rc.y += offsets.y;
	
	// Fit inside of area
	rc.w = std::min(rc.w, availableArea.w);
	rc.h = std::min(rc.h, availableArea.h);

	// Apply margins
	// We use pixel / size as a ratio and apply it to calculated area.
	rc.x += m_MarginLeft / dims.x * rc.w;
	rc.y += m_MarginTop / dims.y * rc.h;
	rc.w -= m_MarginRight / dims.x * rc.w;
	rc.h -= m_MarginBottom / dims.y * rc.h;
	// End margins

	return rc;
}

size_t RUIN::LeafNode::PatchAllDataFromBuffer(const void* buffer, unsigned bufferSize)
{
	return UIManager::GetInstance().GetBindingDatabase().PatchWidgetDataFromBuffer(buffer, bufferSize, this);
}

void RUIN::LeafNode::SetRowNumber(int row)
{
	m_RowNumber = row;
}

int RUIN::LeafNode::GetRowNumber() const
{
	return m_RowNumber;
}

void RUIN::LeafNode::Render(const RenderArea& targetArea)
{
	UIManager::GetInstance().DrawRectangle(targetArea, m_BackgroundColour);
}

void RUIN::LeafNode::InitializeVerticalFillmode(const char* mode)
{
	m_AlignHelper.SetVerticalFillMode(mode);
}

void RUIN::LeafNode::InitializeHorizontalFillmode(const char* mode)
{
	m_AlignHelper.SetHorizontalFillMode(mode);
}

void RUIN::LeafNode::InitializeBackgroundColour(const char* col)
{
	RASSERTF(col[0] == '#', "{} was not a valid hex colour, did not start with #", col);
	RASSERTF(strlen(col) == 9, "{} was not a valid hex colour, it was not 9 characters", col);

	constexpr auto HexToVal = [](char c) -> uint8_t
		{
			if (c >= '0' && c <= '9')
			{
				const auto res = c - '0'; (void)res;
				return c - '0';
			}

			c = (char)std::toupper(c);
			
			if (c >= 'A' && c <= 'F')
			{
				return 10 + c - 'A';
			}

			RASSERT(false, "%c is not a valid hex character");
			return 0;
		};

	uint8_t val = 0;
	val += HexToVal(col[1]) * 16;
	val += HexToVal(col[2]);

	m_BackgroundColour.r = val;

	val = 0;
	val += HexToVal(col[3]) * 16;
	val += HexToVal(col[4]);

	m_BackgroundColour.g = val;

	val = 0;
	val += HexToVal(col[5]) * 16;
	val += HexToVal(col[6]);

	m_BackgroundColour.b = val;

	val = 0;
	val += HexToVal(col[7]) * 16;
	val += HexToVal(col[8]);

	m_BackgroundColour.a = val;
}
