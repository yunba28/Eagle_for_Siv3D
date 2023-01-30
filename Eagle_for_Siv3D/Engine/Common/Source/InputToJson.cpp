#include <Common/InputToJson.hpp>

namespace eagle
{
	JSON InputToJson(const Input& input)
	{
		JSON res;

		res[U"deviceType"] = (uint8)input.deviceType();
		res[U"code"] = input.code();
		res[U"playerIndex"] = input.playerIndex();

		return res;
	}

	JSON InputCombinationToJson(const InputCombination& inputCombination)
	{
		JSON res;

		res[U"left"][U"deviceType"] = (uint8)inputCombination.input1().deviceType();
		res[U"left"][U"code"] = inputCombination.input1().code();
		res[U"left"][U"playerIndex"] = inputCombination.input1().playerIndex();
		res[U"right"][U"deviceType"] = (uint8)inputCombination.input2().deviceType();
		res[U"right"][U"code"] = inputCombination.input2().code();
		res[U"right"][U"playerIndex"] = inputCombination.input2().playerIndex();

		return res;
	}

	JSON InputGroupToJson(const InputGroup& inputGroup)
	{
		JSON res;

		for (auto& input : inputGroup.inputs())
		{
			res[U"input"].push_back(InputToJson(input));
		}

		for (auto& inputCombination : inputGroup.inputCombinations())
		{
			res[U"inputCombination"].push_back(InputCombinationToJson(inputCombination));
		}

		return res;
	}

	Input JsonToInput(const JSON& json)
	{
		return Input
		{
			(InputDeviceType)json[U"deviceType"].get<uint8>(),
			json[U"code"].get<uint8>(),
			json[U"playerIndex"].get<uint8>()
		};
	}

	InputCombination JsonToInputCombination(const JSON& json)
	{
		return InputCombination
		{
			JsonToInput(json[U"left"]),
			JsonToInput(json[U"right"])
		};
	}

	InputGroup JsonToInputGroup(const JSON& json)
	{
		InputGroup inputGroup;

		if (json.hasElement(U"input"))
		{
			for (const auto& input : json[U"input"].arrayView())
			{
				inputGroup = inputGroup | JsonToInput(input);
			}
		}

		if (json.hasElement(U"inputCombination"))
		{
			for (const auto& inputCombination : json[U"inputCombination"].arrayView())
			{
				inputGroup = inputGroup | JsonToInputCombination(inputCombination);
			}
		}

		return inputGroup;
	}
}
