#include "utils.h"
#include "debug.h"

#include <string>
#include <format>

RUIN_Colour RUIN::utils::ColourFromHexString(const char* str)
{
	RASSERTF(str[0] == '#', "{} was not a valid hex colour, did not start with #", str);
	RASSERTF(strlen(str) == 9, "{} was not a valid hex colour, it was not 9 characters", str);

	constexpr auto HexToVal = [](char c) -> uint8_t
		{
			if (c >= '0' && c <= '9')
			{
				const auto res = c - '0'; (void)res;
				return c - '0';
			}

			c = (char)std::toupper(c);

			if (c >= 'A' && c <= 'F')
			{
				return 10 + c - 'A';
			}

			RASSERT(false, "%c is not a valid hex character");
			return 0;
		};

	RUIN_Colour col;

	uint8_t val = 0;
	val += HexToVal(str[1]) * 16;
	val += HexToVal(str[2]);

	col.r = val;

	val = 0;
	val += HexToVal(str[3]) * 16;
	val += HexToVal(str[4]);

	col.g = val;

	val = 0;
	val += HexToVal(str[5]) * 16;
	val += HexToVal(str[6]);

	col.b = val;

	val = 0;
	val += HexToVal(str[7]) * 16;
	val += HexToVal(str[8]);

	col.a = val;

	return col;
}

RUIN_Colour RUIN::utils::GetNextColour()
{
	static int idx = 0;

	static const RUIN_Colour cols[] =
	{
		{255,   0,   0, 255}, // Red
		{255, 127,   0, 255}, // Orange
		{255, 255,   0, 255}, // Yellow
		{  0, 255,   0, 255}, // Green
		{  0,   0, 255, 255}, // Blue
		{ 75,   0, 130, 255}, // Indigo
		{148,   0, 211, 255}  // Violet
	};


	RUIN_Colour res = cols[idx];
	idx = (idx + 1) % (sizeof(cols) / sizeof(RUIN_Colour));

	return res;
}
