#include <Framework/Detail/ActorSystem.hpp>

#include <Framework/Actor.hpp>

namespace eagle::backend
{
	ActorSystem::ActorSystem()
		: mActorTable()
	{

	}

	ActorSystem::~ActorSystem()
	{

	}

	void ActorSystem::update()
	{
		auto compare = [](const SharedObject<Actor>& actor)
		{
			return actor->isPendingKill();
		};

		for (auto& [type, container] : mActorTable)
		{
			auto ret = std::remove_if(container.begin(), container.end(), compare);
			if (ret != container.end())
			{
				container.erase(ret, container.end());
			}
		}
	}

	void ActorSystem::add(const SharedObject<Actor>& _actor, const TypeInfo& _type)
	{
		if (not mActorTable.contains(_type))
		{
			mActorTable.emplace(_type, Container_t{});
		}
		mActorTable[_type].push_back(_actor);
	}
}
