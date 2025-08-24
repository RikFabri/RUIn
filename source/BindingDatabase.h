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
#pragma region DatabaseNodes
	public:
		enum class BindingType
		{
			POD,
			STR
		};
	private:

		using BindingChangeHandler = std::function<void(void*)>;

		struct BindingData
		{
			BindingType type;
			size_t typeHash;
			size_t offset;
			size_t typeSize;
			void* pInstance = nullptr;
			BindingChangeHandler changeHandler = nullptr;

			std::vector<std::function<void(void*)>> observers;
		};

		struct BindingContext
		{
			std::vector<std::string> m_BindingNames;
		};

		struct WidgetId
		{
			void* pInstance = nullptr;
			unsigned bindingContextId;

			bool operator==(const WidgetId& other) const;
		};

		struct BindingContextLookupKeyHasher
		{
			size_t operator()(const WidgetId& key) const noexcept;
		};

		struct ReverseLookupKey
		{
			void* pInstance = nullptr;
			size_t offset;

			bool operator==(const ReverseLookupKey& other) const;
		};

		struct ReverseLookupKeyHasher
		{
			size_t operator()(const ReverseLookupKey& key) const noexcept;
		};

		struct BindingIdentifier
		{
			std::string bindingName;
			unsigned bindingContextId;

			bool operator==(const BindingIdentifier& other) const;
		};

		struct BindingIdentifierHasher
		{
			size_t operator()(const BindingIdentifier& key) const noexcept;
		};
#pragma endregion

	public:

		// Returns new context id
		unsigned PushNewContext();
		void PopContext();

		template<typename BoundDataType>
		void SetDataOnBinding(const std::string& bindingName, BoundDataType data, unsigned bindingContextId = 0);

		// Returns the amount of bytes read from pData
		size_t SetDataOnBinding(const std::string& bindingName, void* pData, unsigned bindingContextId = 0, bool rawInputStrings = false);

		size_t PatchWidgetDataFromBuffer(void* buffer, int bufferSize, void* instance, unsigned bindingContextId);

		template<typename BoundDataType>
		void ObserveBinding(const std::string& bindingName, std::function<void(BoundDataType)> observer, unsigned bindingContextId = 0);


		void CreateBinding(const std::string& bindingName, void* pInstance, size_t typeHash, size_t memberOffset, size_t memberSize, BindingType type);
		void AddChangeHandlerToBinding(const std::string& bindingName, BindingChangeHandler func);

		template<typename BoundDataType>
		void NotifyBindingChanged(void* pInstance, size_t offset, BoundDataType newValue);


	private:
		template<typename BoundDataType>
		void NotifyBindingChangeInternal(const BindingIdentifier& bindingId, BoundDataType newValue);


		//struct TypeBindingDescriptor
		//{
		//	struct NamedOffset
		//	{
		//		std::string m_MemberName;
		//		size_t m_MemberOffset;

		//		bool operator==(const NamedOffset& other);
		//	};

		//	std::vector<NamedOffset> m_Offsets;
		//};


		// TODO: this whole thing needs some proper memory management. Small C++ generic database implementation for later?
		std::unordered_map<BindingIdentifier, BindingData, BindingIdentifierHasher> m_Bindings;
		std::unordered_map<ReverseLookupKey, BindingIdentifier, ReverseLookupKeyHasher> m_BindingNames;
		std::unordered_map<WidgetId, BindingContext, BindingContextLookupKeyHasher> m_WidgetBindingNames;

		unsigned m_CurrentContext = 0;
		unsigned m_NumContexts = 0;

		BindingIdentifier GetCurrentBindingIdentifier(const std::string& bindingName) const;

		//std::unordered_map<size_t, TypeBindingDescriptor> m_DescriptorPerType;
	};


	template<typename BoundDataType>
	inline void BindingDatabase::SetDataOnBinding(const std::string& bindingName, BoundDataType data, unsigned bindingContextId)
	{
		BindingIdentifier bindingId{ bindingName, bindingContextId };
		RASSERT(m_Bindings.contains(bindingId), "Could not find matching binding!");

		const auto& bindingData = m_Bindings.at(bindingId);

		RASSERT(typeid(data).hash_code() == bindingData.typeHash, "The binding expects a different type of data!");

		SetDataOnBinding(bindingName, (void*) & data, bindingContextId);
	}

	template<typename BoundDataType>
	inline void BindingDatabase::ObserveBinding(const std::string& bindingName, std::function<void(BoundDataType)> observer, unsigned bindingContextId)
	{
		BindingIdentifier bindingId{ bindingName, bindingContextId };
		RASSERT(m_Bindings.contains(bindingId), "Could not find matching binding!");

		auto& bindingData = m_Bindings.at(bindingId);

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
	inline void BindingDatabase::NotifyBindingChangeInternal(const BindingIdentifier& bindingId, BoundDataType newValue)
	{
		RASSERT(m_Bindings.contains(bindingId), "Could not find matching binding!");

		auto& bindingData = m_Bindings.at(bindingId);

		RASSERT(typeid(BoundDataType).hash_code() == bindingData.typeHash, "The binding expects a different type of data!");

		for (auto& it : bindingData.observers)
		{
			it(&newValue);
		}
	}
}