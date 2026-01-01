#pragma once
#include "Singleton.h"
#include "widgets/Window.h"
#include "include/RUIN.h"
#include "include/Commands.h"
#include "resources/ClientData.h"

#include "BindingDatabase.h"

#include <string>
#include <unordered_map>
#include <functional>
#include <optional>

namespace RUIN
{
	class UIManager final : public Singleton<UIManager>
	{
	public:
		bool LoadXML(const std::string& path);

		void Update();
		void Render();

		void SetWindowSize(unsigned width, unsigned height);

		// Input events
		void OnCursorMoved(int cursorX, int cursorY);
		void OnCursorDown(int cursorX, int cursorY);
		void OnCursorUp(int cursorX, int cursorY);

		void RegisterNamedCallback(const std::string& name, std::function<void()> func);
		void InvokeNamedCallback(const std::string& name) const;

		BindingDatabase& GetBindingDatabase();

		void SetCallbacks(const RUIN_Callbacks& cb);
		void DrawRectangle(const RenderArea& ra, RUIN_Colour colour) const;
		ClientTexture AllocateTextureFromText(const std::string& text);
		void FreeTexture(void* texture);
		void QueryTextureDimensions(const ClientTexture& texture, uint32_t& width, uint32_t& height) const;
		void DrawTexture(const ClientTexture& texture, const RenderArea& ra) const;
		void SetClipRect(const std::optional<RenderArea>& ra) const;


		using WidgetFactoryFn = std::function<IRenderable*(tinyxml2::XMLElement*)>;
		void RegisterWidgetFactory(const std::string& widgetType, const WidgetFactoryFn& factoryFunc);
		IRenderable* CreateWidgetFromType(const std::string& widgetType, tinyxml2::XMLElement* pElement);

		void SetErrorMessage(std::string message);
		const char* GetLatestErrorMessage() const;

		void ShutDown();
	private:
		friend class Singleton<UIManager>;
		UIManager();
		~UIManager();

		void RegisterBuiltInWidgetFactories();

		Window m_Window;

		BindingDatabase m_BindingDatabase;

		RUIN_Callbacks m_Callbacks;

		std::unordered_map<std::string, WidgetFactoryFn> m_WidgetFactories;
		std::unordered_map<std::string, std::function<void()>> m_NamedUserCallbacks;

		std::string m_LatestErrorMessage;

		unsigned m_NumTextureAllocations = 0;
		unsigned m_NumTextureFrees = 0;

		bool m_ShutdownCalled = false;
	};

}