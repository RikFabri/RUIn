#pragma once
#include <cstdint>

#ifdef RUIN_BUILD
#define API __declspec( dllexport )
#else
#define API __declspec( dllimport )
#endif

namespace RUIN
{
    struct Colour
    {
        uint32_t r;
        uint32_t g;
        uint32_t b;
        uint32_t a;
    };

    struct Rectangle
    { 
        uint32_t x;
        uint32_t y;
        uint32_t w;
        uint32_t h;
    };

    typedef void(*DrawRectangleFn)(Rectangle, Colour);
    typedef void(*DrawTextureFn)(Rectangle, void* texture);
    typedef void*(*AllocateTextureFromImageFn)(const char* path);
    typedef void*(*AllocateTextureFromTextFn)(const char* string);
    typedef void(*QueryTextureDimensions)(const void* texture, uint32_t* outWidth, uint32_t* outHeight);

    struct Callbacks
    {
        DrawRectangleFn drawRectangleFn;
        DrawTextureFn drawTextureFn;
        AllocateTextureFromImageFn allocateTextureFromImageFn;
        AllocateTextureFromTextFn allocateTextureFromTextFn;
        QueryTextureDimensions queryTextureDimensions;
    };


    API bool InitializeCallbacks(const Callbacks& callbacks);

    API void UpdateUI();
    API void RenderUI();

    API void LoadUIFromXML(const char* path);
}