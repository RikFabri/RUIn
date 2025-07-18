#include "UIManager.h"
#include <cassert>
#include "widgets/HorizontalBox.h"
#include "widgets/VerticalBox.h"
#include "widgets/Label.h"

RUIN::UIManager::UIManager()
	: m_Window( 200, 100 )
	, m_DrawRectangle{ nullptr }
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

void RUIN::UIManager::LoadXML(const std::string& path)
{
	using namespace tinyxml2;
	XMLDocument doc;
	const auto result = doc.LoadFile(path.c_str());

	assert(result == XML_SUCCESS);

	auto* pRoot = doc.RootElement();
	m_Window.AddChildWidget(pRoot);
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

void RUIN::UIManager::SetDrawRectangleCb(DrawRectangleCb cb)
{
	m_DrawRectangle = cb;
}

void RUIN::UIManager::DrawRectangle(const RenderArea& ra) const
{
	m_DrawRectangle(ra.x, ra.y, ra.w, ra.h);
}

void RUIN::UIManager::RegisterWidgetFactory(const std::string& widgetType, const WidgetFactoryFn& factoryFunc)
{
	m_WidgetFactories[widgetType] = factoryFunc;
}

IRenderable* RUIN::UIManager::CreateWidgetFromType(const std::string& widgetType, tinyxml2::XMLElement* pElement)
{
	assert(m_WidgetFactories.contains(widgetType));

	return m_WidgetFactories[widgetType](pElement);
}
