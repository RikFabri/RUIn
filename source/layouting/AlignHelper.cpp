#include "AlignHelper.h"
#include <cstring>

void RUIN::AlignHelper::SetVerticalFillMode(const char* fillMode)
{
	if (!fillMode)
	{
		m_VerticalFillMode = FillMode::Left;
		return;
	}

	m_VerticalFillMode = GetFillMode(fillMode);
}

void RUIN::AlignHelper::SetHorizontalFillMode(const char* fillMode)
{
	if (!fillMode)
	{
		m_HorizontalFillMode = FillMode::Left;
		return;
	}

	m_HorizontalFillMode = GetFillMode(fillMode);
}

Erm::vec2f RUIN::AlignHelper::GetOffsets(Erm::vec2f dimensions, Erm::vec2f availableArea)
{
	Erm::vec2f offset{ 0.f, 0.f };

	switch (m_HorizontalFillMode)
	{
	case FillMode::Center:
		offset.x += (availableArea.w - dimensions.w) / 2.f;
		break;
	case FillMode::Right:
		offset.x += availableArea.w - dimensions.w;
		break;
	}

	switch (m_VerticalFillMode)
	{
	case FillMode::Center:
		offset.y += (availableArea.h - dimensions.h) / 2.f;
		break;
	case FillMode::Right:
		offset.y += availableArea.h - dimensions.h;
		break;
	}

	return offset;
}

Erm::vec2f RUIN::AlignHelper::GetScales(Erm::vec2f dimensions, Erm::vec2f availableArea)
{
	Erm::vec2f scales{ 1.f, 1.f };

	if (m_HorizontalFillMode == FillMode::Stretch)
	{
		scales.x = availableArea.x / dimensions.x;
	}

	if (m_VerticalFillMode == FillMode::Stretch)
	{
		scales.y = availableArea.y / dimensions.y;
	}

	return scales;
}

RUIN::AlignHelper::FillMode RUIN::AlignHelper::GetFillMode(const char* fillMode)
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
