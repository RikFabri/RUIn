#pragma once
#include <cstdint>


typedef uint32_t RUIN_TEXTURE_ID;

typedef struct RUIN_RECTANGLE
{
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
} RUIN_RECTANGLE;

typedef struct RUIN_COLOUR
{
	uint32_t r;
	uint32_t g;
	uint32_t b;
	uint32_t a;
} RUIN_COLOUR;

typedef enum RUIN_IntegrationCommandType
{
	RUIN_DRAW_RECTANGLE,
	RUIN_DRAW_TEXTURE,
	RUIN_ALLOCATE_TEXTURE_FROM_IMAGE,
	RUIN_ALLOCATE_TEXTURE_FROM_TEXT

} RUIN_IntegrationCommandType;

typedef struct RUIN_ALLOCATE_TEXTURE_FROM_TEXT_DATA
{
	const char* string; // I don't want the user to be responsible for deallocating these. Can we pool these together and free them once a newer command is handled? Maybe we just notify RUIN when an event is handled?
	RUIN_TEXTURE_ID textureId;

} RUIN_ALLOCATE_TEXTURE_FROM_TEXT_DATA;

typedef struct RUIN_ALLOCATE_TEXTURE_FROM_IMAGE_DATA
{
	const char* path;
	RUIN_TEXTURE_ID textureId;

} RUIN_ALLOCATE_TEXTURE_FROM_IMAGE_DATA;

typedef struct RUIN_DRAW_RECTANGLE_DATA
{
	RUIN_RECTANGLE rectangle;
	RUIN_COLOUR colour;

} RUIN_DRAW_RECTANGLE_DATA;

typedef struct RUIN_DRAW_TEXTURE_DATA
{
	RUIN_RECTANGLE rectangle;
	RUIN_TEXTURE_ID textureId;

} RUIN_DRAW_TEXTURE_DATA;

typedef struct RUIN_IntegrationCommand
{
	uint32_t type; // RUIN_IntegrationCommandType

	union {
		RUIN_DRAW_RECTANGLE_DATA DrawRectangle;
		RUIN_DRAW_TEXTURE_DATA DrawTexture;
		RUIN_ALLOCATE_TEXTURE_FROM_IMAGE_DATA AllocateTextureFromImage;
		RUIN_ALLOCATE_TEXTURE_FROM_TEXT_DATA AllocateTextureFromText;
	};

} RUIN_IntegrationCommand;
