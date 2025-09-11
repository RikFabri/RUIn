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
	if (!mode)
	{
		m_VerticalFillMode = FillMode::Left;
		return;
	}

	m_VerticalFillMode = GetFillMode(mode);
}

void RUIN::LeafNode::InitializeHorizontalFillmode(const char* mode)
{
	if (!mode)
	{
		m_HorizontalFillMode = FillMode::Left;
		return;
	}

	m_HorizontalFillMode = GetFillMode(mode);
}

RUIN::LeafNode::FillMode RUIN::LeafNode::GetFillMode(const char* fillMode)
{
	if (strcmp(fillMode, "right") == 0)
	{
		return FillMode::Right;
	}
	else if (strcmp(fillMode, "center") == 0)
	{
		return FillMode::Center;
	}
	else if (strcmp(fillMode, "stretch") == 0)
	{
		return FillMode::Stretch;
	}

	return FillMode::Left;
}
