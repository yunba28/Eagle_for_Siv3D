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


#include <Core/Scene/BasicScene.hpp>

namespace eagle
{
	template<class Data>
	class GameWorld final
	{
	public:
		using BasicScene = BasicScene<Data>;

		using State_t = String;

		using Data_t = Data;

		GameWorld();

		GameWorld(const GameWorld&) = delete;

		GameWorld(GameWorld&& other) noexcept;

		/// @brief シーン管理を初期化します。
		/// @param data 共有データ
		explicit GameWorld(const std::shared_ptr<Data>& data);

		GameWorld& operator=(const GameWorld&) = delete;

		GameWorld& operator=(GameWorld&& other) noexcept;

		/// @brief シーンを登録します。
		/// @tparam SceneType シーンの型
		/// @param state シーンのキー
		/// @return シーンの登録に成功した場合 true, それ以外の場合は false
		template <class SceneType>
		GameWorld& add(const State_t& state);

		/// @brief 最初のシーンを初期化します。
		/// @param state 最初のシーン
		/// @return 初期化に成功した場合 true, それ以外の場合は false
		bool init(const State_t& state);

		/// @brief 現在のシーンの更新処理のみを行います。
		/// @remark 通常はこの関数は使用しません。
		/// @return シーンの更新処理に成功した場合 true, それ以外の場合は false
		bool updateScene();

		/// @brief 現在のシーンの描画処理のみを行います。
		/// @remark 通常はこの関数は使用しません。
		void drawScene() const;

		/// @brief 現在のシーンの更新処理と描画処理を行います。
		/// @return シーンの更新処理に成功した場合 true, それ以外の場合は false
		bool update();

		/// @brief 共有データを取得します。
		/// @return 共有データへのポインタ
		[[nodiscard]]
		SharedObject<Data> get() noexcept;

		/// @brief 共有データを取得します。
		/// @return 共有データへのポインタ
		[[nodiscard]]
		const SharedObject<const Data> get() const noexcept;

		/// @brief シーンを変更します。
		/// @param state 次のシーンのキー
		/// @param transitionTime フェードイン・アウトの時間
		/// @param crossFade ロスフェードを有効にするか
		/// @return シーンの変更が開始される場合 true, それ以外の場合は false
		bool changeScene(const State_t& state, const Duration& transitionTime = Duration{ 2.0 }, CrossFade crossFade = CrossFade::No);

		/// @brief シーンを変更します。
		/// @param state 次のシーンのキー
		/// @param transitionTimeMillisec フェードイン・アウトの時間（ミリ秒）
		/// @param crossFade クロスフェードを有効にするか
		/// @return シーンの変更が開始される場合 true, それ以外の場合は false
		bool changeScene(const State_t& state, int32 transitionTimeMillisec, CrossFade crossFade = CrossFade::No);

		/// @brief デフォルトのフェードイン・アウトに使う色を設定します。
		/// @param color デフォルトのフェードイン・アウトに使う色
		/// @return *this
		GameWorld& setFadeColor(const ColorF& color) noexcept;

		/// @brief デフォルトのフェードイン・アウトに使う色を返します。
		/// @return デフォルトのフェードイン・アウトに使う色
		[[nodiscard]]
		const ColorF& getFadeColor() const noexcept;

		/// @brief エラーの発生を通知します。
		/// @return この関数を呼ぶと、以降のこのクラスの `GameWorld::update()` が false を返します。
		void notifyError() noexcept;

	private:

		using Scene_t = SharedObject<BasicScene>;

		using FactoryFunction_t = std::function<Scene_t()>;

		std::unique_ptr<GameWorld*> mGameWorldPtr;

		HashTable<State_t, FactoryFunction_t> mFactories;

		SharedObject<Data> mData;

		Scene_t mCurrent;

		Scene_t mNext;

		State_t mCurrentState;

		State_t mNextState;

		Optional<State_t> mFirst;

		enum class TransitionState
		{
			None_,

			FadeIn,

			Active,

			FadeOut,

			FadeInOut,

		} mTransitionState = TransitionState::None_;

		Stopwatch mStopwatch;

		int32 mTransitionTimeMillisec = 1000;

		ColorF mFadeColor = Palette::Black;

		CrossFade mCrossFade = CrossFade::No;

		bool mError = false;

		[[nodiscard]]
		bool updateSingle();

		[[nodiscard]]
		bool updateCross();

		[[nodiscard]]
		bool hasError() const noexcept;
	};
}

#include "GameWorld.ipp"
