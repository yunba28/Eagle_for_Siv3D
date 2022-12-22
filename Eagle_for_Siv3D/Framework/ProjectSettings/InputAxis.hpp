#pragma once

#include <Siv3D.hpp>
#include <Utility/Load.hpp>

namespace eagle::Internal
{
	class InputAxis_helper;

	class InputAxis_impl
	{
	private:

		struct Node
		{
			Input input{};

			double scale{};

			bool operator==(const Node& other)const;

			friend void Formatter(FormatData& formatData, const eagle::Internal::InputAxis_impl::Node& value)
			{
				formatData.string += U"({}, {}, {}, {})"_fmt(
					static_cast<uint8>(value.input.deviceType()),
					value.input.code(),
					value.input.playerIndex(),
					value.scale);
			}

			template <class CharType>
			friend std::basic_istream<CharType>& operator >>(std::basic_istream<CharType>& input, eagle::Internal::InputAxis_impl::Node& value)
			{
				CharType unused{};
				int64 elem[3]{};

				input >> unused
					>> elem[0] >> unused
					>> elem[1] >> unused
					>> elem[2] >> unused
					>> value.scale >> unused;

				Input inputData
				{
					static_cast<InputDeviceType>(elem[0]),
					static_cast<uint8>(elem[1]),
					static_cast<uint8>(elem[2])
				};

				value.input = inputData;

				return input;
			}
		};

	public:

		InputAxis_impl& add(const Input& _input, double _scale);

		void remove(const Input& _input);

		double scale()const;

	private:

		InputAxis_impl& add(const Node& _node);

	private:

		Array<Node> mInputs;

		friend class InputAxis_helper;

		template<class Type>
		friend bool eagle::Load(const String& path, Type& value);

		template<class Type>
		friend bool eagle::Save(const String& path, Type& value);

	};

	class InputAxis_helper final
	{
	public:

		InputAxis_helper() = default;

		~InputAxis_helper() = default;

		InputAxis_impl& add(const String& _name, const Input& _input, double _scale);

		void remove(const String& _name);

		InputAxis_impl& get(const String& _name);

		double operator[](const String& _name)const;

	private:

		InputAxis_impl& add(const String& _name, const InputAxis_impl::Node& _node);

	private:

		HashTable<String, InputAxis_impl> mInputAxisTable{};

		template<class Type>
		friend bool eagle::Load(const String& path, Type& value);

		template<class Type>
		friend bool eagle::Save(const String& path, Type& value);

	};
}

namespace eagle
{
	inline Internal::InputAxis_helper InputAxis{};
}
