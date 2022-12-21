#pragma once

#include <Components/Collider/Collider2D.hpp>

namespace eagle
{
	class MultiCollider2D : public Collider2D
	{
	public:

		void addCircle(const Circle& _circle, const P2Material& _material = {}, const P2Filter& _filter = {});

		void addCircleTrigger(const Circle& _circle, const P2Filter& _filter = {});

		void addRect(const RectF& _rect, const P2Material& _material = {}, const P2Filter& _filter = {});

		void addLine(const Line& _line, OneSided _onesided, const P2Material& _material = {}, const P2Filter& _filter = {});

		void addPolygon(const Polygon& _polygon, const P2Material& _material = {}, const P2Filter& _filter = {});

		void setFilter(P2Filter _filter, size_t _index);

		void setDensity(double _density, size_t _index);

		void setFriction(double _friction, size_t _index);

		void setRestitution(double _restitution, size_t _index);

		void setRestitutionThreshold(double _threshold, size_t _index);

		size_t getShapeCount()const;

	private:

		bool inRegion()const final override;

		static bool LoadShape(const String& path, MultiCollider2D& collider);
		static bool LoadCircle(const INI& ini, MultiCollider2D& collider);
		static bool LoadCircleTrigger(const INI& ini, MultiCollider2D& collider);
		static bool LoadRect(const INI& ini, MultiCollider2D& collider);
		static bool LoadLine(const INI& ini, MultiCollider2D& collider);
		static bool LoadPolygon(const INI& ini, MultiCollider2D& collider);

		static Array<String> SaveShape(const String& path, MultiCollider2D& collider);
		static bool SaveCircle(INI& ini, const P2Circle& shape, Vec2 offset);
		static bool SaveCircleTrigger(INI& ini, const P2Circle& shape, Vec2 offset);
		static bool SaveRect(INI& ini, const P2Rect& shape, Vec2 offset);
		static bool SaveLine(INI& ini, const P2Line& shape, Vec2 offset);
		static bool SavePolygon(INI& ini, const P2Polygon& shape, Vec2 offset);

	private:

		Array<ShapeType> mShapeTypes;

		template<class Type>
		friend bool Load(const String& path, Type& collider);

		template<class Type>
		friend bool Save(const String& path, Type& collider);
	};
}
