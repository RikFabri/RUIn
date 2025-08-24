#include "DataBinding.h"
#include "../UIManager.h"
#include <iostream>

RUIN::ChangeHandlerHelper::ChangeHandlerHelper(XMLBindingCreationHelper* pInitializer)
	: m_pInitializer(pInitializer)
{
}

void RUIN::ChangeHandlerHelper::Then(void(*action)(void*), void* initialInstance)
{
	action(initialInstance);

	if (!m_pInitializer->m_IsCurrentMemberBound)
	{
		return;
	}

	UIManager::GetInstance().GetBindingDatabase().AddChangeHandlerToBinding(m_pInitializer->m_CurrentBindingName, action);
}

void RUIN::XMLBindingCreationHelper::InitializeMethod(void(*method)(void*, const char*), tinyxml2::XMLElement* element, const char* attribute, void* instance)
{
	const auto* attrib = element->FindAttribute(attribute);
	if (!attrib)
		return;

	std::string val = attrib->Value();

	bool isBinding;
	const auto bindingName = ParseBinding(val, isBinding);

	if (isBinding)
	{
		std::cout << "binding found for method. Not implemented yet.";
	}
	else
	{
		method(instance, val.c_str());
	}
}

RUIN::ChangeHandlerHelper RUIN::XMLBindingCreationHelper::InitializeMember(size_t memberOffset, std::string& str, tinyxml2::XMLElement* element, const char* attribute, void* instance)
{
	const char* attribValue = element->Attribute(attribute);

	if (!attribValue && (strcmp(attribute, "text") == 0 || strcmp(attribute, "Text") == 0))
	{
		attribValue = element->GetText();
	}

	str = attribValue ? attribValue : "";


	bool isBinding;
	const auto bindingName = ParseBinding(str, isBinding);
	m_CurrentBindingName = bindingName;
	m_IsCurrentMemberBound = isBinding;

	if (isBinding)
	{
		AddBinding(instance, typeid(std::string).hash_code(), memberOffset, sizeof(std::string), bindingName, attribute, BindingDatabase::BindingType::STR);
	}

	return ChangeHandlerHelper{ this };
}

RUIN::ChangeHandlerHelper RUIN::XMLBindingCreationHelper::InitializeMember(size_t memberOffset, ClientBuffer& member, tinyxml2::XMLElement* element, const char* attribute, void* instance)
{
	InitializeMemberGeneric<ClientBuffer>(memberOffset, member, element, attribute, instance);
	return ChangeHandlerHelper{ this };
}

RUIN::ChangeHandlerHelper RUIN::XMLBindingCreationHelper::InitializeMember(size_t memberOffset, float& member, tinyxml2::XMLElement* element, const char* attribute, void* instance)
{
	InitializeMemberGeneric<float>(memberOffset, member, element, attribute, instance);
	return ChangeHandlerHelper{ this };
}

RUIN::ChangeHandlerHelper RUIN::XMLBindingCreationHelper::InitializeMember(size_t memberOffset, int& member, tinyxml2::XMLElement* element, const char* attribute, void* instance)
{
	InitializeMemberGeneric<int>(memberOffset, member, element, attribute, instance);
	return ChangeHandlerHelper{ this };
}

void RUIN::XMLBindingCreationHelper::AddBinding(void* pInstance, size_t typeHash, size_t memberOffset, size_t memberSize, const std::string& bindingName, const std::string& attribute, BindingDatabase::BindingType type)
{
	UIManager::GetInstance().GetBindingDatabase().CreateBinding(bindingName, pInstance, typeHash, memberOffset, memberSize, type);
}

std::string RUIN::XMLBindingCreationHelper::ParseBinding(std::string& value, bool& isBinding)
{
	const auto strLen = value.length();

	if (strLen > 1 && value[0] == '{' && value[strLen - 1] == '}')
	{
		isBinding = true;
		const auto bindingName = value.substr(1, strLen - 2);
		return bindingName;
	}

	isBinding = false;
	return value;
}
