#pragma once

#include <Core/CoreFwd.hpp>

namespace eagle
{
	class ProjectSettings
	{
	public:

		/// @brief fixedUpdateを呼ぶ間隔を設定します
		/// @param _timestep 秒
		static void SetFixedTimestep(double _timestep)noexcept;

		/// @brief fixedUpdateを呼ぶ間隔を取得します
		static double GetFixedTimestep()noexcept;

		/// @brief Componentの実行順を設定します
		/// @param type 設定するComponentの型情報
		/// @param order 値が小さいほど早く実行される
		static void SetExecutionOrder(const TypeID& type, uint64 order);

		/// @brief Componentの実行順を取得します
		static uint64 GetExecutionOrder(const TypeID& type);

		/// @brief Componentの実行順を設定します
		/// @tparam T 設定するComponentの型
		/// @param order 値が小さいほど速く実行される
		template<class T>
		static void SetExecutionOrder(uint64 order)requires(std::derived_from<T, Component>)
		{
			SetExecutionOrder(typeid(T), order);
		}

		/// @brief Componentの実行順を取得します
		template<class T>
		static uint64 GetExecutionOrder()requires(std::derived_from<T, Component>)
		{
			return GetExecutionOrder(typeid(T));
		}

		/// @brief InputAxisにノードを追加する
		/// @param tag 追加するノードの名前
		static void AddInputAxis(const String& tag, const Input& input, double scale);

		/// @brief InputAxisにノードリストを追加する
		/// @param tag 追加するノードの名前
		static void AddInputAxis(const String& tag, const Array<std::pair<Input,double>>& axisList);

		/// @brief 該当する名前のノードをInputAxisから削除する
		/// @param tag 削除するノードの名前
		static void RemoveInputAxis(const String& tag);

		/// @brief InputActionにノードを追加する
		/// @param tag 追加するノードの名前
		static void AddInputAction(const String& tag, const InputGroup& action);

		/// @brief InputActionにノードリストを追加する
		/// @param tag 追加するノードの名前
		static void AddInputAction(const String& tag, const Array<InputGroup>& actionList);

		/// @brief 該当する名前のノードをInputAxisから削除する
		/// @param tag 削除するノードの名前
		static void RemoveInputAction(const String& tag);

	};
}
