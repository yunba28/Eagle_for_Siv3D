#pragma once

#include <Core/CoreFwd.hpp>
#include <Utility/SmartObject.hpp>

namespace eagle::Internal
{

	class ActorSystem final
	{
	private:

		using List_t = Array<SharedObject<Actor>>;

	public:

		ActorSystem();

		void update();

		void add(const SharedObject<Actor>& _actor, const std::type_index& _type);

		WeakObject<Actor> getActor(const String& _name, const std::type_index& _type)const;

		WeakObject<Actor> getActorByTag(const String& _tag, const std::type_index& _type)const;

		Array<WeakObject<Actor>> getActors(const std::type_index& _type)const;

		Array<WeakObject<Actor>> getActorsByTag(const String& _tag, const std::type_index& _type)const;

		Array<WeakObject<Actor>> getAllActorsByTag(const String& _tag)const;

		size_t count(const std::type_index& _type)const;

		size_t count()const;

		bool contains(const std::type_index& _type)const;

	private:

		HashTable<std::type_index, List_t> mActors;

	};
}
