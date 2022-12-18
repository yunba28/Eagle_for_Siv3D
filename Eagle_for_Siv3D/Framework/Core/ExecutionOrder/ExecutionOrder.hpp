#pragma once

#include <Core/Component/Component.hpp>

namespace eagle
{
	/// @brief Componentの実行順序を設定する
	class ExecutionOrder final
	{
	private:

		ExecutionOrder() = delete;

		~ExecutionOrder() = delete;

	public:

		static void Set(const std::type_index& type, uint64 order);

		template<class T>
		static void Set(uint64 order)requires(std::derived_from<T, Component>)
		{
			Set(typeid(T), order);
		}

		static uint64 Get(const std::type_index& type);

		template<class T>
		static uint64 Get()requires(std::derived_from<T, Component>)
		{
			Get(typeid(T));
		}

	private:

		static HashTable<std::type_index, uint64> cOrderList;

	};
}
