#include "CircleCollider2D.hpp"
#include <Components/Transform.hpp>
#include <Utility/Load.hpp>

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

	template<>
	bool Load<CircleCollider2D>(const String& path, CircleCollider2D& collider)
	{
		if (FileSystem::Extension(path) != U"col")
			return false;

		const INI ini{ path };

		if (ini.isEmpty())
			return false;

		if (not ini.hasSection(U"Circle"))
			return false;

		Circle circle{};
		P2Material material{};
		P2Filter filter{};

		try
		{
			if (ini.hasValue(U"Circle",U"LocalPos"))
			{
				circle.center = Parse<Vec2>(ini[U"Circle.LocalPos"]);
			}

			if (ini.hasValue(U"Circle",U"Radius"))
			{
				circle.r = Parse<double>(ini[U"Circle.Radius"]);
			}

			Collider2D::LoadMaterial(ini, material);
			Collider2D::LoadFilter(ini, filter);
		}
		catch (ParseError&)
		{
			return false;
		}

		collider.setupCircle(circle, material, filter);

		return Collider2D::LoadProperties(ini, collider);
	}

	template<>
	bool Save<CircleCollider2D>(const String& path, CircleCollider2D& collider)
	{
		String out{path};

		if (FileSystem::Extension(out) != U"col")
		{
			auto it = std::find(out.begin(), out.end(), U'.');
			out.erase(it + 1, out.end());
			out += U"col";
		}

		INI ini{};

		ini.addSection(U"Circle");
		ini.write(U"Circle", U"LocalPos", collider.mCircle.center);
		ini.write(U"Circle", U"Radius", collider.mCircle.r);

		Collider2D::SaveMaterial(ini, collider.mP2Body.shape(0));
		Collider2D::SaveFilter(ini, collider.mP2Body.shape(0));
		Collider2D::SaveProperties(ini, collider);

		return ini.save(out);
	}
}
