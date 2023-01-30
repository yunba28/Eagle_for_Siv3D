#include <Components/GUI/Background.hpp>

#include <Framework/Actor.hpp>

namespace eagle::GUI
{
	Background::Background()
		: mTextureRegion()
		, mColor(Palette::White)
	{
		setEnable({ false,false,false }, { false,true });

		setGUIRect({ Scene::Rect(),Scene::Size(),Vec2{0,0},Vec2{0,0} });
	}

	Background::~Background()
	{
	}

	void Background::setGUIRect(const GUIRect& _GUIRect)
	{
		mGUIRect = _GUIRect;
	}

	void Background::setRegion(RectF _region)
	{
		mGUIRect.region = _region;
	}

	void Background::setSize(SizeF _size)
	{
		mGUIRect.size = _size;
	}

	void Background::setAnchor(Vec2 _anchor)
	{
		mGUIRect.anchor = _anchor;
	}

	void Background::setPivot(Vec2 _pivot)
	{
		mGUIRect.pivot = _pivot;
	}

	void Background::setTexture(const TextureRegion& _texture)
	{
		mTextureRegion = _texture;
	}

	void Background::setColor(const Color& _color)
	{
		mColor = _color;
	}

	const TextureRegion& Background::getTexture() const noexcept
	{
		return mTextureRegion;
	}

	const Color& Background::getColor() const noexcept
	{
		return mColor;
	}

	void Background::drawScreen() const
	{
		if (mGUIRect.region.isEmpty())
			return;

		const ScopedViewport2D viewport{ mGUIRect.region.asRect() };

		const auto transform = getTransform();
		const auto pos = transform->getPos2D();
		const auto scale = transform->getScale2D();

		const auto viewRect = mGUIRect.getViewRect(pos, scale);

		if (mTextureRegion.texture)
		{
			viewRect(mTextureRegion).draw(mColor);
		}
		else
		{
			viewRect.draw(mColor);
		}
	}
}
