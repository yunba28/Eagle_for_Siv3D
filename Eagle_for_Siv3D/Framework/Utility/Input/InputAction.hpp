#pragma once

#include <Siv3D.hpp>

namespace eagle::Internal
{
	class InputAction_helper;

	class InputAction_impl final
	{
	public:

		InputAction_impl();

		~InputAction_impl();

		void add(const InputGroup& _action);

		void remove(const InputGroup& _action);

		bool includes(const InputGroup& _action);

		bool down()const;

		bool pressed()const;

		bool up()const;

		void clearInput();

	private:

		Array<InputGroup> mActionList;

		friend class InputAction_helper;

	};

	class InputAction_helper final
	{
	public:

		InputAction_helper& add(const String& _tag, const InputGroup& _action);

		void remove(const String& _tag);

		void remove(const String& _tag, const InputGroup& _action);

		const InputAction_impl& get(const String& _tag)const;

		bool down(const String& _tag)const;

		bool pressed(const String& _tag)const;

		bool up(const String& _tag)const;

		void clearInput(const String& _tag);

	private:

		HashTable<String, InputAction_impl> mActionMap;

	};
}

namespace eagle
{
	inline Internal::InputAction_helper InputAction{};
}
