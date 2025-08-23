#include "BindingDatabase.h"


void RUIN::BindingDatabase::CreateBinding(const std::string& bindingName, void* pInstance, size_t typeHash, size_t memberOffset)
{
	BindingData data
	{
		pInstance,
		typeHash,
		memberOffset
	};

	ReverseLookupKey key
	{
		pInstance,
		memberOffset
	};

	m_Bindings[bindingName] = data;
	m_BindingNames[key] = bindingName;
}

void RUIN::BindingDatabase::AddChangeHandlerToBinding(const std::string& bindingName, BindingChangeHandler func)
{
	RASSERT(m_Bindings.contains(bindingName), "Tried to add changehandler to missing binding!");

	m_Bindings.at(bindingName).changeHandler = func;
}

size_t RUIN::BindingDatabase::Hasher::operator()(const ReverseLookupKey& key) const noexcept
{
	auto h1 = std::hash<size_t>{}(key.offset);
	auto h2 = std::hash<void*>{}(key.pInstance);

	return h1 ^ (h2 << 1);
}

bool RUIN::BindingDatabase::ReverseLookupKey::operator==(const ReverseLookupKey& other) const
{
	return other.offset == offset && other.pInstance == pInstance;
}
