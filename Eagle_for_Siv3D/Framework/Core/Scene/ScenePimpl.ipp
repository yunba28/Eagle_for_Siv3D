#pragma once

namespace eagle::Internal
{
	template<class T>
	WeakObject<T> ScenePimpl::createActor(const String& _name)requires(std::derived_from<T, Actor>)
	{
		return Actor::Create<T>(mThis, _name).weak();
	}

	template<class T>
	WeakObject<T> ScenePimpl::getActor(const String& _name)const requires(std::derived_from<T, Actor>)
	{
		return mActorSystem->getActor(_name, typeid(T));
	}

	template<class T>
	WeakObject<T> ScenePimpl::getActorByTag(const String& _tag)const requires(std::derived_from<T, Actor>)
	{
		return mActorSystem->getActorByTag(_tag, typeid(T));
	}

	template<class T>
	Array<WeakObject<T>> ScenePimpl::getActors()const requires(std::derived_from<T, Actor>)
	{
		Array<WeakObject<T>> result;
		if (auto tmp = mActorSystem->getActors(typeid(T)); tmp)
		{
			result.reserve(tmp.size());
			for (auto& actor : tmp)
			{
				result << Cast<T>(actor);
			}
		}
		return result;
	}

	template<class T>
	Array<WeakObject<T>> ScenePimpl::getActorsByTag(const String& _tag)const requires(std::derived_from<T, Actor>)
	{
		Array<WeakObject<T>> result;
		if (auto tmp = mActorSystem->getActorsByTag(_tag, typeid(T)); tmp)
		{
			result.reserve(tmp.size());
			for (auto& actor : tmp)
			{
				result << Cast<T>(actor);
			}
		}
		return result;
	}

	template<class T>
	void ScenePimpl::destroyActor(const String& _name)requires(std::derived_from<T, Actor>)
	{
		if (auto found = mActorSystem->getActor(_name, typeid(T)).lock();found)
		{
			Actor::Destroy(found);
		}
	}
}
