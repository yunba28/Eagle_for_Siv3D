#pragma once

#include <Siv3D.hpp>

namespace eagle::GUI
{
	struct GUIRect
	{
		/// @brief UIの描画可能範囲
		RectF region{ Scene::Rect() };

		/// @brief UIの大きさ
		SizeF size{ Vec2::All(100) };

		/// @brief region内でのUIの原点
		Vec2 anchor{ Vec2::AnchorCenter()};

		/// @brief UIの原点
		Vec2 pivot{ Vec2::AnchorCenter() };

		constexpr RectF getViewRect(Vec2 _pos, Vec2 _scale = Vec2::One())const
		{
			// スケーリングされたUIのサイズ
			auto scaledSize = size * _scale;

			// 描画領域上のUI原点
			auto origin = region.getRelativePoint(anchor.x, anchor.y);
			// UI原点からのオフセット
			auto offset = -(scaledSize * pivot);

			return RectF{ origin + _pos + offset, scaledSize };
		}
	};

	struct Padding
	{
		constexpr Padding()noexcept = default;
		constexpr Padding(double _t, double _b, double _l, double _r)
			: top(_t), bottom(_b), left(_l), right(_r) {};
		constexpr Padding(std::initializer_list<double> _iniList)
			: top(*(_iniList.begin())), bottom(*(_iniList.begin() + 1)), left(*(_iniList.begin() + 2)), right(*(_iniList.begin() + 3)) {};

		/// @brief パディングを適応した矩形を返す
		constexpr RectF apply(const RectF& _rect)const
		{
			return _rect.stretched(-top, -right, -bottom, -left);
		}

		double top{ 0 };
		double bottom{ 0 };
		double left{ 0 };
		double right{ 0 };
	};

	struct Margin
	{
		constexpr Margin()noexcept = default;
		constexpr Margin(double _t, double _b, double _l, double _r)
			: top(_t), bottom(_b), left(_l), right(_r) {};
		constexpr Margin(std::initializer_list<double> _iniList)
			: top(*(_iniList.begin())), bottom(*(_iniList.begin() + 1)), left(*(_iniList.begin() + 2)), right(*(_iniList.begin() + 3)) {};

		/// @brief マージンを適応した矩形を返す
		constexpr RectF apply(const RectF& _rect)const
		{
			return _rect.stretched(top, right, bottom, left);
		}

		double top{ 0 };
		double bottom{ 0 };
		double left{ 0 };
		double right{ 0 };
	};
}
