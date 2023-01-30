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
				if (node.input.pressedDuration() > node.action.deadZone)
				{
					return true;
				}
			}

			if (node.action.state == InputState::Up)
			{
				if ((node.input.pressedDuration() > node.action.deadZone) and node.input.up())
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

	JSON InputActionState::toJson() const
	{
		JSON res;

		for (const auto& node : mNodeList)
		{
			JSON in;

			in[U"inputGroup"] = InputGroupToJson(node.input);
			in[U"action"][U"state"] = (uint8)node.action.state;
			in[U"action"][U"deadZone"] = node.action.deadZone.count();

			res[U"node"].push_back(in);
		}

		return res;
	}

	void InputActionState::fromJson(const JSON& _json)
	{
		if (_json.hasElement(U"node"))
		{
			for (const auto& node : _json[U"node"].arrayView())
			{
				InputGroup inputGroup = JsonToInputGroup(node[U"inputGroup"]);
				Action action
				{
					(InputState)node[U"action"][U"state"].get<uint8>(),
					Duration{node[U"action"][U"deadZone"].get<double>()}
				};
				add(inputGroup, action);
			}
		}
	}

	bool InputActionDetail::operator()(const String& _actionName) const
	{
		return mInputStates.at(_actionName).action();
	}

	InputActionState& InputActionDetail::operator[](const String& _actionName)
	{
		return mInputStates[_actionName];
	}

	JSON InputActionDetail::toJson() const
	{
		JSON res;

		for (const auto& [name, state] : mInputStates)
		{
			JSON in;

			in[U"name"] = name;
			in[U"value"] = state.toJson();

			res[U"InputActionMap"].push_back(in);
		}

		return res;
	}

	void InputActionDetail::fromJson(const JSON& _json)
	{
		if (_json.hasElement(U"InputActionMap"))
		{
			for (const auto& map : _json[U"InputActionMap"].arrayView())
			{
				String name = map[U"name"].getString();
				mInputStates[name].fromJson(map[U"value"]);
			}
		}
	}
}
