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

		template<typename BoundDataType>
		void ObserveBinding(const std::string& bindingName, std::function<void(BoundDataType)> observer);

		using BindingChangeHandler = std::function<void(void*)>;

		void CreateBinding(const std::string& bindingName, void* pInstance, size_t typeHash, size_t memberOffset);
		void AddChangeHandlerToBinding(const std::string& bindingName, BindingChangeHandler func);

		template<typename BoundDataType>
		void NotifyBindingChanged(void* pInstance, size_t offset, BoundDataType newValue);

	private:
		template<typename BoundDataType>
		void NotifyBindingChangeInternal(const std::string& bindingName, BoundDataType newValue);

		struct BindingData
		{
			void* pInstance = nullptr;
			size_t typeHash;
			size_t offset;
			BindingChangeHandler changeHandler = nullptr;

			std::vector<std::function<void(void*)>> observers;
		};

		struct ReverseLookupKey
		{
			void* pInstance = nullptr;
			size_t offset;

			bool operator==(const ReverseLookupKey& other) const;
		};

		struct Hasher
		{
			size_t operator()(const ReverseLookupKey& key) const noexcept;
		};

		std::unordered_map<std::string, BindingData> m_Bindings;
		std::unordered_map<ReverseLookupKey, std::string, Hasher> m_BindingNames;
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

	template<typename BoundDataType>
	inline void BindingDatabase::ObserveBinding(const std::string& bindingName, std::function<void(BoundDataType)> observer)
	{
		RASSERT(m_Bindings.contains(bindingName), "Could not find matching binding!");

		auto& bindingData = m_Bindings.at(bindingName);

		RASSERT(typeid(BoundDataType).hash_code() == bindingData.typeHash, "The binding expects a different type of data!");

		bindingData.observers.emplace_back([observer](void* obj) { observer(*static_cast<BoundDataType*>(obj)); });
	}

	template<typename BoundDataType>
	inline void BindingDatabase::NotifyBindingChanged(void* pInstance, size_t offset, BoundDataType newValue)
	{
		ReverseLookupKey key
		{
			pInstance,
			offset
		};

		if (!m_BindingNames.contains(key))
		{
			return;
		}

		NotifyBindingChangeInternal(m_BindingNames.at(key), newValue);
	}

	template<typename BoundDataType>
	inline void BindingDatabase::NotifyBindingChangeInternal(const std::string& bindingName, BoundDataType newValue)
	{
		RASSERT(m_Bindings.contains(bindingName), "Could not find matching binding!");

		auto& bindingData = m_Bindings.at(bindingName);

		RASSERT(typeid(BoundDataType).hash_code() == bindingData.typeHash, "The binding expects a different type of data!");

		for (auto& it : bindingData.observers)
		{
			it(&newValue);
		}
	}
}