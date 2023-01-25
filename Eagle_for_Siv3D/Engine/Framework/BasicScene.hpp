﻿#pragma once

#include <Framework/SceneObject.hpp>

#include <Siv3D/SceneManager.hpp>

using s3d::SceneManager;

namespace eagle
{
	template<class State, class Data>
	class BasicScene : public SceneManager<State, Data>::Scene
	{
	public:

		using typename SceneManager<State, Data>::Scene::InitData;

		using Super = BasicScene<State, Data>;

		explicit BasicScene(const InitData& _init);

		virtual ~BasicScene();

		/// @brief 画面の背景色を設定する
		void setBackgroundColor(const Color& _background);

		/// @brief 2Dカメラの弱参照を取得する
		WeakObject<Camera2D> getCamera2D()const noexcept;

		/// @brief 3Dカメラの弱参照を取得する
		WeakObject<DebugCamera3D> getCamera3D()const noexcept;

		/// @brief 画面の背景色を取得する
		const Color& getBackgroundColor()const noexcept;

		/// @brief シーンの構築時に呼びだされます
		virtual void awake() {}

		/// @brief シーンのは生地に呼び出されます
		virtual void dispose() {}

		/// @brief 通常時の更新処理です
		virtual void update()override;

		/// @brief 通常時の描画処理です
		virtual void draw()const override;

	private:

		SceneObject mSceneObject;

	};
}

namespace eagle
{
	template<class State, class Data>
	inline BasicScene<State, Data>::BasicScene(const InitData& _init)
		: Scene(_init)
		, mSceneObject()
	{
		awake();
	}

	template<class State, class Data>
	inline BasicScene<State, Data>::~BasicScene()
	{
		dispose();
	}

	template<class State, class Data>
	inline void BasicScene<State, Data>::setBackgroundColor(const Color & _background)
	{
		mSceneObject.setBackgroundColor(_background);
	}

	template<class State, class Data>
	inline WeakObject<Camera2D> BasicScene<State, Data>::getCamera2D() const noexcept
	{
		return mSceneObject.getCamera2D();
	}

	template<class State, class Data>
	inline WeakObject<DebugCamera3D> BasicScene<State, Data>::getCamera3D() const noexcept
	{
		return mSceneObject.getCamera3D();
	}

	template<class State, class Data>
	inline const Color& BasicScene<State, Data>::getBackgroundColor() const noexcept
	{
		return mSceneObject.getBackgroundColor();
	}

	template<class State, class Data>
	inline void BasicScene<State, Data>::update()
	{
		mSceneObject.getDetail()->update();
	}

	template<class State, class Data>
	inline void BasicScene<State, Data>::draw() const
	{
		mSceneObject.getDetail()->draw();
	}
}