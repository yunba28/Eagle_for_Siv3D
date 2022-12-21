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

	template<>
	bool Load<LineCollider2D>(const String& path, LineCollider2D& collider)
	{
		if (FileSystem::Extension(path) != U"col")
			return false;

		const INI ini{ path };

		if (ini.isEmpty())
			return false;

		if (not ini.hasSection(U"Line"))
			return false;

		Line line{};
		P2Material material{};
		P2Filter filter{};
		OneSided oneSided{ false };

		try
		{
			if (ini.hasValue(U"Line", U"LocalBegin"))
			{
				line.begin = Parse<Vec2>(ini[U"Line.LocalBegin"]);
			}

			if (ini.hasValue(U"Line", U"LocalEnd"))
			{
				line.end = Parse<Vec2>(ini[U"Line.LocalEnd"]);
			}

			if (ini.hasValue(U"Line", U"OneSided"))
			{
				oneSided = YesNo<OneSided_tag>{ Parse<bool>(ini[U"Line.OneSided"]) };
			}

			Collider2D::LoadMaterial(ini, material);
			Collider2D::LoadFilter(ini, filter);
		}
		catch (ParseError&)
		{
			return false;
		}

		collider.setupLine(line, oneSided, material, filter);

		return collider.loadProperties(ini);
	}
}
