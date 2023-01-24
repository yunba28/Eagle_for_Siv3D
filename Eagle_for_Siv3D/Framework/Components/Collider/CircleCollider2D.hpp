#pragma once

#include <Components/Collider/Collider2D.hpp>

namespace eagle
{
	class CircleCollider2D final : public Collider2D
	{
	public:

		void setupCircle(const Circle& _circle, const P2Material& _material = {}, const P2Filter& _filter = {});

		Circle getCircle()const;

	private:

		bool inRegion()const final override;

	private:

		Circle mCircle{};

	};
}
