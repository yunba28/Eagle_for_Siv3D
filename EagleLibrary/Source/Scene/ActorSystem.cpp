#include "ActorSystem.hpp"

#include <algorithm>

namespace eagle::Internal
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

	size_t ActorSystem::count() const noexcept
	{
		size_t c{ 0 };

		for (const auto& [type, container] : mActorTable)
		{
			c += count(type);
		}

		return c;
	}

	size_t ActorSystem::count(const TypeInfo& _type) const noexcept
	{
		if (mActorTable.contains(_type))
		{
			return mActorTable.at(_type).size();
		}
		return 0;
	}
}