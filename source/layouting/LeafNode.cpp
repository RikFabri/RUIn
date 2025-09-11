#include "LeafNode.h"
#include "../UIManager.h"

RUIN::LeafNode::LeafNode(tinyxml2::XMLElement* e)
	: IRenderable()
{
	Bind_method_to_XML(InitializeHorizontalFillmode, e, "horizontal-fillMode");
	Bind_method_to_XML(InitializeVerticalFillmode, e, "vertical-fillMode");

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

size_t RUIN::LeafNode::PatchAllDataFromBuffer(void* buffer, unsigned bufferSize, unsigned bindingContextId)
{
	return UIManager::GetInstance().GetBindingDatabase().PatchWidgetDataFromBuffer(buffer, bufferSize, this, bindingContextId);
}

void RUIN::LeafNode::SetRowNumber(int row)
{
	m_RowNumber = row;
}

int RUIN::LeafNode::GetRowNumber() const
{
	return m_RowNumber;
}

void RUIN::LeafNode::InitializeVerticalFillmode(const char* mode)
{
	m_AlignHelper.SetVerticalFillMode(mode);
}

void RUIN::LeafNode::InitializeHorizontalFillmode(const char* mode)
{
	m_AlignHelper.SetHorizontalFillMode(mode);
}