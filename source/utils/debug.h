#pragma once

#define RASSERT(condition, message) RUIN::AssertAndReport(condition, message)
#define RASSERTF(condition, message, ...) RUIN::AssertAndReport(condition, std::format(message, __VA_ARGS__).c_str())

namespace RUIN
{
	void AssertAndReport(bool isOkay, const char* errorMessage);
}