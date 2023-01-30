#include <Components/GUI/Slider.hpp>

#include <Framework/Actor.hpp>
#include <Components/GUI/Background.hpp>
#include <Common/InputAction.hpp>

namespace eagle::GUI
{
	Slider::Slider()
		: mEmptyRect()
		, mFillRect()
		, mValue(0.0)
		, mValueMin(0.0)
		, mValueMax(1.0)
		, mSlideDirection(SlideDirection::LeftToRight)
		, mWholeNumbers(false)
		, mGrabbed(false)
	{
	}

	Slider::~Slider()
	{
	}

	void Slider::setDrawOrder(size_t _order)
	{
		mEmptyRect.lock()->setDrawOrder(_order);
		mFillRect.lock()->setDrawOrder(_order + 100);
	}

	void Slider::setGUIRect(const GUIRect& _GUIRect)
	{
		mGUIRect = _GUIRect;
		mEmptyRect.lock()->setGUIRect(_GUIRect);
		mFillRect.lock()->setGUIRect(_GUIRect);
	}

	void Slider::setRegion(RectF _region)
	{
		mGUIRect.region = _region;
		mEmptyRect.lock()->setRegion(_region);
		mFillRect.lock()->setRegion(_region);
	}

	void Slider::setSize(SizeF _size)
	{
		mGUIRect.size = _size;
		mEmptyRect.lock()->setSize(_size);
		mFillRect.lock()->setSize(_size);
	}

	void Slider::setAnchor(Vec2 _anchor)
	{
		mGUIRect.anchor = _anchor;
		mEmptyRect.lock()->setAnchor(_anchor);
		mFillRect.lock()->setAnchor(_anchor);
	}

	void Slider::setPivot(Vec2 _pivot)
	{
		mGUIRect.pivot = _pivot;
		mEmptyRect.lock()->setPivot(_pivot);
		mFillRect.lock()->setPivot(_pivot);
	}

	void Slider::setLengthAndThickness(double _length, double _thickness, SlideDirection _slideDirection)
	{
		mSlideDirection = _slideDirection;

		switch (mSlideDirection)
		{
		case SlideDirection::LeftToRight: setSize(SizeF{ _length,_thickness }); break;
		case SlideDirection::RightToLeft: setSize(SizeF{ _length,_thickness }); break;
		case SlideDirection::TopToBottom: setSize(SizeF{ _thickness,_length }); break;
		case SlideDirection::BottomToTop: setSize(SizeF{ _thickness,_length }); break;
		[[unlikely]] default: break;
		}
	}

	void Slider::setEmptyColor(const Color& _color) noexcept
	{
		mEmptyRect.lock()->setColor(_color);
	}

	void Slider::setFillColor(const Color& _color) noexcept
	{
		mFillRect.lock()->setColor(_color);
	}

	void Slider::setEmptyTexture(const TextureRegion& _texture) noexcept
	{
		mEmptyRect.lock()->setTexture(_texture);
	}

	void Slider::setFillTexture(const TextureRegion& _texture) noexcept
	{
		mFillRect.lock()->setTexture(_texture);
	}

	void Slider::setValue(double _value) noexcept
	{
		mValue = _value / (mValueMax - mValueMin);
	}

	void Slider::setValueMinMax(double _min, double _max) noexcept
	{
		mValueMin = _min;
		mValueMax = _max;
	}

	void Slider::setValueMinMax(Vec2 _minmax) noexcept
	{
		mValueMin = _minmax.x;
		mValueMax = _minmax.y;
	}

	void Slider::setSlideDirection(SlideDirection _slideDirection) noexcept
	{
		mSlideDirection = _slideDirection;
	}

	void Slider::applyWholeNumbers(bool _flag) noexcept
	{
		mWholeNumbers = _flag;
	}

	const Color& Slider::getEmptyColor() const noexcept
	{
		return mEmptyRect.lock()->getColor();
	}

	const Color& Slider::getFillColor() const noexcept
	{
		return mFillRect.lock()->getColor();
	}

	const TextureRegion& Slider::getEmptyTexture() const noexcept
	{
		return mEmptyRect.lock()->getTexture();
	}

	const TextureRegion& Slider::getFillTexture() const noexcept
	{
		return mFillRect.lock()->getTexture();
	}

	double Slider::getValue() const noexcept
	{
		const auto value = ((mValueMax - mValueMin) * mValue) + mValueMin;
		return (mWholeNumbers) ? Floor(value) : value;
	}

	double Slider::getValueMin() const noexcept
	{
		return mValueMin;
	}

