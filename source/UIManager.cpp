#include "UIManager.h"
#include "debug.h"
#include "widgets/HorizontalBox.h"
#include "widgets/VerticalBox.h"
#include "widgets/Label.h"
#include "widgets/Button.h"
#include "widgets/Slider.h"

#include <format>

RUIN::UIManager::UIManager()
	: m_Window( 800, 600 )
	, m_LatestErrorMessage()
{
	RegisterBuiltInWidgetFactories(); 
}

RUIN::UIManager::~UIManager()
{
	RASSERT(m_ShutdownCalled, "Application closed without calling RUIN_Shutdown()");
	RASSERT(m_NumTextureAllocations == m_NumTextureFrees, "Not all allocated textures were freed.");
}

void RUIN::UIManager::RegisterBuiltInWidgetFactories()
{
	// TODO: This requires including all widgets in the manager, we should move these into a convenient macro in the widgets themselves

	m_WidgetFactories["HorizontalBox"] = [](tinyxml2::XMLElement* pElement) { return new HorizontalBox(pElement); };
	m_WidgetFactories["VerticalBox"] = [](tinyxml2::XMLElement* pElement) { return new VerticalBox(pElement); };
	m_WidgetFactories["Label"] = [](tinyxml2::XMLElement* pElement) { return new Label(pElement); };
	m_WidgetFactories["Button"] = [](tinyxml2::XMLElement* pElement) { return new Button(pElement); };
	m_WidgetFactories["Slider"] = [](tinyxml2::XMLElement* pElement) { return new Slider(pElement); };
}

bool RUIN::UIManager::LoadXML(const std::string& path)
{
	using namespace tinyxml2;
	XMLDocument doc;
	const auto result = doc.LoadFile(path.c_str());

	if (result != XML_SUCCESS)
	{
		UIManager::GetInstance().SetErrorMessage("Failed to load xml file");
		return false;
	}

	auto* pRoot = doc.RootElement();
	for (auto* e = pRoot->FirstChildElement(); e; e = e->NextSiblingElement())
	{
		m_Window.AddChildWidget(e);
	}

	return true;
}

void RUIN::UIManager::Update()
{
	RenderArea ra{};
	m_Window.CalculateUsedContentArea(ra);
}

void RUIN::UIManager::Render()
{
	RenderArea ra{};
	m_Window.Render(ra);
}

void RUIN::UIManager::SetWindowSize(unsigned width, unsigned height)
{
	m_Window.SetDimensions(width, height);
}

void RUIN::UIManager::OnCursorMoved(int cursorX, int cursorY)
{
	m_Window.HandleMouseMoved(cursorX, cursorY);
}

void RUIN::UIManager::OnCursorDown(int cursorX, int cursorY)
{
	m_Window.HandleMouseDown(cursorX, cursorY);
}

void RUIN::UIManager::OnCursorUp(int cursorX, int cursorY)
{
	m_Window.HandleMouseUp(cursorX, cursorY);
}

void RUIN::UIManager::RegisterNamedCallback(const std::string& name, std::function<void()> func)
{
	m_NamedUserCallbacks[name] = func;
}

void RUIN::UIManager::InvokeNamedCallback(const std::string& name) const
{
	RASSERT(m_NamedUserCallbacks.contains(name), std::format("Named callback {} wasn't registered!", name).c_str());

	m_NamedUserCallbacks.at(name)();
}

RUIN::BindingDatabase& RUIN::UIManager::GetBindingDatabase()
{
	return m_BindingDatabase;
}

void RUIN::UIManager::SetCallbacks(const RUIN_Callbacks& cb)
{
	m_Callbacks = cb;
}

void RUIN::UIManager::DrawRectangle(const RenderArea& ra, RUIN_Colour colour) const
{
	m_Callbacks.drawRectangleFn(ra.GetRect(), colour);
}

RUIN::ClientTexture RUIN::UIManager::AllocateTextureFromText(const std::string& text)
{
	++m_NumTextureAllocations;
	return ClientTexture{ m_Callbacks.allocateTextureFromTextFn(text.c_str()) };
}

void RUIN::UIManager::FreeTexture(void* texture)
{
	++m_NumTextureFrees;
	m_Callbacks.freeTexture(texture);
}

void RUIN::UIManager::QueryTextureDimensions(const ClientTexture& texture, uint32_t& width, uint32_t& height) const
{
	m_Callbacks.queryTextureDimensions(texture.GetClientData(), &width, &height);
}

void RUIN::UIManager::DrawTexture(const ClientTexture& texture, const RenderArea& ra) const
{
	m_Callbacks.drawTextureFn(ra.GetRect(), texture.GetClientData());
}

void RUIN::UIManager::RegisterWidgetFactory(const std::string& widgetType, const WidgetFactoryFn& factoryFunc)
{
	m_WidgetFactories[widgetType] = factoryFunc;
}

RUIN::IRenderable* RUIN::UIManager::CreateWidgetFromType(const std::string& widgetType, tinyxml2::XMLElement* pElement)
{
	RASSERT(m_WidgetFactories.contains(widgetType), "Widget type did not correspond to factory.");

	return m_WidgetFactories[widgetType](pElement);
}

void RUIN::UIManager::SetErrorMessage(std::string message)
{
	m_LatestErrorMessage = std::move(message);
}

const char* RUIN::UIManager::GetLatestErrorMessage() const
{
	return m_LatestErrorMessage.c_str();
}

void RUIN::UIManager::ShutDown()
{
	m_ShutdownCalled = true;
	m_Window.ClearWidgets();
}