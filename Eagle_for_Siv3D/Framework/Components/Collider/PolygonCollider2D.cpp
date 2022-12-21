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

	template<>
	bool Load<PolygonCollider2D>(const String& path, PolygonCollider2D& collider)
	{
		if (FileSystem::Extension(path) != U"col")
			return false;

		const INI ini{ path };

		if (ini.isEmpty())
			return false;

		if (not ini.hasSection(U"Polygon"))
			return false;

		Polygon polygon{};
		P2Material material{};
		P2Filter filter{};
		
		try
		{
			Array<Vec2> points{};

			for (auto& key : ini.getSection(U"Polygon").keys)
			{
				points << Parse<Vec2>(key.value);
			}

			polygon = Polygon{ points };

			Collider2D::LoadMaterial(ini, material);
			Collider2D::LoadFilter(ini, filter);
		}
		catch (ParseError&)
		{
			return false;
		}

		collider.setupPolygon(polygon, material, filter);

		return collider.loadProperties(ini);
	}
}
