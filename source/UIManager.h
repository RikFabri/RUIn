#pragma once
#include "Singleton.h"
#include "widgets/Window.h"
#include "include/RUIN.h"

namespace RUIN
{

	class UIManager final : public Singleton<UIManager>
	{
	public:
		void Update();
		void Render();

		void SetDrawRectangleCb(DrawRectangleCb cb);
		void DrawRectangle(const RenderArea& ra) const;
	private:
		friend class Singleton<UIManager>;
		UIManager();

		Window m_Window;

		DrawRectangleCb m_DrawRectangle;

	};

}