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
	inline HashTable<String, Internal::InputAxis_impl> InputAxis;
}
