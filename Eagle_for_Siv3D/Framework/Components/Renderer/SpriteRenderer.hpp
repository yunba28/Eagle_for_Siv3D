#pragma once

#include <Core/RenderSystem/DrawableComponent.hpp>

namespace eagle
{
	enum class DrawMode : uint8
	{
		Single,
		Slised,
	};

	class SpriteRenderer final : public DrawableComponent2D
	{
	public:

		struct SliseParam
		{
			/// @brief 画像の分割数
			Size division;

			/// @brief 分割した画像を再生する範囲
			Size range;

			/// @brief 再生する時間（秒）
			double duration;
		};

	public:

		SpriteRenderer();

		void setTexture(const Texture& _texture);

		void setDiffuseColor(const Color& _color);

		void setPivot(Vec2 _pivot);

		void setOffset(Vec2 _offset);

		void setDrawMode(DrawMode _mode);

		void setFlipEnable(bool _flipX, bool _flipY);

		void setSliseParam(const SliseParam& _param);

	private:

		void draw()const final override;

		void drawSingle()const;

		void drawSlised()const;

		TextureRegion slisedTexture()const;

		TextureRegion modifyTexture()const;

		TextureRegion modifyTexture(const TextureRegion& _textureRegion)const;

	private:

		/// @brief 描画する画像
		Texture mTexture;

		/// @brief 画像のディフューズカラー
		Color mColor;

		/// @brief 画像の描画基準点
		Vec2 mPivot;

		/// @brief 画像描画のオフセット
		Vec2 mOffset;

		/// @brief 描画モード
		DrawMode mDrawMode;

		/// @brief 画像を反転するか
		std::pair<bool, bool> mFlip;

		/// @brief 分割描画の情報
		SliseParam mParam;
	};
}
