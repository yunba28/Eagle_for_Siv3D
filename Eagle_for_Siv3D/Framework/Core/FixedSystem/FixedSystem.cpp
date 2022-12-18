#include "FixedSystem.hpp"

#include <Core/ExecutionOrder/ExecutionOrder.hpp>

namespace eagle::Internal
{
	double FixedSystem::sTimestep = 1.0 / 60.0;

	FixedSystem::FixedSystem()
		: mComponents()
		, mPendingComponents()
		, mNeedRemove(false)
		, mAccumulation(0)
	{
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

	void FixedSystem::SetTimestep(double _timestep) noexcept
	{
		sTimestep = _timestep;
	}

	double FixedSystem::GetTimestep() noexcept
	{
		return sTimestep;
	}
}
