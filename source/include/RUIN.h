#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RUIN_BUILD
#define API __declspec( dllexport )
#else
#define API __declspec( dllimport )
#endif

typedef struct RUIN_Colour
{
    uint32_t r;
    uint32_t g;
    uint32_t b;
    uint32_t a;
} RUIN_Colour;

typedef struct RUIN_Rectangle
{ 
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
} RUIN_Rectangle;

typedef void(*RUIN_DrawRectangleFn)(RUIN_Rectangle, RUIN_Colour);
typedef void(*RUIN_DrawTextureFn)(RUIN_Rectangle, void* texture);
typedef void*(*RUIN_AllocateTextureFromImageFn)(const char* path);
typedef void*(*RUIN_AllocateTextureFromTextFn)(const char* string);
typedef void(*RUIN_FreeTexture)(const void* texture);
typedef void(*RUIN_QueryTextureDimensions)(const void* texture, uint32_t* outWidth, uint32_t* outHeight);

typedef struct RUIN_Callbacks
{
    RUIN_DrawRectangleFn drawRectangleFn;
    RUIN_DrawTextureFn drawTextureFn;
    RUIN_AllocateTextureFromImageFn allocateTextureFromImageFn;
    RUIN_AllocateTextureFromTextFn allocateTextureFromTextFn;
    RUIN_QueryTextureDimensions queryTextureDimensions;
    RUIN_FreeTexture freeTexture;
} RUIN_Callbacks;

typedef enum RUIN_Result
{
    RUIN_Fail = 0,
    RUIN_Success = 1
} RUIN_Result;

API RUIN_Result RUIN_InitializeCallbacks(const RUIN_Callbacks* callbacks);
API void RUIN_Shutdown();

API void RUIN_UpdateUI();
API void RUIN_RenderUI();

API RUIN_Result RUIN_LoadUIFromXML(const char* path);

API void RUIN_CursorMoved(int cursorX, int cursorY);

// Returns string containing the most recent error reported.
API const char* RUIN_GetError();

#ifdef __cplusplus
}
#endif