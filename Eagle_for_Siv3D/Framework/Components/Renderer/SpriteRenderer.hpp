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

		struct Param
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

		void setTexturePath(const FilePath& _path);

		const FilePath& getTexturePath()const;

		void setDiffuseColor(const Color& _color);

		const Color& getDiffuseColor()const;

		void setPivot(Vec2 _pivot);

		Vec2 getPivot()const;

		void setOffset(Vec2 _offset);

		Vec2 getOffset()const;

		void setDrawMode(DrawMode _mode);

		DrawMode getDrawMode()const;

		void setFlipEnable(bool _mirror, bool _flip);

		bool isMirrored()const;

		bool isFliped()const;

		void setParam(const Param& _param);

		const Param& getParam()const;

	private:

		void draw()const final override;

		void drawSingle()const;

		void drawSlised()const;

		TextureRegion slisedTexture()const;

		TextureRegion modifyTexture()const;

		TextureRegion modifyTexture(const TextureRegion& _textureRegion)const;

	private:
		/// @brief 描画用テクスチャ
		Texture mTexture;

		/// @brief テクスチャ取得に使われたパス
		FilePath mPath;

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
		Param mParam;
	};
}
