#pragma once

#include <unordered_map>
#include <Framework/Component.hpp>

namespace eagle
{
	class ExecutionOrder final
	{
	public:

		/// @brief T�^Component�̎��s����ݒ肷��
		/// @tparam T Component�̔h���N���X
		/// @param order �l���������قǑ������s�ł���
		template<class T, enable_if_t<is_base_of_v<Component,T>, int> = 0>
		static void Set(size_t order)
		{
			Set(typeid(T), order);
		}

		/// @brief T�^Component�̎��s�����擾����
		/// @tparam T Component�̔h���N���X
		template<class T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
		static size_t Get()
		{
			return Get(typeid(T));
		}

		/// @brief Component�̎��s����ݒ肷��
		/// @param info Component�̌^���
		/// @param order �l���������قǑ������s�ł��� 
		static void Set(const TypeInfo& info, size_t order);

		/// @brief Component�̎��s�����擾����
		/// @param info Component�̌^���
		static size_t Get(const TypeInfo& info);

	private:

		static std::unordered_map<TypeInfo, size_t> sOrderList;

	};
}