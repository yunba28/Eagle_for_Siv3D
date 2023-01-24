#include <Classes/FixedUpdate.hpp>

namespace eagle
{
	FixedUpdate::FixedUpdate(const std::function<void()>& _update)
		: mUpdate(_update)
		, mAccumulate(0)
		, mTimestep(1.0 / 60.0)
	{
	}

	FixedUpdate::~FixedUpdate()
	{
	}

	void FixedUpdate::update()
	{
		const auto delta = 0.0166;
		for (; mAccumulate <= mTimestep; mAccumulate -= delta)
		{
			mUpdate();
		}
	}

	void FixedUpdate::setTimestep(double _timestep) noexcept
	{
		mTimestep = _timestep;
	}

	double FixedUpdate::getTimestep() const noexcept
	{
		return mTimestep;
	}
}