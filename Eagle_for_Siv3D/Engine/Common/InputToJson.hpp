#pragma once

#include <Siv3D/InputGroup.hpp>
#include <Siv3D/JSON.hpp>

using s3d::Input;
using s3d::InputCombination;
using s3d::InputGroup;
using s3d::InputDeviceType;
using s3d::JSON;

namespace eagle
{
	JSON InputToJson(const Input& input);

	JSON InputCombinationToJson(const InputCombination& inputCombination);

	JSON InputGroupToJson(const InputGroup& inputGroup);

	Input JsonToInput(const JSON& json);

	InputCombination JsonToInputCombination(const JSON& json);

	InputGroup JsonToInputGroup(const JSON& json);
}
