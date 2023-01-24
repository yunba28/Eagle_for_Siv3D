#pragma once

#include <Core/Component/Component.hpp>

namespace eagle
{
	/// @brief 2D描画を行うためのコンポーネント
	class DrawableComponent2D : public Component
	{
	public:

		DrawableComponent2D();

		/// @brief Componentの関数の有効・無効を設定する
		/// @param _update update関数のフラグ
		/// @param _lateUpdate lateUpdate関数のフラグ 
		/// @param _fixedUpdate fixedUpdate関数のフラグ
		/// @param _draw draw関数のフラグ
		void setFunctionEnable(bool _update, bool _lateUpdate, bool _fixedUpdate, bool _draw)noexcept;

		/// @brief 描画優先度を[0,9999]の範囲で設定する
		virtual void setDrawOrder(uint64 _order)noexcept;

		virtual uint64 getDrawOrder()const noexcept;

	public:

		virtual void draw()const = 0;

		virtual void _internalAwake()override;

		virtual void _internalDraw()const;

	protected:

		bool isEnableDraw()const noexcept;

	private:

		bool mIsEnabledDraw;

	protected:

		uint64 mDrawOrder;
	};

	/// @brief 3D描画を行うためのコンポーネント
	class DrawableComponent3D : public Component
	{
	public:

		DrawableComponent3D();

		/// @brief Componentの関数の有効・無効を設定する
		/// @param _update update関数のフラグ
		/// @param _lateUpdate lateUpdate関数のフラグ 
		/// @param _fixedUpdate fixedUpdate関数のフラグ
		/// @param _draw draw関数のフラグ
		void setFunctionEnable(bool _update, bool _lateUpdate, bool _fixedUpdate, bool _draw)noexcept;

		virtual void draw()const = 0;

		virtual void _internalAwake()override;

		virtual void _internalDraw()const;

	protected:

		bool isEnableDraw()const noexcept;

	private:

		bool mIsEnabledDraw;

	};
}
