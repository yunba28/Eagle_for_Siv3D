#pragma once

#include <typeindex>
#include <concepts>

#include <Siv3D/Array.hpp>
#include <Siv3D/HashTable.hpp>
#include <Siv3D/String.hpp>
#include <Siv3D/Uncopyable.hpp>

using s3d::Array;
using s3d::HashTable;
using s3d::String;
using s3d::Uncopyable;
using s3d::Error;

namespace eagle
{
	/// @brief ActorやComponentを管理し, その更新や描画を行うオブジェクト
	class SceneObject;

	/// @brief SceneObjectが管理する一般的なオブジェクト
	class Actor;

	/// @brief Actorの振る舞いを決定するクラス
	class Component;

	/// @brief Componentの更新用派生クラス
	class Behavior;

	/// @brief Componentの2D描画用派生クラス
	class Renderer2D;

	/// @brief Componentの3D描画用派生クラス
	class Renderer3D;

	/// @brief Actorの座標・回転・拡縮を管理します
	class Transform;

	/// @brief 型情報を保持するクラス
	using TypeInfo = std::type_index;

	namespace backend
	{
		class SceneObjectDetail;
	}
}
