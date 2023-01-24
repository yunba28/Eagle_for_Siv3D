#pragma once

#include <Siv3D.hpp>

namespace eagle::Internal
{
	class InputAxis_helper;

	class InputAxis_impl final
	{
	public:

		struct Node
		{
			Input input;

			double scale;

			bool operator==(const Node& _other)const;
		};

	public:

		InputAxis_impl();

		~InputAxis_impl();

		void add(const Input& _input, double _scale);

		void add(const Node& _node);

		void remove(const Input& _input);

		void remove(const Node& _node);

		void clearInput();

		bool includes(const Input& _input)const;

		double value()const;

	private:

		Array<Node> mNodeList;

		friend class InputAxis_helper;

	};

	class InputAxis_helper final
	{
	public:

		InputAxis_helper& add(const String& _tag, const Input& _input, double _scale);

		InputAxis_helper& add(const String& _tag, const InputAxis_impl::Node& _node);

		void remove(const String& _tag);

		void remove(const String& _tag, const Input& _input);

		void remove(const String& _tag, const InputAxis_impl::Node& _node);

		void clearInput(const String& _tag);

		const InputAxis_impl& get(const String& _tag)const;

		double operator[](const String& _tag)const;

	private:

		HashTable<String, InputAxis_impl> mAxisMap;

	};
}

namespace eagle
{
	inline Internal::InputAxis_helper InputAxis{};
}
