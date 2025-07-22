#pragma once
#include "Singleton.h"
#include "widgets/Window.h"
#include "include/RUIN.h"
#include "include/Commands.h"

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

		void SetCallbacks(const Callbacks& cb);
		void DrawRectangle(const RenderArea& ra) const;
		void* AllocateTextureFromText(const std::string& text) const;
		void QueryTextureDimensions(const void* texture, uint32_t& width, uint32_t& height) const;
		void DrawTexture(void* texture, const RenderArea& ra) const;


		using WidgetFactoryFn = std::function<IRenderable*(tinyxml2::XMLElement*)>;
		void RegisterWidgetFactory(const std::string& widgetType, const WidgetFactoryFn& factoryFunc);
		IRenderable* CreateWidgetFromType(const std::string& widgetType, tinyxml2::XMLElement* pElement);
	private:
		friend class Singleton<UIManager>;
		UIManager();

		void RegisterBuiltInWidgetFactories();

		Window m_Window;

		Callbacks m_Callbacks;

		std::unordered_map<std::string, WidgetFactoryFn> m_WidgetFactories;
	};

}