#pragma once

#include <Core/CoreFwd.hpp>
#include <Utility/SmartObject.hpp>

namespace eagle::Internal
{

	class ActorSystem final
	{
	public:

		ActorSystem();

		void update();

		void add(const SharedObject<Actor>& _actor, const TypeID& _type);

		WeakObject<Actor> getActor(const String& _name, const TypeID& _type)const;

		WeakObject<Actor> getActorByTag(const String& _tag, const TypeID& _type)const;

		Array<WeakObject<Actor>> getActors(const TypeID& _type)const;

		Array<WeakObject<Actor>> getActorsByTag(const String& _tag, const TypeID& _type)const;

		Array<WeakObject<Actor>> getAllActorsByTag(const String& _tag)const;

		size_t count(const TypeID& _type)const;

		size_t count()const;

		bool contains(const TypeID& _type)const;

	private:

		HashTable<TypeID, Array<SharedObject<Actor>>> mActors;

	};
}
