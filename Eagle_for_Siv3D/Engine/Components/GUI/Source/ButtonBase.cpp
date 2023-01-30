#include <Components/GUI/ButtonBase.hpp>

#include <Framework/Actor.hpp>
#include <Components/GUI/Background.hpp>
#include <Components/GUI/Text.hpp>

namespace eagle::GUI
{
	ButtonBase::ButtonBase()
		: mBackground()
		, mColorPalette()
	{
	}

	ButtonBase::~ButtonBase()
	{
	}

	void ButtonBase::setDrawOrder(size_t _order)
	{
		getBackground()->setDrawOrder(_order);
	}

	void ButtonBase::setGUIRect(const GUIRect& _GUIRect)
	{
		Selectable::setGUIRect(_GUIRect);
		getBackground()->setGUIRect(_GUIRect);
	}

	void ButtonBase::setRegion(RectF _region)
	{
		Selectable::setRegion(_region);
		getBackground()->setRegion(_region);
	}

	void ButtonBase::setSize(SizeF _size)
	{
		Selectable::setSize(_size);
		getBackground()->setSize(_size);
	}

	void ButtonBase::setAnchor(Vec2 _anchor)
	{
		Selectable::setAnchor(_anchor);
		getBackground()->setAnchor(_anchor);
	}

	void ButtonBase::setPivot(Vec2 _pivot)
	{
		Selectable::setPivot(_pivot);
		getBackground()->setPivot(_pivot);
	}

	void ButtonBase::setColorPallete(const ColorPalette& _colorPalette)
	{
		mColorPalette = _colorPalette;
	}

	ObjectHandle<Background> ButtonBase::getBackground() const noexcept
	{
		return mBackground.lock();
	}

	const ButtonBase::ColorPalette& ButtonBase::getColorPalette() const noexcept
	{
		return mColorPalette;
	}

	void ButtonBase::awake()
	{
		auto actor = getActor();

		if (mBackground = actor->getComponent<Background>();mBackground.expired())
		{
			mBackground = actor->attachComponent<Background>();
		}

		auto background = mBackground.lock();
		background->setDrawOrder(950);
		background->setGUIRect(getGUIRect());
	}

	void ButtonBase::onEnable()
	{
		mBackground.lock()->setColor(mColorPalette.neutral);
	}

	void ButtonBase::onDisable()
	{
		mBackground.lock()->setColor(mColorPalette.disable);
	}

	void ButtonBase::onNeutral()
	{
		mBackground.lock()->setColor(mColorPalette.neutral);
	}

	void ButtonBase::onHovered()
	{
		mBackground.lock()->setColor(mColorPalette.hovered);
	}

	void ButtonBase::onClicked()
	{
		mBackground.lock()->setColor(mColorPalette.pressed);
	}

	void ButtonBase::onPressed()
	{
		mBackground.lock()->setColor(mColorPalette.pressed);
	}

	void ButtonBase::onReleased()
	{
		mBackground.lock()->setColor(mColorPalette.neutral);
	}
}
