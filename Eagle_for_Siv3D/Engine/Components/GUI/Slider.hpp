#pragma once

#include <Framework/Renderer2D.hpp>
#include <Components/GUI/Detail/Selectable.hpp>

namespace eagle::GUI
{
	enum class SlideDirection : uint8
	{
		LeftToRight,
		RightToLeft,
		TopToBottom,
		BottomToTop
	};

	class Slider : public backend::Selectable
	{
	public:

		Slider();

		virtual ~Slider();

		void setDrawOrder(size_t _order);

		virtual void setGUIRect(const GUIRect& _GUIRect)override;

		virtual void setRegion(RectF _region)override;

		virtual void setSize(SizeF _size)override;

		virtual void setAnchor(Vec2 _anchor)override;

		virtual void setPivot(Vec2 _pivot)override;

		/// @brief スライダーの長さと太さを設定する
		/// @param _length 長さ
		/// @param _thickness 太さ
		/// @param _slideDirection どの方向に加算されるか
		void setLengthAndThickness(double _length, double _thickness, SlideDirection _slideDirection);

		/// @brief スライダーの背景色を設定する
		void setEmptyColor(const Color& _color)noexcept;

		/// @brief スライダーの埋まっている部分の背景色を設定する
		void setFillColor(const Color& _color)noexcept;

		/// @brief スライダーの背景画像を設定する
		void setEmptyTexture(const TextureRegion& _texture)noexcept;

		/// @brief スライダーの埋まっている部分の画像を設定する
		void setFillTexture(const TextureRegion& _texture)noexcept;

		/// @brief スライダーの値を設定する
		void setValue(double _value)noexcept;

		/// @brief スライダーの最小値と最大値を設定する
		/// @param _min 最小値
		/// @param _max 最大値
		void setValueMinMax(double _min, double _max)noexcept;

		/// @brief スライダーの最小値と最大値を設定する
		/// @param _minmax x:最小値 y:最大値
		void setValueMinMax(Vec2 _minmax)noexcept;

		/// @brief スライダーの値がどの方向に加算されるか設定する
		void setSlideDirection(SlideDirection _slideDirection)noexcept;

		/// @brief スライダーの値を整数単位にするかどうか
		void applyWholeNumbers(bool _flag)noexcept;

		/// @brief スライダーの背景色を取得する
		const Color& getEmptyColor()const noexcept;

		/// @brief スライダーの埋まっている部分の色を取得する
		const Color& getFillColor()const noexcept;

		/// @brief スライダーの背景画像を取得する
		const TextureRegion& getEmptyTexture()const noexcept;

		/// @brief スライダーの埋まっている部分の画像を取得する
		const TextureRegion& getFillTexture()const noexcept;

		/// @brief スライダーの値を取得する
		double getValue()const noexcept;

		/// @brief スライダーの最小値を取得する
		double getValueMin()const noexcept;

		/// @brief スライダーの最大値を取得する
		double getValueMax()const noexcept;

		/// @brief 値の小数切り捨てが適応しているか
		bool appliedWholeNumbers()const noexcept;

	private:

		RectF makeFillRect(RectF _rect, double _progress)const noexcept;

		RoundRect makeRoundRect(RectF _rect)const noexcept;

		double calculateValue(RectF _rect)const noexcept;

	private:

		virtual void awake()override;

		virtual void update()override;

		virtual void onNeutral()override;

		virtual void onHovered()override;

		virtual void onClicked()override;

		virtual void onPressed()override;

		virtual void onReleased()override;

	private:

		WeakObject<class Background> mEmptyRect;

		WeakObject<class Background> mFillRect;

		double mValue;

		double mValueMin;

		double mValueMax;

		SlideDirection mSlideDirection;

		bool mWholeNumbers;

		bool mRoundCap;

		bool mGrabbed;

	};
}
