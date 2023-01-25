#include <Common/FixedUpdate.hpp>

#include <Siv3D/Scene.hpp>

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
		const auto delta = s3d::Scene::DeltaTime();
		for (mAccumulate += delta; mAccumulate >= mTimestep; mAccumulate -= mTimestep)
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
