#pragma once
#include "tinyxml2.h"

#include <unordered_map>
#include <string>

#define Create_widget_initializer(WidgetClassName) typedef WidgetClassName _WidgetType;
#define Bind_member_to_XML(member, xmlElement, attributeName) _DataBindingDescriptor.InitializeMember(offsetof(_WidgetType, member), member, xmlElement, attributeName, this)
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
	};


	class XMLBindingCreationHelper
	{
	public:
		void InitializeMethod(void(*method)(void*, const char*), tinyxml2::XMLElement* element, const char* attribute, void* instance);

		ChangeHandlerHelper InitializeMember(size_t memberOffset, std::string& str, tinyxml2::XMLElement* element, const char* attribute, void* instance);

		// Explicitly listing supported types so people don't need to sift through template errors.
		ChangeHandlerHelper InitializeMember(size_t memberOffset, float& member, tinyxml2::XMLElement* element, const char* attribute, void* instance);
		ChangeHandlerHelper InitializeMember(size_t memberOffset, int& member, tinyxml2::XMLElement* element, const char* attribute, void* instance);

	private:
		friend class ChangeHandlerHelper;

		template<typename MemberType>
		void InitializeMemberGeneric(size_t memberOffset, MemberType& member, tinyxml2::XMLElement* element, const char* attribute, void* instance);

		static void AddBinding(void* pInstance, size_t typeHash, size_t memberOffset, const std::string& bindingName);

		std::string ParseBinding(std::string& value, bool& isBinding);

		std::string m_CurrentBindingName;
		bool m_IsCurrentMemberBound;
	};


	template<typename MemberType>
	inline void XMLBindingCreationHelper::InitializeMemberGeneric(size_t memberOffset, MemberType& member, tinyxml2::XMLElement* element, const char* attribute, void* instance)
	{
		const auto* attrib = element->FindAttribute(attribute);
		if (!attrib)
			return;

		std::string val = attrib->Value();

		bool isBinding;
		const auto bindingName = ParseBinding(val, isBinding);
		m_CurrentBindingName = bindingName;
		m_IsCurrentMemberBound = isBinding;

		if (isBinding)
		{
			AddBinding(instance, typeid(member).hash_code(), memberOffset, bindingName);
		}
		else
		{
			element->QueryAttribute(attribute, &member);
		}
	}
}