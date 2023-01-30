#pragma once

#include <Common/InputToJson.hpp>
#include <Siv3D/HashTable.hpp>

using s3d::Array;
using s3d::HashTable;
using s3d::String;

namespace eagle
{
	enum class PadState : uint8
	{
		LThumbX,
		LThumbY,
		RThumbX,
		RThumbY,
	};

	namespace backend
	{
		class InputAxisState final
		{
		public:

			struct Node
			{
				InputGroup input;
				double scale;
			};

			struct Pad
			{
				PadState state{ PadState::LThumbX };
				double scale{ 1.0 };
			};

		public:

			explicit InputAxisState() = default;

			~InputAxisState() = default;

			InputAxisState& add(const InputGroup& _input, double _scale);

			InputAxisState& add(const Pad& _pad);

			InputAxisState& remove(const InputGroup& _input);

			InputAxisState& remove(const PadState& _padState);

			void clear();

			[[nodiscard]]
			double axis(uint8 _playerIndex = 0)const;

			[[nodiscard]]
			bool contains(const InputGroup& _input)const;

			[[nodiscard]]
			bool contains(const PadState& _padState)const;

			JSON toJson()const;

			void fromJson(const JSON& _json);

		private:

			Array<Node> mNodeList;

			Array<Pad> mPadList;

		};

		class InputAxisDetail final
		{
		public:

			explicit InputAxisDetail() = default;

			~InputAxisDetail() = default;

			[[nodiscard]]
			double operator()(const String& _axisName, uint8 _playerIndex = 0)const;

			InputAxisState& operator[](const String& _axisName);

			JSON toJson()const;

			void fromJson(const JSON& _json);

		private:

			HashTable<String, InputAxisState> mInputStates;

		};
	}

	inline backend::InputAxisDetail InputAxis{};
}
