#pragma once

#define RASSERT(condition, message) RUIN::AssertAndReport(condition, message)

namespace RUIN
{
	void AssertAndReport(bool isOkay, const char* errorMessage);
}