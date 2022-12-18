#pragma once

#include <Core/Component/Component.hpp>
#include <Utility/SmartObject.hpp>

namespace eagle
{
	enum class ShapeType
	{
		None=0,
		Circle,
		Rect,
		Quad,
		Line,
		LineString,
		Polygon
	};

	class Collider;

	struct Collision
	{
		WeakObject<Actor> actor{};
		WeakObject<Collider> collider{};
		Vec2 point{ 0,0 };
		Vec2 normal{ 0,0 };
		explicit operator bool()const noexcept;
	};

	class Collider : public Component
	{
	public:

		class Shape
		{
		public:

			template<class ShapeT> void setShape(const ShapeT&) = delete;
			template<> void setShape<Circle>(const Circle& _shape);
			template<> void setShape<RectF>(const RectF& _shape);
			template<> void setShape<Quad>(const Quad& _shape);
			template<> void setShape<Line>(const Line& _shape);
			template<> void setShape<LineString>(const LineString& _shape);
			template<> void setShape<Polygon>(const Polygon& _shape);

			template<class ShapeT> const ShapeT& getShape()const = delete;
			template<> const Circle& getShape()const;
			template<> const RectF& getShape()const;
			template<> const Quad& getShape()const;
			template<> const Line& getShape()const;
			template<> const LineString& getShape()const;
			template<> const Polygon& getShape()const;

			template<class ShapeT> const ShapeT& unsafeGetShape()const;

			ShapeType getShapeType()const noexcept;

			std::pair<Collision, Collision> intersect(const WeakObject<Shape>& _shape)const;

			bool isEmpty()const noexcept;

			explicit operator bool()const noexcept;

		private:

			static Optional<Array<Vec2>> Intersect(const Shape& a, const Shape& b);

		private:
			void* mPtr{ nullptr };
			ShapeType mShape{ ShapeType::None };
		};

		

	public:

		Collider();

		virtual ~Collider();

		void fixedUpdate()override;

	};
}

#include "Collider_detail.ipp"
