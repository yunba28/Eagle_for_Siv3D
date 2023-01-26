#pragma once

#include <Siv3D/InputGroup.hpp>
#include <Siv3D/HashTable.hpp>

using s3d::Array;
using s3d::HashTable;
using s3d::String;
using s3d::Duration;
using s3d::Input;
using s3d::InputCombination;
using s3d::InputGroup;
using s3d::InputDeviceType;

using namespace std::chrono_literals;

namespace eagle
{
	enum class InputState
	{
		Down, Pressed, Up
	};

	namespace backend
	{
		class InputActionState final
		{
		public:

			struct Action
			{
				InputState state{ InputState::Down };
				Duration deadZone{ 0.0s };
			};

			struct Node
			{
				InputGroup input;
				Action action;
			};

		public:

			explicit InputActionState() = default;

			~InputActionState() = default;

			InputActionState& add(const InputGroup& _input, const Action& _action = Action{});

			InputActionState& remove(const InputGroup& _input);

			void clear();

			[[nodiscard]]
			bool action()const;

			[[nodiscard]]
			bool contains(const InputGroup& _input)const;

		private:

			Array<Node> mNodeList;

		};

		class InputActionDetail final
		{
		public:

			explicit InputActionDetail() = default;

			~InputActionDetail() = default;

			[[nodiscard]]
			bool operator()(const String& _actionName)const;

			InputActionState& operator[](const String& _actionName);

		private:

			HashTable<String, InputActionState> mInputStates;

		};
	}

	inline backend::InputActionDetail InputAction{};
}
