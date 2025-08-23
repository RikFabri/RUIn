#include "BindingDatabase.h"


void RUIN::BindingDatabase::CreateBinding(const std::string& bindingName, void* pInstance, size_t typeHash, size_t memberOffset)
{
	BindingData data
	{
		pInstance,
		typeHash,
		memberOffset
	};


	m_Bindings[bindingName] = data;
}

void RUIN::BindingDatabase::AddChangeHandlerToBinding(const std::string& bindingName, BindingChangeHandler func)
{
	RASSERT(m_Bindings.contains(bindingName), "Tried to add changehandler to missing binding!");

	m_Bindings.at(bindingName).changeHandler = func;
}
