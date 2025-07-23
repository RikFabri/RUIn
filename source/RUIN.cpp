#include "include/RUIN.h"
#include <iostream>
#include "UIManager.h"


bool RUIN::InitializeCallbacks(const Callbacks& callbacks)
{
    if (callbacks.allocateTextureFromImageFn == nullptr ||
        callbacks.allocateTextureFromTextFn == nullptr ||
        callbacks.drawRectangleFn == nullptr ||
        callbacks.drawTextureFn == nullptr)
    {
        UIManager::GetInstance().SetErrorMessage("Not all callbacks were provided!");
        return false;
    }

    UIManager::GetInstance().SetCallbacks(callbacks);

    return true;
}

void RUIN::UpdateUI()
{
    UIManager::GetInstance().Update();
}

void RUIN::RenderUI()
{
    UIManager::GetInstance().Render();
}

void RUIN::LoadUIFromXML(const char* path)
{
    UIManager::GetInstance().LoadXML(path);
}

const char* RUIN::GetError()
{
    return UIManager::GetInstance().GetLatestErrorMessage();
}
