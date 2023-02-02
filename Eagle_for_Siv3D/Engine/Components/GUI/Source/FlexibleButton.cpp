#include <Components/GUI/FlexibleButton.hpp>

namespace eagle::GUI
{
	FlexibleButton::FlexibleButton()
		: mFunctionBlock(unspecified)
	{
	}

	FlexibleButton::~FlexibleButton()
	{
	}

	void FlexibleButton::setFunctionBlock(const Optional<FunctionBlock>& _functionBlock)
	{
		mFunctionBlock = _functionBlock;
	}

	const Optional<FlexibleButton::FunctionBlock>& FlexibleButton::getFunctionBlock() const noexcept
	{
		return mFunctionBlock;
	}

	void FlexibleButton::onNeutral()
	{
		Button::onNeutral();

		if (mFunctionBlock and mFunctionBlock->neutral)
		{
			mFunctionBlock->neutral();
		}
	}

	void FlexibleButton::onHovered()
	{
		Button::onHovered();

		if (mFunctionBlock and mFunctionBlock->hovered)
		{
			mFunctionBlock->hovered();
		}
	}

	void FlexibleButton::onUnhovered()
	{
		Button::onUnhovered();

		if (mFunctionBlock and mFunctionBlock->unhovered)
		{
			mFunctionBlock->unhovered();
		}
	}

	void FlexibleButton::onClicked()
	{
		Button::onClicked();

		if (mFunctionBlock and mFunctionBlock->clicked)
		{
			mFunctionBlock->clicked();
		}
	}

	void FlexibleButton::onPressed()
	{
		Button::onPressed();

		if (mFunctionBlock and mFunctionBlock->pressed)
		{
			mFunctionBlock->pressed();
		}
	}

	void FlexibleButton::onReleased()
	{
		Button::onReleased();

		if (mFunctionBlock and mFunctionBlock->released)
		{
			mFunctionBlock->released();
		}
	}
}