	double Slider::getValueMax() const noexcept
	{
		return mValueMax;
	}

	bool Slider::appliedWholeNumbers() const noexcept
	{
		return mWholeNumbers;
	}

	RectF Slider::makeFillRect(RectF _rect, double _progress) const noexcept
	{
		switch (mSlideDirection)
		{
		case SlideDirection::LeftToRight:
			return _rect.scaledAt(_rect.leftCenter(), _progress, 1.0);
			break;
		case SlideDirection::RightToLeft:
			return _rect.scaledAt(_rect.rightCenter(), _progress, 1.0);
			break;
		case SlideDirection::TopToBottom:
			return _rect.scaledAt(_rect.topCenter(), 1.0, _progress);
			break;
		case SlideDirection::BottomToTop:
			return _rect.scaledAt(_rect.bottomCenter(), 1.0, _progress);
			break;
		[[unlikely]] default: break;
		}

		return RectF::Empty();
	}

	RoundRect Slider::makeRoundRect(RectF _rect) const noexcept
	{
		switch (mSlideDirection)
		{
		case SlideDirection::LeftToRight:
			return RoundRect{ _rect,_rect.h * 0.5 };
			break;
		case SlideDirection::RightToLeft:
			return RoundRect{ _rect,_rect.h * 0.5 };
			break;
		case SlideDirection::TopToBottom:
			return RoundRect{ _rect,_rect.w * 0.5 };
			break;
		case SlideDirection::BottomToTop:
			return RoundRect{ _rect,_rect.w * 0.5 };
			break;
		[[unlikely]] default: break;
		}

		return RoundRect{ RectF::Empty() ,0 };
	}

	double Slider::calculateValue(RectF _rect) const noexcept
	{
		double relativePos{ 0.0 };
		double value{ 0.0 };

		const auto cursorPos = Cursor::PosF();

		switch (mSlideDirection)
		{
		case SlideDirection::LeftToRight:
			relativePos = cursorPos.x - _rect.x;
			value = relativePos / _rect.w;
			break;
		case SlideDirection::RightToLeft:
			relativePos = cursorPos.x - _rect.x;
			value = 1.0 - (relativePos / _rect.w);
			break;
		case SlideDirection::TopToBottom:
			relativePos = cursorPos.y - _rect.y;
			value = relativePos / _rect.h;
			break;
		case SlideDirection::BottomToTop:
			relativePos = cursorPos.y - _rect.y;
			value = 1.0 - (relativePos / _rect.h);
			break;
		[[unlikely]] default: break;
		}

		return value;
	}

	void Slider::awake()
	{
		mGUIRect.size = SizeF{ 180,20 };
		mGUIRect.anchor = Vec2::AnchorTopLeft();
		mGUIRect.pivot = Vec2::AnchorTopLeft();

		auto actor = getActor();

		mEmptyRect = actor->attachComponent<Background>();
		mFillRect = actor->attachComponent<Background>();

		{
			auto background = mEmptyRect.lock();
			background->setGUIRect(mGUIRect);
			background->setColor(Color{ 192,192,192 });
			background->setDrawOrder(1000);
		}

		{
			auto background = mFillRect.lock();
			background->setGUIRect(mGUIRect);
			background->setColor(Color{ 0,255,0 });
			background->setDrawOrder(1100);
		}
	}

	void Slider::update()
	{
		if (mGrabbed)
		{
			mGrabbed = InputAction(U"Pressed");
		}

		Selectable::update();

		const auto transform = getTransform();
		const auto pos = transform->getPos2D();
		const auto scale = transform->getScale2D();

		const auto viewRect = mGUIRect.getViewRect(pos, scale);

		if (mGUIRect.region.hasArea())
		{
			if (mGrabbed)
			{
				mValue = Clamp(calculateValue(viewRect), 0.0, 1.0);
			}

			if (isSelected())
			{
				if (const auto wheel = Mouse::Wheel() * 0.01; wheel != 0.0)
				{
					mValue = Clamp(mValue + wheel, 0.0, 1.0);
				}
			}
		}

		const auto fill = makeFillRect(viewRect, mValue);

		mFillRect.lock()->setSize(fill.size);
	}

	void Slider::onNeutral()
	{
	}

	void Slider::onHovered()
	{
		onSelect();
	}

	void Slider::onClicked()
	{
		mGrabbed = true;
	}

	void Slider::onPressed()
	{
	}

	void Slider::onReleased()
	{
		mGrabbed = false;
	}
}
