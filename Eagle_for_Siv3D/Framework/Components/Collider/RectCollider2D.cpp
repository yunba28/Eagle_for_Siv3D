#include "RectCollider2D.hpp"

#include <Components/Transform.hpp>

namespace eagle
{
	void RectCollider2D::setupRect(const RectF& _rect, const P2Material& _material, const P2Filter& _filter)
	{
		if (mP2Body.num_shapes() < 1)
		{
			mP2Body.addRect(_rect, _material, _filter);
		}
	}

	RectF RectCollider2D::getRect() const
	{
		return mRect.movedBy(getTransform()->getWorldPos2D());
	}

	bool RectCollider2D::inRegion() const
	{
		auto camera = mCamera2D.lock();

		auto area = camera->getRegion().scaled(1.2);

		return area.intersects(getRect());
	}
}
