#include "Canvas.h"

RUIN::Canvas::Canvas(tinyxml2::XMLElement* element)
    : LeafNode(element)
    , m_X(FLT_MAX)
    , m_Y(FLT_MAX)
    , m_W(FLT_MAX)
    , m_H(FLT_MAX)
{
    Bind_member_to_XML(m_X, element, "posX");
    Bind_member_to_XML(m_Y, element, "posY");
    Bind_member_to_XML(m_W, element, "width");
    Bind_member_to_XML(m_H, element, "height");
}

void RUIN::Canvas::Render(const RenderArea& targetArea)
{
    LeafNode::Render(targetArea);

    if(targetArea.x != m_X)
    {
        m_X = targetArea.x;
        Notify_member_changed(m_X);
    }
    if(targetArea.y != m_Y)
    {
        m_Y = targetArea.y;
        Notify_member_changed(m_Y);
    }
    if(targetArea.x != m_W)
    {
        m_W = targetArea.w;
        Notify_member_changed(m_W);
    }
    if(targetArea.y != m_H)
    {
        m_H = targetArea.h;
        Notify_member_changed(m_H);
    }
}

Erm::vec2f RUIN::Canvas::GetDimensions()
{
    return { FLT_MAX * 0.5f, FLT_MAX * 0.5f };
}
