#include "CircleTrigger2D.hpp"

#include <Components/Transform.hpp>

namespace eagle
{
	void CircleTrigger2D::setupCircle(const Circle& _circle, const P2Filter& _filter)
	{
		if (mP2Body.num_shapes() < 1)
		{
			mP2Body.addCircleSensor(_circle, _filter);
		}
	}

	Circle CircleTrigger2D::getCircle() const
	{
		return mCircle.movedBy(getTransform()->getWorldPos2D());
	}

	bool CircleTrigger2D::inRegion() const
	{
		auto camera = mCamera2D.lock();

		auto area = camera->getRegion().scaled(1.2);

		return area.intersects(getCircle());
	}
}
