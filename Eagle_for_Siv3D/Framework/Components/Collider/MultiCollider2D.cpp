#include "MultiCollider2D.hpp"
#include <Utility/Load.hpp>

namespace eagle
{
	void MultiCollider2D::addCircle(const Circle& _circle, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addCircle(_circle, _material, _filter);
		mShapeTypes << ShapeType::Circe;
	}

	void MultiCollider2D::addCircleTrigger(const Circle& _circle, const P2Filter& _filter)
	{
		mP2Body.addCircleSensor(_circle, _filter);
		mShapeTypes << ShapeType::CircleTrigger;
	}

	void MultiCollider2D::addRect(const RectF& _rect, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addRect(_rect, _material, _filter);
		mShapeTypes << ShapeType::Rect;
	}

	void MultiCollider2D::addLine(const Line& _line, OneSided _onesided, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addLine(_line, _onesided, _material, _filter);
		mShapeTypes << ShapeType::Line;
	}

	void MultiCollider2D::addPolygon(const Polygon& _polygon, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addPolygon(_polygon, _material, _filter);
		mShapeTypes << ShapeType::Polygon;
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

	Array<String> MultiCollider2D::SaveShape(const String& path, MultiCollider2D& collider)
	{
		const size_t shapeCount = collider.getShapeCount();
		Array<String> shapes{Arg::reserve = shapeCount};

		const auto& body = collider.mP2Body;
		const auto& types = collider.mShapeTypes;

		const Vec2 offset{ -body.getPos() };

		for (size_t i = 0; i < shapeCount; ++i)
		{
			INI ini{};

			if (types[i] == ShapeType::Circe)
			{
				SaveCircle(ini, *body.as<P2Circle>(i), offset);
			}
			else if (types[i] == ShapeType::CircleTrigger)
			{
				SaveCircleTrigger(ini, *body.as<P2Circle>(i), offset);
			}
			else if (types[i] == ShapeType::Rect)
			{
				SaveRect(ini, *body.as<P2Rect>(i), offset);
			}
			else if (types[i] == ShapeType::Line)
			{
				SaveLine(ini, *body.as<P2Line>(i), offset);
			}
			else if (types[i] == ShapeType::Polygon)
			{
				SavePolygon(ini, *body.as<P2Polygon>(i), offset);
			}

			auto out = path + U"/shape{}.col"_fmt(i);
			if (ini.save(out))
			{
				shapes << FileSystem::RelativePath(out);
			}
		}

		return shapes;
	}

	bool MultiCollider2D::SaveCircle(INI& ini, const P2Circle& shape, Vec2 offset)
	{
		ini.addSection(U"Circle");

		auto circle = shape.getCircle();

		ini.write(U"Circle", U"LocalPos", circle.center.movedBy(offset));
		ini.write(U"Circle", U"Radius", circle.r);

		Collider2D::SaveMaterial(ini, shape);
		Collider2D::SaveFilter(ini, shape);

		return true;
	}

	bool MultiCollider2D::SaveCircleTrigger(INI& ini, const P2Circle& shape, Vec2 offset)
	{
		ini.addSection(U"CircleTrigger");

		auto circle = shape.getCircle();

		ini.write(U"CircleTrigger", U"LocalPos", circle.center.movedBy(offset));
		ini.write(U"CircleTrigger", U"Radius", circle.r);

		Collider2D::SaveFilter(ini, shape);

		return true;
	}

	bool MultiCollider2D::SaveRect(INI& ini, const P2Rect& shape, Vec2 offset)
	{
		ini.addSection(U"Rect");

		auto rect = shape.getQuad().boundingRect();

		ini.write(U"Rect", U"LocalPos", rect.pos.movedBy(offset));
		ini.write(U"Rect", U"Size", rect.size);

		Collider2D::SaveMaterial(ini, shape);
		Collider2D::SaveFilter(ini, shape);

		return true;
	}

	bool MultiCollider2D::SaveLine(INI& ini, const P2Line& shape, Vec2 offset)
	{
		ini.addSection(U"Line");

		auto line = shape.getLine();

		ini.write(U"Line", U"LocalBegin", line.begin.movedBy(offset));
		ini.write(U"Line", U"LocalEnd", line.end.movedBy(offset));

		Collider2D::SaveMaterial(ini, shape);
		Collider2D::SaveFilter(ini, shape);

		return true;
	}

	bool MultiCollider2D::SavePolygon(INI& ini, const P2Polygon& shape, Vec2 offset)
	{
		ini.addSection(U"Polygon");

		const auto& polygon = shape.getPolygon();
		const auto& outer = polygon.outer();

		for (int32 i = 0; i < outer.size(); ++i)
		{
			ini.write(U"Polygon", U"Elem{}"_fmt(i), outer[i].movedBy(offset));
		}

		Collider2D::SaveMaterial(ini, shape);
		Collider2D::SaveFilter(ini, shape);

		return true;
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

		return Collider2D::LoadProperties(ini, collider);
	}

	template<>
	bool Save<MultiCollider2D>(const String& path, MultiCollider2D& collider)
	{
		String out{ path };

		if (FileSystem::Extension(out) != U"mcol")
		{
			auto it = std::find(out.begin(), out.end(), U'.');
			out.erase(it + 1, out.end());
			out += U"mcol";
		}

		INI ini{};

		ini.addSection(U"Shapes");

		auto directory = FileSystem::ParentPath(out) + U"{}Shapes"_fmt(FileSystem::BaseName(out));
		auto shapes = MultiCollider2D::SaveShape(directory, collider);
		for (int32 i = 0;i < shapes.size();++i)
		{
			ini.write(U"Shapes", U"Shape{}"_fmt(i), shapes[i]);
		}

		Collider2D::SaveProperties(ini, collider);

		return ini.save(out);
	}
}
