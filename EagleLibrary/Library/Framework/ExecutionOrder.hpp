#pragma once

#include <unordered_map>
#include <Framework/Component.hpp>

namespace eagle
{
	class ExecutionOrder final
	{
	public:

		/// @brief T型Componentの実行順を設定する
		/// @tparam T Componentの派生クラス
		/// @param order 値が小さいほど早く実行できる
		template<class T, enable_if_t<is_base_of_v<Component,T>, int> = 0>
		static void Set(size_t order)
		{
			Set(typeid(T), order);
		}

		/// @brief T型Componentの実行順を取得する
		/// @tparam T Componentの派生クラス
		template<class T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
		static size_t Get()
		{
			return Get(typeid(T));
		}

		/// @brief Componentの実行順を設定する
		/// @param info Componentの型情報
		/// @param order 値が小さいほど早く実行できる 
		static void Set(const TypeInfo& info, size_t order);

		/// @brief Componentの実行順を取得する
		/// @param info Componentの型情報
		static size_t Get(const TypeInfo& info);

	private:

		static std::unordered_map<TypeInfo, size_t> sOrderList;

	};
}