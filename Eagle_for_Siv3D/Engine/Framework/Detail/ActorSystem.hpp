#pragma once

#include <Framework/FrameworkFwd.hpp>
#include <Common/SmartObject.hpp>

namespace eagle::backend
{
	class ActorSystem final
	{
	public:

		ActorSystem();

		~ActorSystem();

		void update();

		void add(const SharedObject<Actor>& _actor, const TypeInfo& _type);

	private:

		using Container_t = Array<SharedObject<Actor>>;
		HashTable<TypeInfo, Container_t> mActorTable;

	};
}
