#pragma once
#include "../../resources/ClientData.h"
#include "../BindingDatabase.h"
#include "tinyxml2.h"
#include "debug.h"

#include <unordered_map>
#include <string>
#include <format>

#define TYPE_OF_THIS std::remove_pointer_t<decltype(this)>
#define Bind_member_to_XML(member, xmlElement, attributeName) _DataBindingDescriptor.InitializeMember(offsetof(TYPE_OF_THIS, member), member, xmlElement, attributeName, this)
#define Bind_method_to_XML(method, xmlElement, attributeName) _DataBindingDescriptor.InitializeMethod(&ConvertMemberToGenericFreeFunction<TYPE_OF_THIS, &TYPE_OF_THIS::##method>, xmlElement, attributeName, this)
#define OnChange(changeHandlerMemberFunction) Then(&ConvertMemberToGenericFreeFunction<TYPE_OF_THIS, &TYPE_OF_THIS::##changeHandlerMemberFunction>, this)

#define Notify_member_changed(member) UIManager::GetInstance().GetBindingDatabase().NotifyBindingChanged(this, offsetof(TYPE_OF_THIS, member), member);



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
		ChangeHandlerHelper InitializeMember(size_t memberOffset, ClientBuffer& member, tinyxml2::XMLElement* element, const char* attribute, void* instance);

		// Explicitly listing supported types so people don't need to sift through template errors.
		ChangeHandlerHelper InitializeMember(size_t memberOffset, float& member, tinyxml2::XMLElement* element, const char* attribute, void* instance);
		ChangeHandlerHelper InitializeMember(size_t memberOffset, int& member, tinyxml2::XMLElement* element, const char* attribute, void* instance);

	private:
		friend class ChangeHandlerHelper;

		template<typename MemberType>
		void InitializeMemberGeneric(size_t memberOffset, MemberType& member, tinyxml2::XMLElement* element, const char* attribute, void* instance);

		static void AddBinding(void* pInstance, size_t typeHash, size_t memberOffset, size_t memberSize, const std::string& bindingName, const std::string& attribute, BindingDatabase::BindingType type);

		std::string ParseBinding(std::string& value, bool& isBinding);

		std::string m_CurrentBindingName;
		bool m_IsCurrentMemberBound;
	};


	template<typename MemberType>
	inline void XMLBindingCreationHelper::InitializeMemberGeneric(size_t memberOffset, MemberType& member, tinyxml2::XMLElement* element, const char* attribute, void* instance)
	{
		m_IsCurrentMemberBound = false;
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
			BindingDatabase::BindingType type = BindingDatabase::BindingType::POD;
			
			if (typeid(MemberType) == typeid(std::string))
			{
				type = BindingDatabase::BindingType::STR;
			}

			AddBinding(instance, typeid(member).hash_code(), memberOffset, sizeof(MemberType), bindingName, attribute, type);
		}
		else
		{
			if constexpr (std::is_pod_v<MemberType>) {
				element->QueryAttribute(attribute, &member);
			}
			else
			{
				RASSERT(false, std::format("{} Is not supported without binding!", attribute).c_str());
			}
		}
	}
}