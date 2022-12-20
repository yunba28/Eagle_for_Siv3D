#pragma once

#include <Core/CoreFwd.hpp>
#include <Utility/SmartObject.hpp>

namespace eagle
{
	class Collider2D;
}

namespace eagle::Internal
{
	class FixedSystem final
	{
	public:

		FixedSystem();

		~FixedSystem();

		void update();

		void add(const WeakObject<Component>& _component);

		void add(const WeakObject<Collider2D>& _collider2D);

		void removeCollider(P2BodyID _id);

		static WeakObject<P2World> GetP2World()noexcept;

		static void SetTimestep(double _timestep)noexcept;

		static double GetTimestep()noexcept;

	private:

		void p2Update();

		void p2CollisionUpdate(P2ContactPair _pair, P2Collision _collision);

	private:

		Array<WeakObject<Component>> mComponents;

		Array<WeakObject<Component>> mPendingComponents;

		HashTable<P2BodyID, WeakObject<Collider2D>> mColliders2D;

		bool mNeedRemove;

		double mAccumulation;

		static SharedObject<P2World> sP2World;

		static double sTimestep;
	};
}
