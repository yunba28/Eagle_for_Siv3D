#include "Actor.hpp"
#pragma once

namespace eagle
{
	template<class T>
	WeakObject<T> Actor::attachComponent()requires(std::derived_from<T, Component>)
	{
		SharedObject<T> ptr = Component::Create<T>(mThis);
		mComponents << ptr.weak<Component>();
		return ptr.weak();
	}

	template<class T>
	WeakObject<T> Actor::getComponent()const requires(std::derived_from<T, Component>)
	{
		const std::type_index type{ typeid(T) };
		auto compare = [&type](const Component_ref& ref)
		{
			return ref.lock()->getType() == type;
		};
		if (auto found = std::find_if(mComponents.begin(), mComponents.end(), compare); found != mComponents.end())
		{
			return Cast<T>((*found));
		}
		return WeakObject<T>{};
	}

	template<class T>
	void Actor::detachComponent()requires(std::derived_from<T, Component>)
	{
		const std::type_index type{ typeid(T) };
		auto compare = [&type](const Component_ref& ref)
		{
			return ref.lock()->getType() == type;
		};
		if (auto found = std::find_if(mComponents.begin(), mComponents.end(), compare); found != mComponents.end())
		{
			Component::Destroy((*found));
			mComponents.erase(found);
		}
	}

	template<class T>
	SharedObject<T> Actor::Create(const Scene_ref& scene, const String& name)requires(std::derived_from<T, Actor>)
	{
		SharedObject<T> ptr{ MakeShared<T>() };
		Create_impl(scene, Cast<Actor>(ptr), typeid(T), name);
		return ptr;
	}

	template<class T, class TransformT>
	SharedObject<T> Actor::Create(const Scene_ref& scene, const String& name)requires(std::derived_from<T, Actor> or std::derived_from<TransformT,Transform>)
	{
		SharedObject<T> ptr{ MakeShared<T>() };
		SharedObject<Actor> actor{ Cast<Actor>(ptr) };
		CreateEmpty_impl(scene, Cast<Actor>(ptr), typeid(T), name);
		actor->mTransform = actor->attachComponent<TransformT>();
		return ptr;
	}
}
