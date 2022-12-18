#pragma once

namespace eagle
{
	template<> void Collider::Shape::setShape(const Circle& _shape)
	{
		if (mPtr)
			delete mPtr;
		mPtr = (void*)new Circle{ _shape };
		mShape = ShapeType::Circle;
	}

	template<> void Collider::Shape::setShape(const RectF& _shape)
	{
		if (mPtr)
			delete mPtr;
		mPtr = (void*)new RectF{ _shape };
		mShape = ShapeType::Rect;
	}

	template<> void Collider::Shape::setShape(const Quad& _shape)
	{
		if (mPtr)
			delete mPtr;
		mPtr = (void*)new Quad{ _shape };
		mShape = ShapeType::Quad;
	}

	template<> void Collider::Shape::setShape(const Line& _shape)
	{
		if (mPtr)
			delete mPtr;
		mPtr = (void*)new Line{ _shape };
		mShape = ShapeType::Line;
	}

	template<> void Collider::Shape::setShape(const LineString& _shape)
	{
		if (mPtr)
			delete mPtr;
		mPtr = (void*)new LineString{ _shape };
		mShape = ShapeType::LineString;
	}

	template<> void Collider::Shape::setShape(const Polygon& _shape)
	{
		if (mPtr)
			delete mPtr;
		mPtr = (void*)new Polygon{ _shape };
		mShape = ShapeType::Polygon;
	}

	template<> const Circle& Collider::Shape::getShape()const
	{
		if (mPtr and mShape == ShapeType::Circle)
		{
			return unsafeGetShape<Circle>();
		}

		throw Error{ U"Collider::Shape : bad cast." };
	}

	template<> const RectF& Collider::Shape::getShape()const
	{
		if (mPtr and mShape == ShapeType::Rect)
		{
			return unsafeGetShape<RectF>();
		}

		throw Error{ U"Collider::Shape : bad cast." };
	}

	template<> const Quad& Collider::Shape::getShape()const
	{
		if (mPtr and mShape == ShapeType::Quad)
		{
			return unsafeGetShape<Quad>();
		}

		throw Error{ U"Collider::Shape : bad cast." };
	}

	template<> const Line& Collider::Shape::getShape()const
	{
		if (mPtr and mShape == ShapeType::Line)
		{
			return unsafeGetShape<Line>();
		}

		throw Error{ U"Collider::Shape : bad cast." };
	}

	template<> const LineString& Collider::Shape::getShape()const
	{
		if (mPtr and mShape == ShapeType::LineString)
		{
			return unsafeGetShape<LineString>();
		}

		throw Error{ U"Collider::Shape : bad cast." };
	}

	template<> const Polygon& Collider::Shape::getShape()const
	{
		if (mPtr and mShape == ShapeType::Polygon)
		{
			return unsafeGetShape<Polygon>();
		}

		throw Error{ U"Collider::Shape : bad cast." };
	}

	template<class ShapeT> const ShapeT& Collider::Shape::unsafeGetShape()const
	{
		return *reinterpret_cast<ShapeT*>(mPtr);
	}
}
