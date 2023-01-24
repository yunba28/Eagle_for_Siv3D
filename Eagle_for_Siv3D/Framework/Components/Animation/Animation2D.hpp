#pragma once

#include <Siv3D.hpp>

namespace eagle
{
	class Animation2D
	{
	public:

		struct Param
		{
			/// @brief 画像の分割数
			Size division;

			/// @brief 分割した画像を再生する範囲
			Size range;

			/// @brief ループするかどうか
			bool hasLoop;
		};

		struct KeyEvent
		{
			/// @brief 実行するイベント
			std::function<void()> func;

			/// @brief イベントが起動する時間
			double beginSec;

			/// @brief イベント起動を受け付ける終りの時間
			double endSec;

			/// @brief 呼ぶのは一度だけか
			bool callOnce{ false };

			/// @brief 最初の呼び出しを行っているか
			bool isCalled{ false };
		};

	public:

		Animation2D();

		~Animation2D();

		void update();

		void draw(Vec2 _pos)const;

		void start();

		void restart();

		void pause();

		void resume();

		void setTexturePath(const FilePath& _path);

		void setBeginEvent(const std::function<void()>& _event);

		void setEndEvent(const std::function<void()>& _event);

		void addKeyEvent(const KeyEvent& _event);

		void setDiffuseColor(const Color& _color);

		void setPivot(Vec2 _pivot);

		void setOffset(Vec2 _offset);

		void setParam(const Param& _param);

		void setDuration(const Duration& _duration);

		const FilePath& getTexturePath()const;

		const Color& getDiffuseColor()const;

		Vec2 getPivot()const;

		Vec2 getOffset()const;

		const Param& getParam()const;

		Duration getDuration()const;

		bool isStarted()const;

		bool isRunning()const;

		bool isComplated()const;

		bool isPaused()const;

	private:

		TextureRegion slisedTexture()const;

	private:

		/// @brief 描画のためのテクスチャ
		Texture mTexture;

		/// @brief テクスチャ取得のために使われたパス
		FilePath mPath;

		/// @brief アニメーションが開始した際に呼ばれるイベント
		std::function<void()> mBeginEvent;

		/// @brief アニメーションが終了した際に呼ばれるイベント
		/// @remark 途中で切り替わった場合は呼ばれないので注意
		std::function<void()> mEndEvent;

		/// @brief アニメーション中に実行されるキーイベント
		Array<KeyEvent> mKeyEvents;

		/// @brief 画像のディフューズカラー
		Color mColor;

		/// @brief 画像の描画基準点
		Vec2 mPivot;

		/// @brief 画像描画のオフセット
		Vec2 mOffset;

		/// @brief 分割描画の情報
		Param mParam;

		/// @brief アニメーション用のタイマー
		Timer mTimer;
	};
}
