#pragma once
#include "utils/databaseTable.h"
#include "debug.h"
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <functional>
#include <format>

namespace RUIN
{
	class BindingDatabase
	{
#pragma region DatabaseNodes
	public:
		enum class BindingType
		{
			POD,
			STR,
			FUNCTION
		};
	private:

		using BindingChangeHandler = std::function<void(void*)>;

		struct BindingData
		{
			BindingType type;
			size_t typeHash;
			size_t offset;
			size_t typeSize;
			unsigned bindingContextId;
			void* pInstance = nullptr;
			BindingChangeHandler changeHandler = nullptr;

			std::vector<std::function<void(void*, int)>> observers;
			std::string bindingName;
		};

		struct QueryByNameAndContext
		{
			static std::pair<std::string, unsigned> ReturnKeyFromData(const BindingData& data) { return { data.bindingName, data.bindingContextId }; }
		};

		struct QueryByName
		{
			static std::string ReturnKeyFromData(const BindingData& data) { return data.bindingName; }
		};

		struct QueryByOffset
		{
			static std::pair<void*, size_t> ReturnKeyFromData(const BindingData& data) { return { data.pInstance, data.offset }; }
		};

		struct QueryByWidget
		{
			static void* ReturnKeyFromData(const BindingData& data) { return data.pInstance; }
		};
#pragma endregion

	public:

		// Returns new context id
		unsigned PushNewContext();
		void PopContext();

		template<typename BoundDataType>
		void SetDataOnBinding(const std::string& bindingName, BoundDataType data, unsigned bindingContextId = 0);

		size_t PatchWidgetDataFromBuffer(void* buffer, int bufferSize, void* instance);

		// TODO: Currently observers are part of the binding, 
		// which means if Observe is called for a binding, 
		// and a virtual widget matching the name is spawned later, we don't listen to the new one yet.
		// Calling this twice will result in duplicate observer registrations, which is also bad.
		template<typename BoundDataType>
		void ObserveBinding(const std::string& bindingName, std::function<void(BoundDataType, int)> observer);


		void CreateBinding(const std::string& bindingName, void* pInstance, size_t typeHash, size_t memberOffset, size_t memberSize, BindingType type);
		void AddChangeHandlerToBinding(const std::string& bindingName, BindingChangeHandler func);

		template<typename BoundDataType>
		void NotifyBindingChanged(void* pInstance, size_t offset, BoundDataType newValue, int row);


	private:
		template<typename BoundDataType>
		void NotifyBindingChangeInternal(const BindingData& binding, BoundDataType newValue, int row);

		// Returns the amount of bytes read from pData
		size_t SetDataOnBindingInternal(const BindingData& binding, void* pData, bool rawInputStrings = false);

		DB::Table<BindingData, QueryByNameAndContext, QueryByName, QueryByOffset, QueryByWidget> m_Bindings;

		unsigned m_CurrentContext = 0;
		unsigned m_NumContexts = 0;
	};


	template<typename BoundDataType>
	inline void BindingDatabase::SetDataOnBinding(const std::string& bindingName, BoundDataType data, unsigned bindingContextId)
	{
		const auto bindings = m_Bindings.QueryRow<QueryByNameAndContext>({ bindingName, bindingContextId });

		RASSERT(bindings.size() == 1, std::format("Could not find matching binding! \"{}\"", bindingName).c_str());

		const auto& bindingData = *bindings[0];

		RASSERT(typeid(data).hash_code() == bindingData.typeHash, "The binding expects a different type of data!");

		SetDataOnBindingInternal(bindingData, (void*) & data);
	}

	template<typename BoundDataType>
	inline void BindingDatabase::ObserveBinding(const std::string& bindingName, std::function<void(BoundDataType, int)> observer)
	{
		const auto bindings = m_Bindings.QueryRow<QueryByName>(bindingName);

		RASSERT(bindings.size() > 0, "Could not find any matching bindings!");

		for (auto* binding : bindings)
		{
			RASSERT(typeid(BoundDataType).hash_code() == binding->typeHash, "The binding expects a different type of data!");

			binding->observers.emplace_back([observer](void* obj, int row) { observer(*static_cast<BoundDataType*>(obj), row); });
		}
	}

	template<typename BoundDataType>
	inline void BindingDatabase::NotifyBindingChanged(void* pInstance, size_t offset, BoundDataType newValue, int row)
	{
		const auto bindings = m_Bindings.QueryRow<QueryByOffset>({ pInstance, offset });

		if (bindings.empty())
		{
			return;
		}

		RASSERT(bindings.size() == 1, "Multiple bindings identified by ptr and offset?");

		NotifyBindingChangeInternal(*bindings[0], newValue, row);
	}

	template<typename BoundDataType>
	inline void BindingDatabase::NotifyBindingChangeInternal(const BindingData& binding, BoundDataType newValue, int row)
	{
		RASSERT(typeid(BoundDataType).hash_code() == binding.typeHash, "The binding expects a different type of data!");

		for (auto& it : binding.observers)
		{
			it(&newValue, row);
		}
	}
}