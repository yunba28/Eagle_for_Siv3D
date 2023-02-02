#include <Components/GUI/Button.hpp>

#include <Framework/Actor.hpp>
#include <Components/GUI/Background.hpp>
#include <Components/GUI/Text.hpp>

namespace eagle::GUI
{
	Button::Button()
		: mBackground()
		, mColorBlock()
		, mTextureBlock(unspecified)
	{
	}

	Button::~Button()
	{
	}

	void Button::setDrawOrder(size_t _order)
	{
		getBackground()->setDrawOrder(_order);
	}

	void Button::setGUIRect(const GUIRect& _GUIRect)
	{
		Selectable::setGUIRect(_GUIRect);
		getBackground()->setGUIRect(_GUIRect);
	}

	void Button::setRegion(RectF _region)
	{
		Selectable::setRegion(_region);
		getBackground()->setRegion(_region);
	}

	void Button::setSize(SizeF _size)
	{
		Selectable::setSize(_size);
		getBackground()->setSize(_size);
	}

	void Button::setAnchor(Vec2 _anchor)
	{
		Selectable::setAnchor(_anchor);
		getBackground()->setAnchor(_anchor);
	}

	void Button::setPivot(Vec2 _pivot)
	{
		Selectable::setPivot(_pivot);
		getBackground()->setPivot(_pivot);
	}

	void Button::setColorBlock(const ColorBlock& _colorBlock)
	{
		mColorBlock = _colorBlock;
	}

	void Button::setTextureBlock(const Optional<TextureBlock>& _textureBlock)
	{
		mTextureBlock = _textureBlock;
	}

	ObjectHandle<Background> Button::getBackground() const noexcept
	{
		return mBackground.lock();
	}

	const Button::ColorBlock& Button::getColorBlock() const noexcept
	{
		return mColorBlock;
	}

	const Optional<Button::TextureBlock>& Button::getTextureBlock() const noexcept
	{
		return mTextureBlock;
	}

	void Button::awake()
	{
		mGUIRect.size.set(120, 40);

		auto actor = getActor();

		if (mBackground = actor->getComponent<Background>();mBackground.expired())
		{
			mBackground = actor->attachComponent<Background>();
		}

		auto background = mBackground.lock();
		background->setGUIRect(getGUIRect());
	}

	void Button::start()
	{
		auto background = mBackground.lock();

		background->setColor(mColorBlock.neutral);

		if (mTextureBlock and mTextureBlock->neutral)
		{
			background->setTexture(mTextureBlock->neutral);
		}
	}

	void Button::onEnable()
	{
		auto background = mBackground.lock();

		background->setColor(mColorBlock.neutral);

		if (mTextureBlock and mTextureBlock->neutral)
		{
			background->setTexture(mTextureBlock->neutral);
		}
	}

	void Button::onDisable()
	{
		auto background = mBackground.lock();

		background->setColor(mColorBlock.disable);

		if (mTextureBlock and mTextureBlock->disable)
		{
			background->setTexture(mTextureBlock->disable);
		}
	}

	void Button::onHovered()
	{
		auto background = mBackground.lock();

		background->setColor(mColorBlock.hovered);

		if (mTextureBlock and mTextureBlock->hovered)
		{
			background->setTexture(mTextureBlock->hovered);
		}
	}

	void Button::onUnhovered()
	{
		auto background = mBackground.lock();

		background->setColor(mColorBlock.neutral);

		if (mTextureBlock and mTextureBlock->neutral)
		{
			background->setTexture(mTextureBlock->neutral);
		}
	}

	void Button::onClicked()
	{
		mBackground.lock()->setColor(mColorBlock.pressed);

		auto background = mBackground.lock();

		background->setColor(mColorBlock.pressed);

		if (mTextureBlock and mTextureBlock->pressed)
		{
			background->setTexture(mTextureBlock->pressed);
		}
	}

	void Button::onReleased()
	{
		auto background = mBackground.lock();

		background->setColor(mColorBlock.hovered);

		if (mTextureBlock and mTextureBlock->hovered)
		{
			background->setTexture(mTextureBlock->hovered);
		}
	}
}
