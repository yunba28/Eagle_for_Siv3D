#pragma once

#include <Framework/Detail/SceneObjectDetail.hpp>

namespace eagle
{

	class SceneObject final : Uncopyable
	{
	public:

		SceneObject();

		/// @brief T型のActorを作成する
		/// @tparam T Actorの派生クラス
		/// @param _name Actorの名前
		template<class T = Actor>
		WeakObject<T> createActor(const String& _name = U"MyActor");

		/// @brief 画面の背景色を設定する
		void setBackgroundColor(const Color& _background);

		/// @brief 2Dカメラの弱参照を取得する
		WeakObject<Camera2D> getCamera2D()const noexcept;

		/// @brief 3Dカメラの弱参照を取得する
		WeakObject<DebugCamera3D> getCamera3D()const noexcept;

		/// @brief 画面の背景色を取得する
		const Color& getBackgroundColor()const noexcept;

		ObjectHandle<backend::SceneObjectDetail> getDetail()const noexcept;

	private:

		SharedObject<backend::SceneObjectDetail> mSceneObject;

	};

	template<class T>
	WeakObject<T> SceneObject::createActor(const String& _name)
	{
		return mSceneObject->createActor<T>(_name);
	}
}
