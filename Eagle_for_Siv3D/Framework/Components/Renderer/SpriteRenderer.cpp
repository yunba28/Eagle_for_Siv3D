#include "SpriteRenderer.hpp"
#include <Components/Transform.hpp>

namespace eagle
{
	SpriteRenderer::SpriteRenderer()
		: mTexture()
		, mPath()
		, mColor(Palette::White)
		, mPivot(Vec2::AnchorCenter())
		, mOffset(0,0)
		, mDrawMode(DrawMode::Single)
		, mFlip(false, false)
		, mParam()
	{
	}

	void SpriteRenderer::setTexturePath(const FilePath& _path)
	{
		mTexture = Texture{ _path };
		mPath = _path;
	}

	const FilePath& SpriteRenderer::getTexturePath() const
	{
		return mPath;
	}

	void SpriteRenderer::setDiffuseColor(const Color& _color)
	{
		mColor = _color;
	}

	const Color& SpriteRenderer::getDiffuseColor() const
	{
		return mColor;
	}

	void SpriteRenderer::setPivot(Vec2 _pivot)
	{
		mPivot = _pivot;
	}

	Vec2 SpriteRenderer::getPivot() const
	{
		return mPivot;
	}

	void SpriteRenderer::setOffset(Vec2 _offset)
	{
		mOffset = _offset;
	}

	Vec2 SpriteRenderer::getOffset() const
	{
		return mOffset;
	}

	void SpriteRenderer::setDrawMode(DrawMode _mode)
	{
		mDrawMode = _mode;
	}

	DrawMode SpriteRenderer::getDrawMode() const
	{
		return mDrawMode;
	}

	void SpriteRenderer::setFlipEnable(bool _mirror, bool _flip)
	{
		mFlip = { _mirror,_flip };
	}

	bool SpriteRenderer::isMirrored() const
	{
		return mFlip.first;
	}

	bool SpriteRenderer::isFliped() const
	{
		return mFlip.second;
	}

	void SpriteRenderer::setParam(const Param& _param)
	{
		mParam = _param;
	}

	const SpriteRenderer::Param& SpriteRenderer::getParam() const
	{
		return mParam;
	}

	void SpriteRenderer::draw() const
	{
		if (mDrawMode == DrawMode::Single)
		{
			drawSingle();
		}
		else
		{
			drawSlised();
		}
	}

	void SpriteRenderer::drawSingle() const
	{
		const auto tf = getTransform();
		const auto pos = tf->getWorldPos2D();

		const auto tex = modifyTexture();

		const auto offset = -tex.region().getRelativePoint(mPivot.x, mPivot.y) + mOffset;

		if (auto rot = tf->eulerAngles().z; rot != 0.0)
		{
			tex.rotated(rot).drawAt(pos + offset, mColor);
		}
		else
		{
			tex.drawAt(pos + offset, mColor);
		}
	}

	void SpriteRenderer::drawSlised() const
	{
		const auto tf = getTransform();
		const auto pos = tf->getWorldPos2D();

		const auto tex = modifyTexture(slisedTexture());

		const auto offset = -tex.region().getRelativePoint(mPivot.x, mPivot.y) + mOffset;

		if (auto rot = tf->eulerAngles().z; rot != 0.0)
		{
			tex.rotated(rot).draw(pos + offset, mColor);
		}
		else
		{
			tex.draw(pos + offset, mColor);
		}
	}

	TextureRegion SpriteRenderer::slisedTexture() const
	{
		const auto progress = (static_cast<int32>(Scene::Time() * 1000) % static_cast<int32>(mParam.duration * 1000)) * 0.001;

		const auto size = mTexture.size() / mParam.division;

		const auto length = mParam.range.y - mParam.range.x;

		const auto index = mParam.range.x + static_cast<int32>(length * progress);

		const Point index2D{ index % mParam.division.x ,index / mParam.division.x };

		return mTexture(index2D * size, size);
	}

	TextureRegion SpriteRenderer::modifyTexture() const
	{
		const auto tf = getTransform();

		TextureRegion tex = mTexture;

		const auto scl = tf->getWorldScale2D();

		if (scl != Vec2::One())
		{
			tex = tex.scaled(scl);
		}

		const auto [fx, fy] = mFlip;

		tex = tex.mirrored(fx).flipped(fy);

		return tex;
	}

	TextureRegion SpriteRenderer::modifyTexture(const TextureRegion& _textureRegion) const
	{
		const auto tf = getTransform();

		TextureRegion tex = _textureRegion;

		const auto scl = tf->getWorldScale2D();

		if (scl != Vec2::One())
		{
			tex = tex.scaled(scl);
		}

		const auto [fx, fy] = mFlip;

		tex = tex.mirrored(fx).flipped(fy);

		return tex;
	}
}
