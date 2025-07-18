#pragma once

#ifdef RUIN_BUILD
#define API __declspec( dllexport )
#else
#define API __declspec( dllimport )
#endif

namespace RUIN
{
    typedef void(*DrawRectangleCb)(int, int, int, int);

    API void SetDrawRectangleCallback(DrawRectangleCb cb);

    API void UpdateUI();
    API void RenderUI();

    API void LoadUIFromXML(const char* path);
}