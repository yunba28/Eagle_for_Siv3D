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

	template<>
	bool Load<CircleTrigger2D>(const String& path, CircleTrigger2D& collider)
	{
		if (FileSystem::Extension(path) != U"col")
			return false;

		const INI ini{ path };

		if (ini.isEmpty())
			return false;

		if (not ini.hasSection(U"CircleTrigger"))
			return false;

		Circle circle{};
		P2Filter filter{};

		try
		{
			if (ini.hasValue(U"CircleTrigger", U"LocalPos"))
			{
				circle.center = Parse<Vec2>(ini[U"CircleTrigger.LocalPos"]);
			}

			if (ini.hasValue(U"CircleTrigger", U"Radius"))
			{
				circle.r = Parse<double>(ini[U"CircleTrigger.Radius"]);
			}

			Collider2D::LoadFilter(ini, filter);
		}
		catch (ParseError&)
		{
			return false;
		}

		collider.setupCircle(circle, filter);

		return collider.loadProperties(ini);
	}
}
