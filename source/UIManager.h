#pragma once
#include "Singleton.h"
#include "widgets/Window.h"
#include "include/RUIN.h"

#include <string>
#include <unordered_map>
#include <functional>

namespace RUIN
{

	class UIManager final : public Singleton<UIManager>
	{
	public:
		void LoadXML(const std::string& path);

		void Update();
		void Render();

		void SetDrawRectangleCb(DrawRectangleCb cb);
		void DrawRectangle(const RenderArea& ra) const;


		using WidgetFactoryFn = std::function<IRenderable*(tinyxml2::XMLElement*)>;
		void RegisterWidgetFactory(const std::string& widgetType, const WidgetFactoryFn& factoryFunc);
		IRenderable* CreateWidgetFromType(const std::string& widgetType, tinyxml2::XMLElement* pElement);
	private:
		friend class Singleton<UIManager>;
		UIManager();

		void RegisterBuiltInWidgetFactories();

		Window m_Window;

		DrawRectangleCb m_DrawRectangle;

		std::unordered_map<std::string, WidgetFactoryFn> m_WidgetFactories;
	};

}