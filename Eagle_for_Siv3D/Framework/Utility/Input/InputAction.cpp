#include "InputAction.hpp"

namespace eagle::Internal
{
	InputAction_impl::InputAction_impl()
		: mActionList()
	{
	}

	InputAction_impl::~InputAction_impl()
	{
	}

	void InputAction_impl::add(const InputGroup& _action)
	{
		if (not mActionList.includes(_action))
		{
			mActionList << _action;
		}
	}

	void InputAction_impl::remove(const InputGroup& _action)
	{
		if (mActionList.includes(_action))
		{
			mActionList.remove(_action);
		}
	}

	bool InputAction_impl::includes(const InputGroup& _action)
	{
		return mActionList.includes(_action);
	}

	bool InputAction_impl::down() const
	{
		for (const auto& action : mActionList)
		{
			if (action.down())
				return true;
		}
		return false;
	}

	bool InputAction_impl::pressed() const
	{
		for (const auto& action : mActionList)
		{
			if (action.pressed())
				return true;
		}
		return false;
	}

	bool InputAction_impl::up() const
	{
		for (const auto& action : mActionList)
		{
			if (action.up())
				return true;
		}
		return false;
	}

	void InputAction_impl::clearInput()
	{
		for (const auto& action : mActionList)
		{
			for (const auto& input : action.inputs())
			{
				input.clearInput();
			}
			for (const auto& combinations : action.inputCombinations())
			{
				combinations.input1().clearInput();
				combinations.input2().clearInput();
			}
		}
	}

	InputAction_helper& InputAction_helper::add(const String& _tag, const InputGroup& _action)
	{
		mActionMap[_tag].add(_action);
		return *this;
	}

	void InputAction_helper::remove(const String& _tag)
	{
		mActionMap.erase(_tag);
	}

	void InputAction_helper::remove(const String& _tag, const InputGroup& _action)
	{
		mActionMap[_tag].remove(_action);
	}

	const InputAction_impl& InputAction_helper::get(const String& _tag)const
	{
		return mActionMap.at(_tag);
	}

	bool InputAction_helper::down(const String& _tag) const
	{
		if (mActionMap.contains(_tag))
		{
			return mActionMap.at(_tag).down();
		}
		return false;
	}

	bool InputAction_helper::pressed(const String& _tag) const
	{
		if (mActionMap.contains(_tag))
		{
			return mActionMap.at(_tag).pressed();
		}
		return false;
	}

	bool InputAction_helper::up(const String& _tag) const
	{
		if (mActionMap.contains(_tag))
		{
			return mActionMap.at(_tag).up();
		}
		return false;
	}

	void InputAction_helper::clearInput(const String& _tag)
	{
		if (mActionMap.contains(_tag))
		{
			mActionMap.at(_tag).clearInput();
		}
	}
}
