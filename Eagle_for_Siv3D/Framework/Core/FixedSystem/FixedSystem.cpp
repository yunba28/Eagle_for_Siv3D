#include "FixedSystem.hpp"

#include <Core/ExecutionOrder/ExecutionOrder.hpp>
#include <Components/Collider/Collider2D.hpp>

namespace eagle::Internal
{
	SharedObject<P2World> FixedSystem::sP2World = nullptr;

	double FixedSystem::sTimestep = 1.0 / 60.0;

	FixedSystem::FixedSystem()
		: mComponents()
		, mPendingComponents()
		, mNeedRemove(false)
		, mAccumulation(0)
	{
		if (not sP2World)
		{
			sP2World = MakeShared<P2World>(980);
		}
	}

	FixedSystem::~FixedSystem()
	{
		sP2World.reset();
	}

	void FixedSystem::update()
	{
		for (mAccumulation += Scene::DeltaTime();mAccumulation >= sTimestep;mAccumulation -= sTimestep)
		{
			// Componentの削除
			if (mNeedRemove)
			{
				mComponents.remove(WeakObject<Component>{});
				mNeedRemove = false;
			}

			// Componentの追加
			if (mPendingComponents)
			{
				mComponents.append(mPendingComponents);
				mPendingComponents.clear();

				auto compare = [](const WeakObject<Component>& a, const WeakObject<Component>& b)
				{
					auto aHand = a.lock();
					auto bHand = b.lock();
					return ExecutionOrder::Get(aHand->getType()) < ExecutionOrder::Get(bHand->getType());
				};

				mComponents.stable_sort_by(compare);
			}

			p2Update();

			for (auto& ref : mComponents)
			{
				auto component = ref.lock();

				// Componentが有効なら更新
				if (component and (not component->isPendingKill()))
				{
					if (component->isEnable())
					{
						component->_internalFixedUpdate();
					}
				}
				else
				{
					ref.reset();
					mNeedRemove = true;
				}
			}
		}
	}

	void FixedSystem::add(const WeakObject<Component>& _component)
	{
		mPendingComponents << _component;
	}

	void FixedSystem::add(const WeakObject<Collider2D>& _collider2D)
	{
		auto collider = _collider2D.lock();
		auto id = collider->id();
		if (not mColliders2D.contains(id))
		{
			mColliders2D.emplace(id, _collider2D);
		}
	}

	void FixedSystem::removeCollider(P2BodyID _id)
	{
		if (mColliders2D.contains(_id))
		{
			mColliders2D.erase(_id);
		}
	}

	WeakObject<P2World> FixedSystem::GetP2World() noexcept
	{
		return sP2World.weak();
	}

	void FixedSystem::SetTimestep(double _timestep) noexcept
	{
		sTimestep = _timestep;
	}

	double FixedSystem::GetTimestep() noexcept
	{
		return sTimestep;
	}

	void FixedSystem::p2Update()
	{
		sP2World->update(sTimestep);

		const auto& collisions = sP2World->getCollisions();

		for (const auto& [pair, collision] : collisions)
		{
			p2CollisionUpdate(pair, collision);
		}
	}

	void FixedSystem::p2CollisionUpdate(P2ContactPair _pair, P2Collision _collision)
	{
		const auto& wa = mColliders2D[_pair.a];
		const auto& wb = mColliders2D[_pair.b];
		auto ha = wa.lock();
		auto hb = wb.lock();

		Collision collisionA{};
		collisionA.opponent = hb->getActor().weak();
		collisionA.p2.pair = _pair;
		collisionA.p2.collider = wb;
		collisionA.p2.normal = -_collision.normal();

		Collision collisionB{};
		collisionB.opponent = ha->getActor().weak();
		collisionB.p2.pair = { _pair.b,_pair.a };
		collisionB.p2.collider = wa;
		collisionB.p2.normal = _collision.normal();

		if (_collision.hasContacts())
		{
			const auto& contact = _collision.contact(0);

			collisionA.p2.point = contact.point;
			collisionA.p2.normalImpulse = contact.normalImpulse;
			collisionA.p2.tangentImpulse = contact.tangentImpulse;

			collisionB.p2.point = contact.point;
			collisionB.p2.normalImpulse = contact.normalImpulse;
			collisionB.p2.tangentImpulse = contact.tangentImpulse;
		}

		ha->onCollision(collisionA);
		hb->onCollision(collisionB);
	}
}
