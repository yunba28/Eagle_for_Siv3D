#include "ComponentSystem.hpp"

#include <Core/ExecutionOrder/ExecutionOrder.hpp>

namespace eagle::Internal
{
	ComponentArray::ComponentArray()
		: mComponents()
		, mPendingComponents()
		, mNeedRemove(false)
	{
	}

	void ComponentArray::update()
	{
		// 削除の必要があれば該当するComponentを削除する
		if (mNeedRemove)
		{
			mComponents.remove(nullptr);
			mNeedRemove = false;
		}

		// 新たなComponentを追加
		if (mPendingComponents)
		{
			mComponents.append(mPendingComponents);
			mPendingComponents.clear();
		}

		for (auto& component : mComponents)
		{
			// Componentが有効なら更新
			if (component and (not component->isPendingKill()))
			{
				if (component->isEnable())
				{
					component->_internalUpdate();
				}
			}
			else
			{
				component = nullptr;
				mNeedRemove = true;
			}
		}
	}

	void ComponentArray::lateUpdate()
	{
		for (auto& component : mComponents)
		{
			// Componentが有効なら更新
			if (component and (not component->isPendingKill()))
			{
				if (component->isEnable())
				{
					component->_internalLateUpdate();
				}
			}
			else
			{
				component = nullptr;
				mNeedRemove = true;
			}
		}
	}

	void ComponentArray::add(const SharedObject<Component>& _component)
	{
		mPendingComponents << _component;
	}

	WeakObject<Component> ComponentArray::getComponent(const WeakObject<Actor>& _actor) const
	{
		auto actor = _actor.lock();
		auto compare = [&actor](const SharedObject<Component>& component)
		{
			return component->getActor() == actor;
		};

		if (auto found = std::find_if(mComponents.begin(), mComponents.end(), compare); found != mComponents.end())
		{
			return (*found).weak();
		}

		return WeakObject<Component>{};
	}

	WeakObject<Component> ComponentArray::getComponentByTag(const String& _tag) const
	{
		auto compare = [&_tag](const SharedObject<Component>& component)
		{
			return component->compareTag(_tag);
		};

		if (auto found = std::find_if(mComponents.begin(), mComponents.end(), compare); found != mComponents.end())
		{
			return (*found).weak();
		}

		return WeakObject<Component>{};
	}

	Array<WeakObject<Component>> ComponentArray::getComponents() const
	{
		Array<WeakObject<Component>> result{ Arg::reserve = mComponents.size() };
		for (auto& component : mComponents)
		{
			result << component.weak();
		}
		return result;
	}

	Array<WeakObject<Component>> ComponentArray::getComponentsByTag(const String& _tag) const
	{
		auto compare = [&_tag](const SharedObject<Component>& component)
		{
			return component->compareTag(_tag);
		};

		if (auto tmp = mComponents.filter(compare); tmp)
		{
			Array<WeakObject<Component>> result{ Arg::reserve = tmp.size() };
			for (auto& component : tmp)
			{
				result << component.weak();
			}
			return result;
		}

		return Array<WeakObject<Component>>{};
	}

	size_t ComponentArray::count() const noexcept
	{
		return mComponents.size();
	}

	ComponentSystem::ComponentSystem()
		: mComponents()
		, mExecutionOrder()
	{
	}

	void ComponentSystem::update()
	{
		// 新たなComponent型を追加
		if (mPendingOrders)
		{
			mExecutionOrder.append(mPendingOrders);
			mPendingOrders.clear();

			auto compare = [](const std::type_index& a, const std::type_index& b)
			{
				return ExecutionOrder::Get(a) < ExecutionOrder::Get(b);
			};

			mExecutionOrder.stable_sort_by(compare);
		}

		for (auto& order : mExecutionOrder)
		{
			mComponents[order].update();
		}
	}

	void ComponentSystem::lateUpdate()
	{
		for (auto& order : mExecutionOrder)
		{
			mComponents[order].lateUpdate();
		}
	}

	void ComponentSystem::add(const SharedObject<Component>& _component, const std::type_index& _type)
	{
		// 新しい型の追加
		if (not contains(_type))
		{
			mComponents.emplace(_type, List_t{});
			mPendingOrders << _type;
		}

		mComponents[_type].add(_component);
	}

	WeakObject<Component> ComponentSystem::getComponent(const WeakObject<Actor>& _actor, const std::type_index& _type) const
	{
		if (contains(_type))
		{
			return mComponents.at(_type).getComponent(_actor);
		}

		return WeakObject<Component>{};
	}

	WeakObject<Component> ComponentSystem::getComponentByTag(const String& _tag, const std::type_index& _type) const
	{
		if (contains(_type))
		{
			return mComponents.at(_type).getComponentByTag(_tag);
		}

		return WeakObject<Component>{};
	}

	Array<WeakObject<Component>> ComponentSystem::getComponents(const std::type_index& _type) const
	{
		if (contains(_type))
		{
			return mComponents.at(_type).getComponents();
		}

		return Array<WeakObject<Component>>{};
	}

	Array<WeakObject<Component>> ComponentSystem::getComponentsByTag(const String& _tag, const std::type_index& _type) const
	{
		if (contains(_type))
		{
			return mComponents.at(_type).getComponentsByTag(_tag);
		}

		return Array<WeakObject<Component>>{};
	}

	size_t ComponentSystem::count(const std::type_index& _type) const
	{
		if (contains(_type))
		{
			return mComponents.at(_type).count();
		}

		return 0;
	}

	size_t ComponentSystem::count() const
	{
		size_t s{0};
		for (auto& [type, list] : mComponents)
		{
			s += list.count();
		}
		return s;
	}

	bool ComponentSystem::contains(const std::type_index& _type) const
	{
		return mComponents.contains(_type);
	}
}
