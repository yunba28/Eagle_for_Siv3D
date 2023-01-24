#pragma once

namespace eagle
{
	template<class Type>
	struct EqualTo final
	{
		constexpr EqualTo(const Type _value) :value(_value) {}

		constexpr bool operator()(const Type _value)const
		{
			return _value == value;
		}

		const Type value;
	};

	template<class Type>
	struct LessTo final
	{
		constexpr LessTo(const Type _value) :value(_value) {}

		constexpr bool operator()(const Type _value)const
		{
			return _value < value;
		}

		const Type value;
	};

	template<class Type>
	struct GreaterTo final
	{
		constexpr GreaterTo(const Type _value) :value(_value) {}

		constexpr bool operator()(const Type _value)const
		{
			return _value > value;
		}

		const Type value;
	};
}
