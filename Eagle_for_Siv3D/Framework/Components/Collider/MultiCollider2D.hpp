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

	private:

	};
}
