#pragma once
#include "../../resources/ClientData.h"
#include "../BindingDatabase.h"
#include "tinyxml2.h"
#include "debug.h"

#include <unordered_map>
#include <string>
#include <format>

#define TYPE_OF_THIS std::remove_pointer_t<decltype(this)>
#define Bind_member_to_XML(member, xmlElement, attributeName) XMLBindingCreationHelper::InitializeMember(offsetof(TYPE_OF_THIS, member), member, xmlElement, attributeName, this)
#define Bind_method_to_XML(method, xmlElement, attributeName) XMLBindingCreationHelper::InitializeMethod(&ConvertMemberToGenericFreeFunction<TYPE_OF_THIS, &TYPE_OF_THIS::##method>, xmlElement, attributeName, this)
#define OnChange(changeHandlerMemberFunction) Then(&ConvertMemberToGenericFreeFunction<TYPE_OF_THIS, &TYPE_OF_THIS::##changeHandlerMemberFunction>, this)

#define Notify_member_changed(member) UIManager::GetInstance().GetBindingDatabase().NotifyBindingChanged(this, offsetof(TYPE_OF_THIS, member), member, GetRowNumber());
#define Notify_method_changed(member, data) UIManager::GetInstance().GetBindingDatabase().NotifyBindingChanged(this, (size_t)&member, data, GetRowNumber());

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
		ChangeHandlerHelper(bool isBound, const std::string& bindingName);

		void Then(void(*action)(void*), void* initialInstance);

	private:
		bool m_IsBound;
		std::string m_BindingName;
	};


	class XMLBindingCreationHelper
	{
	public:
		static void InitializeMethod(void(*method)(void*, const char*), tinyxml2::XMLElement* element, const char* attribute, void* instance);

		static ChangeHandlerHelper InitializeMember(size_t memberOffset, std::string& str, tinyxml2::XMLElement* element, const char* attribute, void* instance);
		static ChangeHandlerHelper InitializeMember(size_t memberOffset, ClientBuffer& member, tinyxml2::XMLElement* element, const char* attribute, void* instance);

		// Explicitly listing supported types so people don't need to sift through template errors.
		static ChangeHandlerHelper InitializeMember(size_t memberOffset, float& member, tinyxml2::XMLElement* element, const char* attribute, void* instance);
		static ChangeHandlerHelper InitializeMember(size_t memberOffset, int& member, tinyxml2::XMLElement* element, const char* attribute, void* instance);

	private:
		friend class ChangeHandlerHelper;

		template<typename MemberType>
		static std::tuple<bool, std::string> InitializeMemberGeneric(size_t memberOffset, MemberType& member, tinyxml2::XMLElement* element, const char* attribute, void* instance);

		static void AddBinding(void* pInstance, size_t typeHash, size_t memberOffset, size_t memberSize, const std::string& bindingName, const std::string& attribute, BindingDatabase::BindingType type);

		static std::string ParseBinding(std::string& value, bool& isBinding);
	};


	template<typename MemberType>
	inline std::tuple<bool, std::string> XMLBindingCreationHelper::InitializeMemberGeneric(size_t memberOffset, MemberType& member, tinyxml2::XMLElement* element, const char* attribute, void* instance)
	{
		const auto* attrib = element->FindAttribute(attribute);
		if (!attrib)
			return { false, "" };

		std::string val = attrib->Value();

		bool isBinding;
		const auto bindingName = ParseBinding(val, isBinding);

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

		return { isBinding, bindingName };
	}
}