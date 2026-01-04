#include "assert.h"
#include "../UIManager.h"

#include <cassert>
#include "debug.h"

void RUIN::AssertAndReport(bool isOkay, const char* errorMessage)
{
	if (isOkay)
	{
		return;
	}

	UIManager::GetInstance().SetErrorMessage(errorMessage);
	std::cerr << errorMessage << std::endl;
	assert(false);
}