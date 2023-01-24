#pragma once

namespace eagle
{
	class Uncopyable
	{
	public:
		Uncopyable() = default;
		~Uncopyable() = default;
		Uncopyable(const Uncopyable&) = delete;
		Uncopyable& operator=(const Uncopyable&) = delete;
	};
}