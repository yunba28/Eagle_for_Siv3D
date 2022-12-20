#pragma once

#include <Components/Collider/Collider2D.hpp>

namespace eagle
{
	class PolygonCollider2D final : public Collider2D
	{
	public:

		void setupPolygon(const Polygon& _polygon, const P2Material& _material = {}, const P2Filter& _filter = {});

		Polygon getPolygon()const;

	private:

		bool inRegion()const final override;

		Polygon mPolygon;
	};
}
