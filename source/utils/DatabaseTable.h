#pragma once
#include <vector>
#include <unordered_map>
#include <tuple>
#include <iostream>

namespace DB
{
	namespace Helper
	{
		template<typename T, typename Tuple>
		struct tuple_index;

		template<typename T, typename... Types>
		struct tuple_index<T, std::tuple<T, Types...>> 
		{
			static constexpr std::size_t value = 0;
		};

		template<typename T, typename U, typename... Types>
		struct tuple_index<T, std::tuple<U, Types...>>
		{
			static constexpr std::size_t value = 1 + tuple_index<T, std::tuple<Types...>>::value;
		};
	}

	template<typename Data, typename... Queries>
	class Table
	{
	public:
		void Insert(Data&& data);

		template<typename Query>
		auto QueryRow(const typename Query::KeyType& key) -> std::vector<Data*>;

	private:
		template<std::size_t I = 0>
		void InsertIndices(const Data& data, size_t index);

		std::vector<Data> m_Data;
		std::tuple<std::unordered_map<typename Queries::KeyType, std::vector<size_t>, typename Queries::Hasher>...> m_ReverseLookups;
	};

	template<typename Data, typename ...Queries>
	inline void Table<Data, Queries...>::Insert(Data&& data)
	{
		const auto size = m_Data.size();
		m_Data.emplace_back(data);
		InsertIndices(*m_Data.rbegin(), size);
	}

	template<typename Data, typename ...Queries>
	template<typename Query>
	inline auto Table<Data, Queries...>::QueryRow(const typename Query::KeyType& key) -> std::vector<Data*>
	{
		constexpr std::size_t I = Helper::tuple_index<Query, std::tuple<Queries...>>::value;
		auto& reverseLookup = std::get<I>(m_ReverseLookups);

		std::vector<Data*> result{};

		auto it = reverseLookup.find(key);
		if (it != reverseLookup.end())
		{
			for (auto index : it->second)
			{
				result.emplace_back(&m_Data[index]);
			}
		}

		return result;
	}

	template<typename Data, typename ...Queries>
	template<std::size_t I>
	inline void Table<Data, Queries...>::InsertIndices(const Data& data, size_t index)
	{
		if constexpr (I < sizeof...(Queries))
		{
			auto& reverseLookup = std::get<I>(m_ReverseLookups);
			using Query = std::tuple_element_t<I, std::tuple<Queries...>>;

			const Query query{};

			reverseLookup[query.ReturnKeyFromData(data)].emplace_back(index);

			InsertIndices<I + 1>(data, index);
		}
	}
}