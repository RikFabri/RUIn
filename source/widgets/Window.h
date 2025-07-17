#pragma once
#include "../layouting/Container.h"

namespace RUIN
{

class Window : public UIContainer
{
public:
	Window(int width, int height);

	void Render(const RenderArea& targetArea) override;

private:
	int m_Width, m_Height;

	virtual RenderArea GetAreaForChild(const RenderArea& availableArea, RenderArea& usedArea, RenderContext& ctx) const override;
};

}