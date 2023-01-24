#pragma once

#include <functional>

namespace eagle
{
	class FixedUpdate final
	{
	public:

		FixedUpdate(const std::function<void()>& _update);

		~FixedUpdate();

		void update();

		void setTimestep(double _timestep)noexcept;

		double getTimestep()const noexcept;

	private:

		std::function<void()> mUpdate;

		double mAccumulate;

		double mTimestep;

	};
}