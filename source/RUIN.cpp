#include "include/RUIN.h"
#include <iostream>
#include "UIManager.h"

void RUIN::SetDrawRectangleCallback(DrawRectangleCb cb)
{
    UIManager::GetInstance().SetDrawRectangleCb(cb);
}

void RUIN::UpdateUI()
{
    UIManager::GetInstance().Update();
}

void RUIN::RenderUI()
{
    UIManager::GetInstance().Render();
}
