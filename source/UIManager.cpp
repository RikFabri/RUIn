#include "UIManager.h"
#include "debug.h"
#include "widgets/HorizontalBox.h"
#include "widgets/VerticalBox.h"
#include "widgets/Label.h"

RUIN::UIManager::UIManager()
	: m_Window( 800, 600 )
	, m_LatestErrorMessage()
{
	RegisterBuiltInWidgetFactories(); 
}

void RUIN::UIManager::RegisterBuiltInWidgetFactories()
{
	// TODO: This requires including all widgets in the manager, we should move these into a convenient macro in the widgets themselves

	m_WidgetFactories["HorizontalBox"] = [](tinyxml2::XMLElement* pElement) { return new HorizontalBox(pElement); };
	m_WidgetFactories["VerticalBox"] = [](tinyxml2::XMLElement* pElement) { return new VerticalBox(pElement); };
	m_WidgetFactories["Label"] = [](tinyxml2::XMLElement* pElement) { return new Label(pElement); };
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

void RUIN::UIManager::SetCallbacks(const Callbacks& cb)
{
	m_Callbacks = cb;
}

void RUIN::UIManager::DrawRectangle(const RenderArea& ra) const
{
	m_Callbacks.drawRectangleFn(ra.GetRect(), {125, 125, 0, 255});
}

void* RUIN::UIManager::AllocateTextureFromText(const std::string& text) const
{
	return m_Callbacks.allocateTextureFromTextFn(text.c_str());
}

void RUIN::UIManager::QueryTextureDimensions(const void* texture, uint32_t& width, uint32_t& height) const
{
	m_Callbacks.queryTextureDimensions(texture, &width, &height);
}

void RUIN::UIManager::DrawTexture(void* texture, const RenderArea& ra) const
{
	m_Callbacks.drawTextureFn(ra.GetRect(), texture);
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
