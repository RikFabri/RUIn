#pragma once
#include "tinyxml2.h"

#include <unordered_map>
#include <string>

#define Create_widget_initializer(WidgetClassName) typedef WidgetClassName _WidgetType;
#define Bind_member_to_XML(member, xmlElement, attributeName) _DataBindingDescriptor.InitializeMember(offsetof(_WidgetType, member), member, xmlElement, attributeName)
#define Bind_method_to_XML(method, xmlElement, attributeName) _DataBindingDescriptor.InitializeMethod(&ConvertMemberToGenericFreeFunction<_WidgetType, &_WidgetType::##method>, xmlElement, attributeName, this)
#define OnChange(changeHandlerMemberFunction) Then(&ConvertMemberToGenericFreeFunction<_WidgetType, &_WidgetType::##changeHandlerMemberFunction>, this)


namespace RUIN
{
	template <typename T, void (T::* Member)()>
	void ConvertMemberToGenericFreeFunction(void* instance) 
	{
		(static_cast<T*>(instance)->*Member)();
	}

	template <typename T, void (T::* Member)(const char*)>
	void ConvertMemberToGenericFreeFunction(void* instance, const char* data)
	{
		(static_cast<T*>(instance)->*Member)(data);
	}

	class XMLBindingCreationHelper;

	// Returned by Bind_member_to_XML() to allow binding a changehandler
	class ChangeHandlerHelper
	{
	public:
		explicit ChangeHandlerHelper(XMLBindingCreationHelper* pInitializer);

		void Then(void(*action)(void*), void* initialInstance);

	private:
		XMLBindingCreationHelper* m_pInitializer;
		void(*func)(void*);
	};


	class XMLBindingCreationHelper
	{
	public:
		void InitializeMethod(void(*method)(void*, const char*), tinyxml2::XMLElement* element, const char* attribute, void* instance);

		ChangeHandlerHelper InitializeMember(size_t memberOffset, std::string& str, tinyxml2::XMLElement* element, const char* attribute);

		// Explicitly listing supported types so people don't need to sift through template errors.
		ChangeHandlerHelper InitializeMember(size_t memberOffset, float& member, tinyxml2::XMLElement* element, const char* attribute);
		ChangeHandlerHelper InitializeMember(size_t memberOffset, int& member, tinyxml2::XMLElement* element, const char* attribute);

	private:
		friend class ChangeHandlerHelper;

		template<typename MemberType>
		void InitializeMemberGeneric(size_t memberOffset, MemberType& member, tinyxml2::XMLElement* element, const char* attribute);

		std::string ParseBinding(std::string& value, bool& isBinding);

		std::unordered_map<std::string, size_t> m_OffsetsByName;
	};


	template<typename MemberType>
	inline void XMLBindingCreationHelper::InitializeMemberGeneric(size_t memberOffset, MemberType& member, tinyxml2::XMLElement* element, const char* attribute)
	{
		const auto* attrib = element->FindAttribute(attribute);
		if (!attrib)
			return;

		std::string val = attrib->Value();

		bool isBinding;
		const auto bindingName = ParseBinding(val, isBinding);

		if (isBinding)
		{
			m_OffsetsByName[bindingName] = memberOffset;
		}
		else
		{
			element->QueryAttribute(attribute, &member);
		}
	}
}