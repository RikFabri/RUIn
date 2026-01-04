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
typedef void(*RUIN_FreeTextureFn)(const void* texture);
typedef void(*RUIN_QueryTextureDimensionsFn)(const void* texture, uint32_t* outWidth, uint32_t* outHeight);
typedef void(*RUIN_SetClipRectangleFn)(RUIN_Rectangle*);

typedef struct RUIN_Callbacks
{
    RUIN_DrawRectangleFn drawRectangleFn;
    RUIN_DrawTextureFn drawTextureFn;
    RUIN_AllocateTextureFromImageFn allocateTextureFromImageFn;
    RUIN_AllocateTextureFromTextFn allocateTextureFromTextFn;
    RUIN_QueryTextureDimensionsFn queryTextureDimensions;
    RUIN_FreeTextureFn freeTexture;
    RUIN_SetClipRectangleFn setClipRectangle;
} RUIN_Callbacks;

typedef enum RUIN_Result
{
    RUIN_Fail = 0,
    RUIN_Success = 1
} RUIN_Result;

// (De)Initialization
API RUIN_Result RUIN_InitializeCallbacks(const RUIN_Callbacks* callbacks);
API void RUIN_Shutdown();

// Frame
API void RUIN_UpdateUI();
API void RUIN_RenderUI();

API RUIN_Result RUIN_LoadUIFromXML(const char* path);

API void RUIN_SetWindowDimensions(unsigned width, unsigned height);

// Event handling
API void RUIN_CursorMoved(int cursorX, int cursorY);
API void RUIN_CursorUp(int cursorX, int cursorY);
API void RUIN_CursorDown(int cursorX, int cursorY);
API void RUIN_ScrollWheel(float change, int cursorX, int cursorY);

// Data binding
API void RUIN_SetBindValuei(const char* bindingName, int value);
API void RUIN_SetBindValuef(const char* bindingName, float value);
API void RUIN_SetBindValues(const char* bindingName, const char* value);

// The second parameter indicates the widget's row number. This is useful when listening to a list of templated widgets.
API void RUIN_BindValuei(const char* bindingName, void(*onChanged)(int, int));
API void RUIN_BindValuef(const char* bindingName, void(*onChanged)(float, int));
API void RUIN_BindValues(const char* bindingName, void(*onChanged)(const char*, int));

API void RUIN_BindBuffer(const char* bindingName, const void* buffer, unsigned bufferSize);
// API void RUIN_BindBufferLayout(const char* bindingName, ) // TODO: we could allow explicit buffer layout bindings to add type-safety and increase compatibility with data sources.

API void RUIN_RegisterNamedCallback(const char* name, void(*func)(void));

// Returns string containing the most recent error reported.
API const char* RUIN_GetError();

#ifdef __cplusplus
}
#endif