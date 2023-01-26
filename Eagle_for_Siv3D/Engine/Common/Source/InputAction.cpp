#include <Common/InputAction.hpp>

namespace eagle::backend
{
	InputActionState& InputActionState::add(const InputGroup& _input, const Action& _action)
	{
		if (not contains(_input))
		{
			mNodeList.push_back(Node{ _input,_action });
		}

		return *this;
	}

	InputActionState& InputActionState::remove(const InputGroup& _input)
	{
		auto nodeEq = [&_input](const Node& node)
		{
			return node.input == _input;
		};

		mNodeList.remove_if(nodeEq);

		return *this;
	}

	void InputActionState::clear()
	{
		mNodeList.clear();
	}

	bool InputActionState::action() const
	{
		for (const auto& node : mNodeList)
		{
			if (node.action.state == InputState::Down)
			{
				if (node.input.down())
				{
					return true;
				}
			}

			if (node.action.state == InputState::Pressed)
			{
				if (node.input.pressedDuration() >= node.action.deadZone)
				{
					return true;
				}
			}

			if (node.action.state == InputState::Up)
			{
				if ((node.input.pressedDuration() >= node.action.deadZone) and node.input.up())
				{
					return true;
				}
			}
		}

		return false;
	}

	bool InputActionState::contains(const InputGroup& _input) const
	{
		auto nodeEq = [&_input](const Node& node)
		{
			return node.input == _input;
		};

		return mNodeList.any(nodeEq);
	}

	bool InputActionDetail::operator()(const String& _actionName) const
	{
		return mInputStates.at(_actionName).action();
	}

	InputActionState& InputActionDetail::operator[](const String& _actionName)
	{
		return mInputStates[_actionName];
	}
}
