#pragma once
#include "../layouting/Container.h"

namespace RUIN
{

class Window : public UIContainer
{
public:
	Window(int width, int height);

	void Render(const RenderArea& targetArea) override;

	void SetDimensions(unsigned width, unsigned height);

private:
	float m_Width, m_Height;

	virtual RUIN::RenderArea GetAreaForChild(const RenderArea& availableArea, const RenderArea& usedArea, const RenderContext& ctx) const override;
};

}