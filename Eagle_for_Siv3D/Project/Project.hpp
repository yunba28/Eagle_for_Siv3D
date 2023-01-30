#pragma once

#include <Siv3D.hpp>
#include <Eagle.hpp>

using GameWorld = eagle::World<String, void>;

using eagle::SharedObject;
using eagle::WeakObject;

using eagle::InputAction;
using eagle::InputAxis;
using eagle::InputState;
using eagle::PadState;

using XController = s3d::detail::XInput_impl;
