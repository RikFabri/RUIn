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
	class ClientTexture
	{
	public:
		~ClientTexture();
		ClientTexture();
		ClientTexture(void* pClientData);
		ClientTexture(ClientTexture&& other) noexcept;
		ClientTexture(const ClientTexture& other) = delete;
		ClientTexture& operator=(ClientTexture&& other) noexcept;

		void* GetClientData() const;
		void GetDimensions(uint32_t& width, uint32_t& height) const;
	private:

		void* m_pClientData;
	};

	class UIManager final : public Singleton<UIManager>
	{
	public:
		bool LoadXML(const std::string& path);

		void Update();
		void Render();

		void SetCallbacks(const RUIN_Callbacks& cb);
		void DrawRectangle(const RenderArea& ra) const;
		ClientTexture AllocateTextureFromText(const std::string& text);
		void FreeTexture(void* texture);
		void QueryTextureDimensions(const ClientTexture& texture, uint32_t& width, uint32_t& height) const;
		void DrawTexture(const ClientTexture& texture, const RenderArea& ra) const;


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

		RUIN_Callbacks m_Callbacks;

		std::unordered_map<std::string, WidgetFactoryFn> m_WidgetFactories;

		std::string m_LatestErrorMessage;

		unsigned m_NumTextureAllocations = 0;
		unsigned m_NumTextureFrees = 0;
	};

}