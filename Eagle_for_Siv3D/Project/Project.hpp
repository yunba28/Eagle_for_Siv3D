#pragma once

#include <Siv3D.hpp>
#include <Eagle.hpp>

namespace Project
{
	using GameWorld = eagle::GameWorld<String, void>;

	using eagle::SharedObject;
	using eagle::WeakObject;

	using eagle::InputAction;
	using eagle::InputAxis;
	using eagle::InputState;
	using eagle::PadState;

	using XController = s3d::detail::XInput_impl;
}
