#include "InputAxis.hpp"

namespace eagle::Internal
{
	bool InputAxis_impl::Node::operator==(const Node& _other) const
	{
		return input == _other.input;
	}

	InputAxis_impl::InputAxis_impl()
		: mNodeList()
	{
	}

	InputAxis_impl::~InputAxis_impl()
	{
	}

	void InputAxis_impl::add(const Input& _input, double _scale)
	{
		if (mNodeList.none([&_input](const Node& node) {return node.input == _input;}))
		{
			mNodeList << Node{ _input,_scale };
		}
	}

	void InputAxis_impl::add(const Node& _node)
	{
		if (mNodeList.none([&_node](const Node& node) {return node.input == _node.input; }))
		{
			mNodeList << _node;
		}
	}

	void InputAxis_impl::remove(const Input& _input)
	{
		auto it = std::find(mNodeList.begin(), mNodeList.end(), Node{ _input,0 });

		if (it != mNodeList.end())
		{
			mNodeList.erase(it);
		}
	}

	void InputAxis_impl::remove(const Node& _node)
	{
		auto it = std::find(mNodeList.begin(), mNodeList.end(), _node);

		if (it != mNodeList.end())
		{
			mNodeList.erase(it);
		}
	}

	void InputAxis_impl::clearInput()
	{
		for (const auto& node : mNodeList)
		{
			node.input.clearInput();
		}
	}

	bool InputAxis_impl::includes(const Input& _input) const
	{
		return mNodeList.includes(Node{ _input,0 });
	}

	double InputAxis_impl::value() const
	{
		double scale{ 0 };

		for (const auto& node : mNodeList)
		{
			if (node.input.pressed())
			{
				scale += node.scale;
			}
		}

		return scale;
	}

	InputAxis_helper& InputAxis_helper::add(const String& _tag, const Input& _input, double _scale)
	{
		mAxisMap[_tag].add(_input, _scale);
		return *this;
	}

	InputAxis_helper& InputAxis_helper::add(const String& _tag, const InputAxis_impl::Node& _node)
	{
		mAxisMap[_tag].add(_node);
		return *this;
	}

	void InputAxis_helper::remove(const String& _tag)
	{
		mAxisMap.erase(_tag);
	}

	void InputAxis_helper::remove(const String& _tag, const Input& _input)
	{
		mAxisMap[_tag].remove(_input);
	}

	void InputAxis_helper::remove(const String& _tag, const InputAxis_impl::Node& _node)
	{
		mAxisMap[_tag].remove(_node);
	}

	void InputAxis_helper::clearInput(const String& _tag)
	{
		mAxisMap[_tag].clearInput();
	}

	const InputAxis_impl& InputAxis_helper::get(const String& _tag) const
	{
		return mAxisMap.at(_tag);
	}

	double InputAxis_helper::operator[](const String& _tag) const
	{
		if (mAxisMap.contains(_tag))
		{
			return mAxisMap.at(_tag).value();
		}

		return 0.0;
	}
}
