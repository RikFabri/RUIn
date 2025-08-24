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

size_t RUIN::BindingDatabase::SetDataOnBinding(const std::string& bindingName, void* pData, unsigned bindingContextId, bool rawInputStrings)
{
	BindingIdentifier bindingId{ bindingName, bindingContextId };
	RASSERT(m_Bindings.contains(bindingId), "Could not find matching binding!");

	const auto& bindingData = m_Bindings.at(bindingId);

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


	if (bindingData.changeHandler)
	{
		bindingData.changeHandler(bindingData.pInstance);
	}

	return dataRead;
}

size_t RUIN::BindingDatabase::PatchWidgetDataFromBuffer(void* buffer, int bufferSize, void* instance, unsigned bindingContextId)
{
	WidgetId bindingContextKey{ instance, bindingContextId };

	if (!m_WidgetBindingNames.contains(bindingContextKey))
	{
		return 0;
	}

	const auto& bindingNames = m_WidgetBindingNames.at(bindingContextKey);

	size_t dataRead = 0;

	RASSERT(bindingNames.m_BindingNames.size() <= 1, "Running into untested feature!");
	
	for (const auto& bindingName : bindingNames.m_BindingNames)
	{
		BindingIdentifier bindingId{ bindingName, bindingContextId };

		auto* pBuffer = (char*)buffer + dataRead;
		dataRead += SetDataOnBinding(bindingName, pBuffer, bindingContextId, true);

		RASSERT(dataRead <= bufferSize, std::format("Not enough data in bound buffer! While binding {}", bindingName).c_str());
	}

	return dataRead;
}

void RUIN::BindingDatabase::CreateBinding(const std::string& bindingName, void* pInstance, size_t typeHash, size_t memberOffset, size_t memberSize, BindingType type)
{
	BindingData data
	{
		type,
		typeHash,
		memberOffset,
		memberSize,
		pInstance
	};

	ReverseLookupKey key
	{
		pInstance,
		memberOffset
	};

	WidgetId contextKey
	{
		pInstance,
		m_CurrentContext
	};

	BindingIdentifier bindingKey
	{
		bindingName,
		m_CurrentContext
	};

	m_Bindings[bindingKey] = data;
	m_BindingNames[key] = bindingKey;
	m_WidgetBindingNames[contextKey].m_BindingNames.push_back(bindingName);
}

void RUIN::BindingDatabase::AddChangeHandlerToBinding(const std::string& bindingName, BindingChangeHandler func)
{
	const auto id = GetCurrentBindingIdentifier(bindingName);
	RASSERT(m_Bindings.contains(id), "Tried to add changehandler to missing binding!");

	m_Bindings.at(id).changeHandler = func;
}

RUIN::BindingDatabase::BindingIdentifier RUIN::BindingDatabase::GetCurrentBindingIdentifier(const std::string& bindingName) const
{
	return BindingIdentifier(bindingName, m_CurrentContext);
}

size_t RUIN::BindingDatabase::ReverseLookupKeyHasher::operator()(const ReverseLookupKey& key) const noexcept
{
	auto h1 = std::hash<size_t>{}(key.offset);
	auto h2 = std::hash<void*>{}(key.pInstance);

	return h1 ^ (h2 << 1);
}

bool RUIN::BindingDatabase::ReverseLookupKey::operator==(const ReverseLookupKey& other) const
{
	return other.offset == offset && other.pInstance == pInstance;
}

//bool RUIN::BindingDatabase::TypeBindingDescriptor::NamedOffset::operator==(const NamedOffset& other)
//{
//	return other.m_MemberName == m_MemberName && other.m_MemberOffset == m_MemberOffset;
//}

bool RUIN::BindingDatabase::WidgetId::operator==(const WidgetId& other) const
{
	return other.bindingContextId == bindingContextId && other.pInstance == pInstance;
}

size_t RUIN::BindingDatabase::BindingContextLookupKeyHasher::operator()(const WidgetId& key) const noexcept
{
	auto h1 = std::hash<unsigned>{}(key.bindingContextId);
	auto h2 = std::hash<void*>{}(key.pInstance);

	return h1 ^ (h2 << 1);
}

bool RUIN::BindingDatabase::BindingIdentifier::operator==(const BindingIdentifier& other) const
{
	return other.bindingContextId == bindingContextId && other.bindingName == bindingName;
}

size_t RUIN::BindingDatabase::BindingIdentifierHasher::operator()(const BindingIdentifier& key) const noexcept
{
	auto h1 = std::hash<unsigned>{}(key.bindingContextId);
	auto h2 = std::hash<std::string>{}(key.bindingName);

	return h1 ^ (h2 << 1);
}
