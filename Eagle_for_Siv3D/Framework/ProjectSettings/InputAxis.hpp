#pragma once

#include <Siv3D.hpp>

namespace eagle::Internal
{
	class InputAxis_impl
	{
	public:

		InputAxis_impl& add(const Input& _input, double _scale);

		void remove(const Input& _input);

		double scale()const;

	private:

		struct Node
		{
			Input input;
			double scale;
			bool operator==(const Node& other)const;
		};

		Array<Node> mInputs;

	};
}

namespace eagle
{
	class InputSerializer final
	{
	public:

		InputSerializer() :mDeviceType(), mCode(), mPlayerIndex() {}

		InputSerializer(const Input& _input)
			:mDeviceType(static_cast<uint8>(_input.deviceType())), mCode(_input.code()), mPlayerIndex(_input.playerIndex()) {}

		Input asInput()const;

		template <class Archive>
		void SIV3D_SERIALIZE(Archive& archive)
		{
			archive(mDeviceType, mCode, mPlayerIndex);
		}

	private:
		uint8 mDeviceType{};
		uint8 mCode{};
		uint8 mPlayerIndex{};
	};

	inline HashTable<String, Internal::InputAxis_impl> InputAxis;
}
