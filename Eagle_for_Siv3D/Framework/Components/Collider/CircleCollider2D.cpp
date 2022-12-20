#include "CircleCollider2D.hpp"
#include <Components/Transform.hpp>

namespace eagle
{
	void CircleCollider2D::setupCircle(const Circle& _circle, const P2Material& _material, const P2Filter& _filter)
	{
		if (mP2Body.num_shapes() < 1)
		{
			mP2Body.addCircle(_circle, _material, _filter);
			mCircle = _circle;
		}
	}

	Circle CircleCollider2D::getCircle() const
	{
		return mCircle.movedBy(getTransform()->getWorldPos2D());
	}

	bool CircleCollider2D::inRegion() const 
	{
		auto camera = mCamera2D.lock();

		auto area = camera->getRegion().scaled(1.2);

		return area.intersects(getCircle());
	}
}
