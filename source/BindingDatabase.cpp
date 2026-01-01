#include "BindingDatabase.h"
#include <algorithm>
#include <format>


unsigned RUIN::BindingDatabase::PushNewContext()
{
	++m_NumContexts;
	m_CurrentContext = m_NumContexts;

	return m_CurrentContext;
}

void RUIN::BindingDatabase::PopContext()
{
	RASSERT(m_CurrentContext != 0, "BindingDatabase mismatching number of PushContext PopContext!");
	--m_CurrentContext;
}

size_t RUIN::BindingDatabase::SetDataOnBindingInternal(const BindingData& bindingData, const void* pData, bool rawInputStrings)
{
	size_t dataRead = bindingData.typeSize;

	char* basePtr = static_cast<char*>(bindingData.pInstance);

	if (bindingData.type == BindingType::POD)
	{
		auto* pMember = (basePtr + bindingData.offset);

		std::memcpy(pMember, pData, bindingData.typeSize);
	}
	else if (bindingData.type == BindingType::STR)
	{
		RASSERT(rawInputStrings, "We don't support std::string when dealing with an immutable buffer. The generic code is always dealing with copies, so modern strings should be movable");
		auto* pMember = reinterpret_cast<std::string*>(basePtr + bindingData.offset);

		auto* string = static_cast<const char*>(pData);
		dataRead = strlen(string) + 1;

		*pMember = string;
	}
	else if (bindingData.type == BindingType::FUNCTION) // This field does not have a backing member
	{
		auto* func = reinterpret_cast<void(*)(void*, const char*)>(bindingData.offset);

		if (rawInputStrings)
		{
			func(bindingData.pInstance, (const char*)pData);
		}
		else
		{
			auto* string = static_cast<const std::string*>(pData);
			func(bindingData.pInstance, string->c_str());
		}
	}


	if (bindingData.changeHandler)
	{
		bindingData.changeHandler(bindingData.pInstance);
	}

	return dataRead;
}

size_t RUIN::BindingDatabase::SetDataOnBindingInternal(const BindingData& bindingData, void* pData, bool rawInputStrings)
{
	const bool isModernString = bindingData.type == BindingType::STR && !rawInputStrings;

	if (!isModernString)
	{
		return SetDataOnBindingInternal(bindingData, (const void*)pData, rawInputStrings);
	}

	char* basePtr = static_cast<char*>(bindingData.pInstance);
	auto* pMember = reinterpret_cast<std::string*>(basePtr + bindingData.offset);

	auto* string = static_cast<std::string*>(pData);
	size_t dataRead = string->length() + 1;

	// This move looks evil, but if we have a string that's not raw, then the generic type implementation has copied it to create this buffer
	// So it would be wasteful to copy again.
	// Immutable buffers will always be raw strings, as they cross dll boundaries.
	// TODO: can this be handled more explicitly by overloading const void* vs void*, when patching a value vs a buffer?
	*pMember = std::move(*string);

	if (bindingData.changeHandler)
	{
		bindingData.changeHandler(bindingData.pInstance);
	}

	return dataRead;
}

size_t RUIN::BindingDatabase::PatchWidgetDataFromBuffer(const void* buffer, int bufferSize, void* instance)
{
	const auto bindings = m_Bindings.QueryRow<QueryByWidget>(instance);

	if (bindings.empty())
	{
		return 0;
	}

	size_t dataRead = 0;

	RASSERT(bindings.size() <= 1, "Running into untested feature!");
	
	for (const auto* binding : bindings)
	{
		const auto* pBuffer = (const char*)buffer + dataRead;
		dataRead += SetDataOnBindingInternal(*binding, pBuffer, true);

		RASSERT(dataRead <= bufferSize, std::format("Not enough data in bound buffer! While binding {}", binding->bindingName).c_str());
	}

	return dataRead;
}

void RUIN::BindingDatabase::CreateBinding(const std::string& bindingName, void* pInstance, size_t typeHash, size_t memberOffset, size_t memberSize, BindingType type)
{
	BindingData data;
	data.type = type;
	data.typeHash = typeHash;
	data.offset = memberOffset;
	data.typeSize = memberSize;
	data.bindingContextId = m_CurrentContext;
	data.pInstance = pInstance;
	data.bindingName = bindingName;

	static_assert(sizeof(BindingData) == 184, "Did you modify the struct? Don't forget to assign the new value!");

	m_Bindings.Insert(std::move(data));
}

void RUIN::BindingDatabase::AddChangeHandlerToBinding(const std::string& bindingName, BindingChangeHandler func)
{
	const auto bindings = m_Bindings.QueryRow<QueryByNameAndContext>({ bindingName, m_CurrentContext });

	RASSERT(bindings.size() == 1, "Tried to add changehandler to missing binding!");

	bindings[0]->changeHandler = func;
}