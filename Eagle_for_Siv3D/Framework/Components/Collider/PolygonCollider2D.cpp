#include "PolygonCollider2D.hpp"
#include <Components/Transform.hpp>

namespace eagle
{
	void PolygonCollider2D::setupPolygon(const Polygon& _polygon, const P2Material& _material, const P2Filter& _filter)
	{
		if(mP2Body.num_shapes() < 1)
		{
			mP2Body.addPolygon(_polygon, _material, _filter);
			mPolygon = _polygon;
		}
	}

	Polygon PolygonCollider2D::getPolygon() const
	{
		return mPolygon.movedBy(getTransform()->getWorldPos2D());
	}

	bool PolygonCollider2D::inRegion() const
	{
		auto camera = mCamera2D.lock();

		auto area = camera->getRegion();

		return area.intersects(getPolygon().boundingRect());
	}
}
