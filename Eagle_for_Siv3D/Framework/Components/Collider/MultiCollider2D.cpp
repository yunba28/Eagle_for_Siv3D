#include "MultiCollider2D.hpp"

namespace eagle
{
	void MultiCollider2D::addCircle(const Circle& _circle, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addCircle(_circle, _material, _filter);
		mShapeTypes << typeid(P2Circle);
	}

	void MultiCollider2D::addCircleTrigger(const Circle& _circle, const P2Filter& _filter)
	{
		mP2Body.addCircleSensor(_circle, _filter);
		mShapeTypes << typeid(P2Circle);
	}

	void MultiCollider2D::addRect(const RectF& _rect, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addRect(_rect, _material, _filter);
		mShapeTypes << typeid(P2Rect);
	}

	void MultiCollider2D::addLine(const Line& _line, OneSided _onesided, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addLine(_line, _onesided, _material, _filter);
		mShapeTypes << typeid(P2Line);
	}

	void MultiCollider2D::addPolygon(const Polygon& _polygon, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addPolygon(_polygon, _material, _filter);
		mShapeTypes << typeid(P2Polygon);
	}

	void MultiCollider2D::setFilter(P2Filter _filter, size_t _index)
	{
		mP2Body.shape(_index).setFilter(_filter);
	}

	void MultiCollider2D::setDensity(double _density, size_t _index)
	{
		mP2Body.shape(_index).setDensity(_density);
	}

	void MultiCollider2D::setFriction(double _friction, size_t _index)
	{
		mP2Body.shape(_index).setFriction(_friction);
	}

	void MultiCollider2D::setRestitution(double _restitution, size_t _index)
	{
		mP2Body.shape(_index).setRestitution(_restitution);
	}

	void MultiCollider2D::setRestitutionThreshold(double _threshold, size_t _index)
	{
		mP2Body.shape(_index).setRestitutionThreshold(_threshold);
	}

	size_t MultiCollider2D::getShapeCount() const
	{
		return mP2Body.num_shapes();
	}

	bool MultiCollider2D::inRegion() const
	{
		return true;
	}

	bool MultiCollider2D::LoadShape(const String& path, MultiCollider2D& collider)
	{
		if (FileSystem::Extension(path) != U"col")
			return false;

		const INI ini{ path };

		if (ini.isEmpty())
			return false;

		if (ini.hasSection(U"Circle"))
		{
			return LoadCircle(ini, collider);
		}
		else if (ini.hasSection(U"CircleTrigger"))
		{
			return LoadCircleTrigger(ini, collider);
		}
		else if (ini.hasSection(U"Rect"))
		{
			return LoadRect(ini, collider);
		}
		else if (ini.hasSection(U"Line"))
		{
			return LoadLine(ini, collider);
		}
		else if (ini.hasSection(U"Polygon"))
		{
			return LoadPolygon(ini, collider);
		}
		else
		{
			return false;
		}
	}

	bool MultiCollider2D::LoadCircle(const INI& ini, MultiCollider2D& collider)
	{
		Circle circle{};
		P2Material material{};
		P2Filter filter{};

		try
		{
			if (ini.hasValue(U"Circle", U"LocalPos"))
			{
				circle.center = Parse<Vec2>(ini[U"Circle.LocalPos"]);
			}

			if (ini.hasValue(U"Circle", U"Radius"))
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

		collider.addCircle(circle, material, filter);

		return true;
	}

	bool MultiCollider2D::LoadCircleTrigger(const INI& ini, MultiCollider2D& collider)
	{
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

		collider.addCircleTrigger(circle, filter);

		return true;
	}

	bool MultiCollider2D::LoadRect(const INI& ini, MultiCollider2D& collider)
	{
		RectF rect{};
		P2Material material{};
		P2Filter filter{};

		try
		{
			if (ini.hasValue(U"Rect", U"LocalPos"))
			{
				rect.pos = Parse<Vec2>(ini[U"Rect.LocalPos"]);
			}

			if (ini.hasValue(U"Rect", U"Size"))
			{
				rect.size = Parse<Vec2>(ini[U"Rect.Size"]);
			}

			Collider2D::LoadMaterial(ini, material);
			Collider2D::LoadFilter(ini, filter);
		}
		catch (ParseError&)
		{
			return false;
		}

		collider.addRect(rect, material, filter);

		return true;
	}

	bool MultiCollider2D::LoadLine(const INI& ini, MultiCollider2D& collider)
	{
		Line line{};
		OneSided oneSided{ false };
		P2Material material{};
		P2Filter filter{};

		try
		{
			if (ini.hasValue(U"Line", U"LocalBegin"))
			{
				line.begin = Parse<Vec2>(ini[U"Line.LocalBegin"]);
			}

			if (ini.hasValue(U"Line", U"localEnd"))
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

		collider.addLine(line, oneSided, material, filter);

		return true;
	}

	bool MultiCollider2D::LoadPolygon(const INI& ini, MultiCollider2D& collider)
	{
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

		collider.addPolygon(polygon, material, filter);

		return false;
	}

	template<>
	bool Load<MultiCollider2D>(const String& path, MultiCollider2D& collider)
	{
		if (FileSystem::Extension(path) != U"mcol")
			return false;

		const INI ini{ path };

		if (ini.isEmpty())
			return false;

		if (not ini.hasSection(U"Shapes"))
			return false;

		for (const auto& key : ini.getSection(U"Shapes").keys)
		{
			MultiCollider2D::LoadShape(key.value, collider);
		}

		return collider.loadProperties(ini);
	}
}
