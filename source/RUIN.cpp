#include "include/RUIN.h"
#include <iostream>
#include "UIManager.h"

using namespace RUIN;

RUIN_Result RUIN_InitializeCallbacks(const RUIN_Callbacks* callbacks)
{
    if (callbacks->allocateTextureFromImageFn == nullptr ||
        callbacks->allocateTextureFromTextFn == nullptr ||
        callbacks->drawRectangleFn == nullptr ||
        callbacks->drawTextureFn == nullptr ||
        callbacks->freeTexture == nullptr)
    {
        UIManager::GetInstance().SetErrorMessage("Not all callbacks were provided!");
        return RUIN_Fail;
    }

    UIManager::GetInstance().SetCallbacks(*callbacks);

    return RUIN_Success;
}

void RUIN_Shutdown()
{
    return UIManager::GetInstance().ShutDown();
}

void RUIN_UpdateUI()
{
    UIManager::GetInstance().Update();
}

void RUIN_RenderUI()
{
    UIManager::GetInstance().Render();
}

RUIN_Result RUIN_LoadUIFromXML(const char* path)
{
    return UIManager::GetInstance().LoadXML(path) ? RUIN_Success : RUIN_Fail;
}

const char* RUIN_GetError()
{
    return UIManager::GetInstance().GetLatestErrorMessage();
}
