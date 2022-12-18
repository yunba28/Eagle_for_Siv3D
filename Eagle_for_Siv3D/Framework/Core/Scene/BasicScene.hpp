#pragma once

//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2022 Ryo Suzuki
//	Copyright (c) 2016-2022 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
// original -> s3d::SceneManager is originally designed by Takuto Takahashi (OpenSiv3D Project)
//-----------------------------------------------

#include <Core/Scene/ScenePimpl.hpp>

namespace eagle
{
	template<class Data>
	class BasicScene : Uncopyable
	{
	public:

		using State_t = String;

		using Data_t = Data;

		struct InitData
		{
			State_t state;

			SharedObject<Data_t> data;

			GameWorld<Data_t>*& world;

			InitData() = default;

			InitData(const State_t& _state, const SharedObject<Data_t>& _data, GameWorld<Data_t>*& _world);
		};

	public:

		explicit BasicScene(const InitData& _ini);

		virtual ~BasicScene() = default;

		/// @brief 通常時の更新処理です。
		virtual void update() {}

		/// @brief フェードイン時の更新処理です。
		/// @param t フェードインの進度 [0.0, 1.0]
		virtual void updateFadeIn(double) {}

		/// @brief フェードアウト時の更新処理です。
		/// @param t フェードアウトの進度 [0.0, 1.0]
		virtual void updateFadeOut(double) {}

		/// @brief 通常時の描画処理です。
		virtual void draw() const {}

		/// @brief フェードイン時の描画処理です。
		/// @param t フェードインの進度 [0.0, 1.0]
		virtual void drawFadeIn(double t) const;

		/// @brief フェードアウト時の描画処理です。
		/// @param t フェードアウトの進度 [0.0, 1.0]
		virtual void drawFadeOut(double t) const;

	public:

		void _internalUpdate();

		void _internalDraw()const;

	protected:

		/// @brief 現在のステートのキーを取得します。
		/// @return 現在のステートのキー
		[[nodiscard]]
		const State_t& getState() const;

		/// @brief 共有データへの参照を取得します。
		/// @return 共有データへの参照
		template <class DataType = Data, std::enable_if_t<not std::disjunction_v<std::is_array<DataType>, std::is_void<DataType>>>* = nullptr>
		[[nodiscard]]
		DataType& getData() const;

		/// @brief シーンの変更をリクエストします。
		/// @param state 次のシーンのキー
		/// @param transitionTime フェードイン・アウトの時間
		/// @param crossFade クロスフェードを有効にするか
		/// @return シーンの変更が開始される場合 true, それ以外の場合は false
		bool changeScene(const State_t& state, const Duration& transitionTime = Duration{ 2.0 }, CrossFade crossFade = CrossFade::No);

		/// @brief シーンの変更をリクエストします。
		/// @param state 次のシーンのキー
		/// @param transitionTimeMillisec フェードイン・アウトの時間（ミリ秒）
		/// @param crossFade クロスフェードを有効にするか
		/// @return シーンの変更が開始される場合 true, それ以外の場合は false
		bool changeScene(const State_t& state, int32 transitionTimeMillisec, CrossFade crossFade = CrossFade::No);

		/// @brief エラーの発生を通知します。
		/// @remark この関数を呼ぶと、以降のこのシーンを管理するクラスの `SceneManager::update()` が false を返します。
		void notifyError();

		template<class T = Actor>
		WeakObject<T> createActor(const String& _name)requires(std::derived_from<T, Actor>);

		template<class T = Actor>
		WeakObject<T> getActor(const String& _name)const requires(std::derived_from<T, Actor>);

		template<class T = Actor>
		WeakObject<T> getActorByTag(const String& _tag)const requires(std::derived_from<T, Actor>);

		template<class T = Actor>
		Array<WeakObject<T>> getActors()const requires(std::derived_from<T, Actor>);

		template<class T = Actor>
		Array<WeakObject<T>> getActorsByTag(const String& _tag)const requires(std::derived_from<T, Actor>);

		template<class T = Actor>
		void destroyActor(const String& _name)requires(std::derived_from<T, Actor>);

		void destroyActorsByTag(const String& _tag);

	private:

		/// @brief フレームワーク内のシーンの詳細な実装
		SharedObject<Internal::ScenePimpl> mScenePimpl;

		State_t mState;

		SharedObject<Data_t> mData;

		GameWorld<Data_t>*& mWorld;
	};
}

#include "BasicScene.ipp"
