#pragma once

#include <Core/CoreFwd.hpp>
#include <Utility/SmartObject.hpp>

namespace eagle
{
	class Collision final
	{
	public:

		ObjectHandle<Actor> getOpponent()const { return opponent.lock(); }

	public:

		WeakObject<Actor> opponent{};

		struct P2
		{
			WeakObject<class Collider2D> collider{};
			P2ContactPair pair{};
			Vec2 point{};
			Vec2 normal{};
			double normalImpulse{};
			double tangentImpulse{};
		}p2;

	};
}
