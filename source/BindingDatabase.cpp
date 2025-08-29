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

size_t RUIN::BindingDatabase::SetDataOnBindingInternal(const BindingData& bindingData, void* pData, unsigned bindingContextId, bool rawInputStrings)
{
	size_t dataRead = bindingData.typeSize;

	char* basePtr = static_cast<char*>(bindingData.pInstance);

	if (bindingData.type == BindingType::POD)
	{
		auto* pMember = (basePtr + bindingData.offset);

		std::memcpy(pMember, pData, bindingData.typeSize);
	}
	else if (bindingData.type == BindingType::STR && rawInputStrings)
	{
		auto* pMember = reinterpret_cast<std::string*>(basePtr + bindingData.offset);

		auto* string = static_cast<const char*>(pData);
		dataRead = strlen(string) + 1;

		*pMember = string;
	}
	else if (bindingData.type == BindingType::STR && !rawInputStrings)
	{
		auto* pMember = reinterpret_cast<std::string*>(basePtr + bindingData.offset);

		auto* string = static_cast<std::string*>(pData);
		dataRead = string->length() + 1;

		*pMember = std::move(*string);
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
			auto* string = static_cast<std::string*>(pData);
			func(bindingData.pInstance, string->c_str());
		}
	}


	if (bindingData.changeHandler)
	{
		bindingData.changeHandler(bindingData.pInstance);
	}

	return dataRead;
}

size_t RUIN::BindingDatabase::PatchWidgetDataFromBuffer(void* buffer, int bufferSize, void* instance, unsigned bindingContextId)
{
	const auto bindings = m_Bindings.QueryRow<QueryByWidget>({ instance, bindingContextId });

	if (bindings.empty())
	{
		return 0;
	}

	size_t dataRead = 0;

	RASSERT(bindings.size() <= 1, "Running into untested feature!");
	
	for (const auto* binding : bindings)
	{
		auto* pBuffer = (char*)buffer + dataRead;
		dataRead += SetDataOnBindingInternal(*binding, pBuffer, bindingContextId, true);

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