#include "InputAxis.hpp"

namespace eagle::Internal
{
	InputAxis_impl& InputAxis_impl::add(const Input& _input, double _scale)
	{
		Node node{ _input,_scale };

		if (not mInputs.includes(node))
		{
			mInputs << std::move(node);
		}

		return *this;
	}

	void InputAxis_impl::remove(const Input& _input)
	{
		mInputs.remove_if([&_input](const Node& node) {return node.input == _input; });
	}

	double InputAxis_impl::scale() const
	{
		double scale{ 0 };
		for (const auto& node : mInputs)
		{
			if (node.input.pressed())
			{
				scale += node.scale;
			}
		}
		return scale;
	}

	bool InputAxis_impl::Node::operator==(const Node& _other) const
	{
		return input == _other.input;
	}
}

namespace eagle
{
	Input InputSerializer::asInput() const
	{
		return Input{ static_cast<InputDeviceType>(mDeviceType),mCode,mPlayerIndex };
	}
}
