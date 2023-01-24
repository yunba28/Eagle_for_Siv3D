#pragma once

#include <unordered_map>
#include <Framework/Actor.hpp>

namespace eagle::Internal
{
	class ActorSystem final
	{
	public:

		ActorSystem();

		~ActorSystem();

		void update();

		void add(const SharedObject<Actor>& _actor, const TypeInfo& _type);

		size_t count()const noexcept;

		size_t count(const TypeInfo& _type)const noexcept;

	private:

		using Container_t = std::vector<SharedObject<Actor>>;
		std::unordered_map<TypeInfo, Container_t> mActorTable;

	};
}