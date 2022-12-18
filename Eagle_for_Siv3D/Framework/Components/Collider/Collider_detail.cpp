#include "Collider_detail.hpp"

namespace eagle
{
	Collision::operator bool() const noexcept
	{
		return (not actor.expired()) and (not collider.expired()) and (not normal.isZero());
	}

	ShapeType Collider::Shape::getShapeType() const noexcept
	{
		return mShape;
	}

	std::pair<Collision, Collision> Collider::Shape::intersect(const WeakObject<Shape>& _shape) const
	{
		auto shape = _shape.lock();

		if (not shape)
			return { Collision{},Collision{} };

		if (isEmpty() or shape->isEmpty())
			return { Collision{},Collision{} };

		auto a = getShapeType();
		auto b = shape->getShapeType();

		Optional<Array<Vec2>> point = Intersect(*this, *shape);

		

		return { Collision{},Collision{} };
	}

	Optional<Array<Vec2>> Collider::Shape::Intersect(const Shape& a, const Shape& b)
	{
		auto at = a.getShapeType();
		auto bt = b.getShapeType();

		if (at == ShapeType::Circle)
		{
			switch (bt)
			{
			case ShapeType::Circle:
				return Geometry2D::IntersectAt(a.unsafeGetShape<Circle>(), b.unsafeGetShape<Circle>());
				break;
			case ShapeType::Rect:
				return Geometry2D::IntersectAt(a.unsafeGetShape<Circle>(), b.unsafeGetShape<RectF>());
				break;
			case ShapeType::Quad:
				return Geometry2D::IntersectAt(a.unsafeGetShape<Circle>(), b.unsafeGetShape<Quad>());
				break;
			case ShapeType::Line:
				return Geometry2D::IntersectAt(a.unsafeGetShape<Circle>(), b.unsafeGetShape<Line>());
				break;
			case ShapeType::LineString:
				return Geometry2D::IntersectAt(a.unsafeGetShape<Circle>(), b.unsafeGetShape<LineString>());
				break;
			case ShapeType::Polygon:
				return Geometry2D::IntersectAt(a.unsafeGetShape<Circle>(), b.unsafeGetShape<Polygon>());
				break;
			default:
				break;
			}
		}

		return unspecified;
	}

	bool Collider::Shape::isEmpty() const noexcept
	{
		return mPtr == nullptr;
	}

	Collider::Shape::operator bool() const noexcept
	{
		return mPtr != nullptr;
	}
}
