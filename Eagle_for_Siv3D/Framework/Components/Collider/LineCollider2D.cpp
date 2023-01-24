#include "LineCollider2D.hpp"
#include <Components/Transform.hpp>

namespace eagle
{
	void LineCollider2D::setupLine(const Line& _line, OneSided _oneSided, const P2Material& _material, const P2Filter& _filter)
	{
		if (mP2Body.num_shapes() < 1)
		{
			mP2Body.addLine(_line, _oneSided, _material, _filter);
			mLine = _line;
			mOneSided = _oneSided;
		}
	}

	Line LineCollider2D::getLine() const
	{
		return mLine.movedBy(getTransform()->getWorldPos2D());
	}

	bool LineCollider2D::inRegion() const
	{
		auto camera = mCamera2D.lock();

		auto region = camera->getRegion().scaled(1.2);

		return region.intersects(getLine().boundingRect());
	}
}
