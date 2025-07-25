#include "DataBinding.h"

RUIN::ChangeHandlerHelper::ChangeHandlerHelper(XMLBindingCreationHelper* pInitializer)
	: m_pInitializer(pInitializer)
	, func(nullptr)
{
}

void RUIN::ChangeHandlerHelper::Then(void(*action)(void*), void* initialInstance)
{
	// TODO: should be in xml initializer
	func = action;
	action(initialInstance);
}

void RUIN::XMLBindingCreationHelper::InitializeMethod(void(*method)(void*, const char*), tinyxml2::XMLElement* element, const char* attribute, void* instance)
{
	// TODO: save method
	method(instance, element->Attribute(attribute));
}

RUIN::ChangeHandlerHelper RUIN::XMLBindingCreationHelper::InitializeMember(size_t memberOffset, std::string& str, tinyxml2::XMLElement* element, const char* attribute)
{
	const char* attribValue = element->Attribute(attribute);

	if (!attribValue && (strcmp(attribute, "text") == 0 || strcmp(attribute, "Text") == 0))
	{
		attribValue = element->GetText();
	}

	str = attribValue ? attribValue : "";


	bool isBinding;
	const auto bindingName = ParseBinding(str, isBinding);
	if (isBinding)
	{
		m_OffsetsByName[bindingName] = memberOffset;
	}

	return ChangeHandlerHelper{ this };
}

RUIN::ChangeHandlerHelper RUIN::XMLBindingCreationHelper::InitializeMember(size_t memberOffset, float& member, tinyxml2::XMLElement* element, const char* attribute)
{
	InitializeMemberGeneric<float>(memberOffset, member, element, attribute);
	return ChangeHandlerHelper{ this };
}

RUIN::ChangeHandlerHelper RUIN::XMLBindingCreationHelper::InitializeMember(size_t memberOffset, int& member, tinyxml2::XMLElement* element, const char* attribute)
{
	InitializeMemberGeneric<int>(memberOffset, member, element, attribute);
	return ChangeHandlerHelper{ this };
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
