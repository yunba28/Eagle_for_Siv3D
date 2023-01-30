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
		ButtonBase::onNeutral();

		if (mFunctionBlock and mFunctionBlock->neutral)
		{
			mFunctionBlock->neutral();
		}
	}

	void FlexibleButton::onHovered()
	{
		ButtonBase::onHovered();

		if (mFunctionBlock and mFunctionBlock->hovered)
		{
			mFunctionBlock->hovered();
		}
	}

	void FlexibleButton::onClicked()
	{
		ButtonBase::onClicked();

		if (mFunctionBlock and mFunctionBlock->clicked)
		{
			mFunctionBlock->clicked();
		}
	}

	void FlexibleButton::onPressed()
	{
		ButtonBase::onPressed();

		if (mFunctionBlock and mFunctionBlock->pressed)
		{
			mFunctionBlock->pressed();
		}
	}

	void FlexibleButton::onReleased()
	{
		ButtonBase::onReleased();

		if (mFunctionBlock and mFunctionBlock->released)
		{
			mFunctionBlock->released();
		}
	}
}
