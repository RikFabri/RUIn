#include "include/RUIN.h"
#include "UIManager.h"

using namespace RUIN;

RUIN_Result RUIN_InitializeCallbacks(const RUIN_Callbacks* callbacks)
{
    if (callbacks->allocateTextureFromImageFn == nullptr ||
        callbacks->allocateTextureFromTextFn == nullptr ||
        callbacks->drawRectangleFn == nullptr ||
        callbacks->drawTextureFn == nullptr ||
        callbacks->freeTexture == nullptr ||
        callbacks->setClipRectangle == nullptr)
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

void RUIN_SetWindowDimensions(unsigned width, unsigned height)
{
    UIManager::GetInstance().SetWindowSize(width, height);
}

void RUIN_CursorMoved(int cursorX, int cursorY)
{
    UIManager::GetInstance().OnCursorMoved(cursorX, cursorY);
}

void RUIN_CursorUp(int cursorX, int cursorY)
{
    UIManager::GetInstance().OnCursorUp(cursorX, cursorY);
}

void RUIN_CursorDown(int cursorX, int cursorY)
{
    UIManager::GetInstance().OnCursorDown(cursorX, cursorY);
}

void RUIN_SetBindValuei(const char* bindingName, int value)
{
    UIManager::GetInstance().GetBindingDatabase().SetDataOnBinding<int>(bindingName, value);
}

void RUIN_SetBindValuef(const char* bindingName, float value)
{
    UIManager::GetInstance().GetBindingDatabase().SetDataOnBinding<float>(bindingName, value);
}

void RUIN_SetBindValues(const char* bindingName, const char* value)
{
    UIManager::GetInstance().GetBindingDatabase().SetDataOnBinding<std::string>(bindingName, value);
}

void RUIN_BindValuei(const char* bindingName, void(*onChanged)(int, int))
{
    UIManager::GetInstance().GetBindingDatabase().ObserveBinding<int>(bindingName, onChanged);
}

void RUIN_BindValuef(const char* bindingName, void(*onChanged)(float, int))
{
    UIManager::GetInstance().GetBindingDatabase().ObserveBinding<float>(bindingName, onChanged);
}

void RUIN_BindValues(const char* bindingName, void(*onChanged)(const char*, int))
{
    UIManager::GetInstance().GetBindingDatabase().ObserveBinding<std::string>(bindingName, [onChanged](std::string str, int row) { onChanged(str.c_str(), row); });
}

void RUIN_BindBuffer(const char* bindingName, const void* buffer, unsigned bufferSize)
{
    ClientBuffer clientBuffer
    {
        bufferSize,
        buffer
    };

    UIManager::GetInstance().GetBindingDatabase().SetDataOnBinding<ClientBuffer>(bindingName, clientBuffer);
}

void RUIN_RegisterNamedCallback(const char* name, void(*func)(void))
{
    UIManager::GetInstance().RegisterNamedCallback(name, func);
}

const char* RUIN_GetError()
{
    return UIManager::GetInstance().GetLatestErrorMessage();
}
