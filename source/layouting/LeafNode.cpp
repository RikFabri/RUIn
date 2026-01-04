#include "LeafNode.h"
#include "../UIManager.h"
#include "../utils/utils.h"

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

RUIN::RenderArea RUIN::LeafNode::CalculateUsedContentArea(const Erm::vec2f& availableArea)
{
	const auto dims = GetDimensions();

	RenderArea usedArea{};
	usedArea.w = dims.w;
	usedArea.h = dims.h;

	const auto offsets = m_AlignHelper.GetOffsets(dims, availableArea);
	const auto scales = m_AlignHelper.GetScales(dims, availableArea);

	usedArea.w *= scales.x;
	usedArea.h *= scales.y;

	usedArea.x += offsets.x;
	usedArea.y += offsets.y;
	
	// Fit inside of area // TODO: this should be handled by alignment offsets and cliprect I think?
	//usedArea.w = std::min(usedArea.w, availableArea.w);
	//usedArea.h = std::min(usedArea.h, availableArea.h);

	// Apply margins
	// We use pixel / size as a ratio and apply it to calculated area.
	usedArea.x += m_MarginLeft / dims.x * usedArea.w;
	usedArea.y += m_MarginTop / dims.y * usedArea.h;
	usedArea.w -= m_MarginRight / dims.x * usedArea.w;
	usedArea.h -= m_MarginBottom / dims.y * usedArea.h;
	// End margins

	return usedArea;
}

void RUIN::LeafNode::ApplyContentAwareTransormations(const Erm::vec2f&, const Erm::vec2f&)
{
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
	m_BackgroundColour = utils::ColourFromHexString(col);
}
