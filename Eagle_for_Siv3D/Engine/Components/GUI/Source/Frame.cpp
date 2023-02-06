#include <Components/GUI/Frame.hpp>

#include <Framework/Actor.hpp>

namespace eagle::GUI
{
	Frame::Frame()
		: mColor(Palette::White)
		, mThickness()
	{
		setEnable({ false,false,false }, { false,true });
	}

	Frame::~Frame()
	{
	}

	void Frame::setGUIRect(const GUIRect& _GUIRect)
	{
		mGUIRect = _GUIRect;
	}

	void Frame::setRegion(RectF _region)
	{
		mGUIRect.region = _region;
	}

	void Frame::setSize(SizeF _size)
	{
		mGUIRect.size = _size;
	}

	void Frame::setAnchor(Vec2 _anchor)
	{
		mGUIRect.anchor = _anchor;
	}

	void Frame::setPivot(Vec2 _pivot)
	{
		mGUIRect.pivot = _pivot;
	}

	void Frame::setColor(const Color& _color) noexcept
	{
		mColor = _color;
	}

	void Frame::setThickness(double _thickness) noexcept
	{
		mThickness.inner = _thickness / 2;
		mThickness.outer = _thickness / 2;
	}

	void Frame::setThickness(double _innerThickness, double _outerThickness) noexcept
	{
		mThickness.inner = _innerThickness;
		mThickness.outer = _outerThickness;
	}

	void Frame::setThickness(Vec2 _thickness) noexcept
	{
		mThickness.inner = _thickness.x;
		mThickness.outer = _thickness.y;
	}

	const Color& Frame::getColor() const noexcept
	{
		return mColor;
	}

	Vec2 Frame::getThickness() const noexcept
	{
		return { mThickness.inner,mThickness.outer };
	}

	void Frame::drawScreen() const
	{
		if (mGUIRect.region.isEmpty())
			return;

		const ScopedViewport2D viewport{ mGUIRect.region.asRect() };

		const auto transform = getTransform();
		const auto pos = transform->getPos2D();
		const auto scale = transform->getScale2D();

		mGUIRect.getViewRect(pos, scale)
			.drawFrame(mThickness.inner, mThickness.outer, mColor);
	}
}
