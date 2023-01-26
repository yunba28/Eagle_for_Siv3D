#include <Common/InputAxis.hpp>

namespace eagle::backend
{
	InputAxisState& InputAxisState::add(const InputGroup& _input, double _scale)
	{
		if (not contains(_input))
		{
			mNodeList.push_back(Node{ _input,_scale });
		}

		return *this;
	}

	InputAxisState& InputAxisState::add(const Pad& _pad)
	{
		if (not contains(_pad.state))
		{
			mPadList.push_back(_pad);
		}

		return *this;
	}

	InputAxisState& InputAxisState::remove(const InputGroup& _input)
	{
		auto nodeEq = [&_input](const Node& node)->bool
		{
			return node.input == _input;
		};

		mNodeList.remove_if(nodeEq);

		return *this;
	}

	InputAxisState& InputAxisState::remove(const PadState& _padState)
	{
		auto padEq = [&_padState](const Pad& pad)->bool
		{
			return pad.state == _padState;
		};

		mPadList.remove_if(padEq);

		return *this;
	}

	void InputAxisState::clear()
	{
		mNodeList.clear();
		mPadList.clear();
	}

	double InputAxisState::axis(uint8 _playerIndex) const
	{
		double result{ 0.0 };

		if (auto xc = XInput(_playerIndex);xc.isConnected())
		{
			xc.setLeftThumbDeadZone();
			xc.setRightThumbDeadZone();

			for (const auto& pad : mPadList)
			{
				double ret{ 0.0 };

				// 左スティックのX軸を取得
				if (pad.state == PadState::LThumbX)
				{
					ret = xc.leftThumbX * pad.scale;
				}
				// 左スティックのY軸を取得
				else if (pad.state == PadState::LThumbY)
				{
					ret = xc.leftThumbY * pad.scale;
				}
				// 右スティックのX軸を取得
				else if (pad.state == PadState::RThumbX)
				{
					ret = xc.rightThumbX * pad.scale;
				}
				// 右スティックのY軸を取得
				else if (pad.state == PadState::RThumbY)
				{
					ret = xc.rightThumbY * pad.scale;
				}

				result += ret;
			}
		}

		if (result != 0.0)
			return result;

		for (const auto& node : mNodeList)
		{
			if (node.input.pressed())
			{
				result += node.scale;
			}
		}

		return result;
	}

	bool InputAxisState::contains(const InputGroup& _input) const
	{
		auto nodeEq = [&_input](const Node& node)->bool
		{
			return node.input == _input;
		};

		return mNodeList.any(nodeEq);
	}

	bool InputAxisState::contains(const PadState& _padState) const
	{
		auto padEq = [&_padState](const Pad& pad)->bool
		{
			return pad.state == _padState;
		};

		return mPadList.any(padEq);
	}

	double InputAxisDetail::operator()(const String& _axisName, uint8 _playerIndex) const
	{
		return mInputStates.at(_axisName).axis(_playerIndex);
	}

	InputAxisState& InputAxisDetail::operator[](const String& _axisName)
	{
		return mInputStates[_axisName];
	}
}
