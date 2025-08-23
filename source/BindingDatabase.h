#pragma once
#include "debug.h"
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <functional>


namespace RUIN
{
	class BindingDatabase
	{
	public:
		template<typename BoundDataType>
		void SetDataOnBinding(const std::string& bindingName, BoundDataType data);

		using BindingChangeHandler = std::function<void(void*)>;

		void CreateBinding(const std::string& bindingName, void* pInstance, size_t typeHash, size_t memberOffset);
		void AddChangeHandlerToBinding(const std::string& bindingName, BindingChangeHandler func);

	private:
		struct BindingData
		{
			void* pInstance = nullptr;
			size_t typeHash;
			size_t offset;
			BindingChangeHandler changeHandler = nullptr;
		};

		std::unordered_map<std::string, BindingData> m_Bindings;
	};


	template<typename BoundDataType>
	inline void BindingDatabase::SetDataOnBinding(const std::string& bindingName, BoundDataType data)
	{
		RASSERT(m_Bindings.contains(bindingName), "Could not find matching binding!");

		const auto& bindingData = m_Bindings.at(bindingName);

		RASSERT(typeid(data).hash_code() == bindingData.typeHash, "The binding expects a different type of data!");

		char* basePtr = static_cast<char*>(bindingData.pInstance);
		
		auto* pMember = reinterpret_cast<BoundDataType*>(basePtr + bindingData.offset);
		*pMember = std::move(data);

		if (bindingData.changeHandler)
		{
			bindingData.changeHandler(bindingData.pInstance);
		}
	}
}