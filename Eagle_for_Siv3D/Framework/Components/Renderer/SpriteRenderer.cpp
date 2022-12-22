#include "SpriteRenderer.hpp"
#include <Components/Transform.hpp>
#include <Utility/Load.hpp>

namespace eagle
{
	SpriteRenderer::SpriteRenderer()
		: mAssetTag()
		, mColor(Palette::White)
		, mPivot(Vec2::AnchorCenter())
		, mOffset(0,0)
		, mDrawMode(DrawMode::Single)
		, mFlip(false, false)
		, mParam()
	{
	}

	void SpriteRenderer::setTextureAssetTag(const AssetTag& _assetTag)
	{
		mAssetTag = _assetTag;
	}

	const AssetTag& SpriteRenderer::getTextureAssetTag() const
	{
		return mAssetTag;
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

	void SpriteRenderer::setSliseParam(const SliseParam& _param)
	{
		mParam = _param;
	}

	const SpriteRenderer::SliseParam& SpriteRenderer::getSliseParam() const
	{
		return mParam;
	}

	void SpriteRenderer::draw() const
	{
		if (mAssetTag.empty())
			return;

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
		const auto texture = TextureAsset(mAssetTag);

		const auto progress = (static_cast<int32>(Scene::Time() * 1000) % static_cast<int32>(mParam.duration * 1000)) * 0.001;

		const auto size = texture.size() / mParam.division;

		const auto length = mParam.range.y - mParam.range.x;

		const auto index = mParam.range.x + static_cast<int32>(length * progress);

		const Point index2D{ index % mParam.division.x ,index / mParam.division.x };

		return texture(index2D * size, size);
	}

	TextureRegion SpriteRenderer::modifyTexture() const
	{
		const auto tf = getTransform();

		TextureRegion tex = TextureAsset(mAssetTag);

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

	template<>
	bool Load<SpriteRenderer>(const String& path, SpriteRenderer& renderer)
	{
		if (FileSystem::Extension(path) != U"sprite")
			return false;

		const INI ini{ path };

		if (ini.isEmpty())
			return false;

		if (not ini.hasSection(U"Properties"))
			return false;

		AssetTag assetTag{};
		Color diffuseColor{};
		Vec2 pivot{};
		Vec2 offset{};
		DrawMode drawMode{};
		bool isMirror{};
		bool isFlip{};
		SpriteRenderer::SliseParam param{};

		try
		{
			if (ini.hasValue(U"Properties", U"AssetTag"))
			{
				assetTag = ini[U"Properties.AssetTag"];
			}

			if (ini.hasValue(U"Properties", U"DiffuseColor"))
			{
				diffuseColor = Parse<Color>(ini[U"Properties.DiffuseColor"]);
			}

			if (ini.hasValue(U"Properties", U"Pivot"))
			{
				pivot = Parse<Vec2>(ini[U"Properties.Pivot"]);
			}

			if (ini.hasValue(U"Properties", U"Offset"))
			{
				offset = Parse<Vec2>(ini[U"Properties.Offset"]);
			}

			if (ini.hasValue(U"Properties", U"DrawMode"))
			{
				drawMode = static_cast<DrawMode>(Parse<uint8>(ini[U"Properties.DrawMode"]));
			}

			if (ini.hasValue(U"Properties", U"IsMirror"))
			{
				isMirror = Parse<bool>(ini[U"Properties.IsMirror"]);
			}

			if (ini.hasValue(U"Properties", U"IsFlip"))
			{
				isFlip = Parse<bool>(ini[U"Properties.IsFlip"]);
			}

			if (ini.hasValue(U"Properties", U"Division"))
			{
				param.division = Parse<Point>(ini[U"Properties.Division"]);
			}

			if (ini.hasValue(U"Properties", U"Range"))
			{
				param.range = Parse<Point>(ini[U"Properties.Range"]);
			}

			if (ini.hasValue(U"Properties", U"Duration"))
			{
				param.duration = Parse<double>(ini[U"Properties.Duration"]);
			}
		}
		catch (ParseError&)
		{
			return false;
		}

		renderer.setTextureAssetTag(assetTag);
		renderer.setDiffuseColor(diffuseColor);
		renderer.setPivot(pivot);
		renderer.setOffset(offset);
		renderer.setDrawMode(drawMode);
		renderer.setFlipEnable(isMirror, isFlip);
		renderer.setSliseParam(param);

		return true;
	}

	template<>
	bool Save<SpriteRenderer>(const String& path, SpriteRenderer& renderer)
	{
		String out{ path };

		if (FileSystem::Extension(out) != U"sprite")
		{
			auto it = std::find(out.begin(), out.end(), U'.');
			out.erase(it + 1, out.end());
			out += U"sprite";
		}

		INI ini{};

		ini.addSection(U"Properties");
		ini.write(U"Properties", U"AssetTag", renderer.getTextureAssetTag());
		ini.write(U"Properties", U"DiffuseColor", renderer.getDiffuseColor());
		ini.write(U"Properties", U"Pivot", renderer.getPivot());
		ini.write(U"Properties", U"Offset", renderer.getOffset());
		ini.write(U"Properties", U"DrawMode", static_cast<int32>(renderer.getDrawMode()));
		ini.write(U"Properties", U"IsMirror", renderer.isMirrored());
		ini.write(U"Properties", U"IsFlip", renderer.isFliped());

		const auto& param = renderer.getSliseParam();

		ini.write(U"Properties", U"Division", param.division);
		ini.write(U"Properties", U"Range", param.range);
		ini.write(U"Properties", U"Duration", param.duration);

		return ini.save(path);
	}
}
