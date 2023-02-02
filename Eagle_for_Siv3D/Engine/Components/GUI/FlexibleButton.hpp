#pragma once

#include <Components/GUI/Button.hpp>

namespace eagle::GUI
{
	class FlexibleButton : public Button
	{
	public:

		struct FunctionBlock
		{
			std::function<void(void)> neutral;
			std::function<void(void)> hovered;
			std::function<void(void)> unhovered;
			std::function<void(void)> clicked;
			std::function<void(void)> pressed;
			std::function<void(void)> released;
		};

	public:

		FlexibleButton();

		virtual ~FlexibleButton();

		void setFunctionBlock(const Optional<FunctionBlock>& _functionBlock);

		const Optional<FunctionBlock>& getFunctionBlock()const noexcept;

	private:

		virtual void onNeutral()override;

		virtual void onHovered()override;

		virtual void onUnhovered()override;

		virtual void onClicked()override;

		virtual void onPressed()override;

		virtual void onReleased()override;

	private:

		Optional<FunctionBlock> mFunctionBlock;

	};
}
