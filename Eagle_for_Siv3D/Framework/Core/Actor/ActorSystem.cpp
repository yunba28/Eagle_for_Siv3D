#include "ActorSystem.hpp"

#include <Core/Actor/Actor.hpp>

namespace eagle::Internal
{
	ActorSystem::ActorSystem()
		: mActors()
	{
	}

	void ActorSystem::update()
	{
		auto compare = [](const SharedObject<Actor>& actor)
		{
			return actor->isPendingKill();
		};

		for (auto& [type, list] : mActors)
		{
			list.remove_if(compare);
		}
	}

	void ActorSystem::add(const SharedObject<Actor>& _actor, const TypeID& _type)
	{
		if (not mActors.contains(_type))
		{
			mActors.emplace(_type, Array<SharedObject<Actor>>{});
		}

		mActors[_type] << _actor;
	}

	WeakObject<Actor> ActorSystem::getActor(const String& _name, const TypeID& _type) const
	{
		// 型がない
		if (not contains(_type))
			return WeakObject<Actor>{};

		// 名前を比較するためのラムダ式
		auto compare = [&_name](const SharedObject<Actor>& actor)
		{
			return actor->compareName(_name);
		};

		auto& list = mActors.at(_type);

		// 該当する名前のActorが見つかれば参照を返す
		if (auto found = std::find_if(list.cbegin(), list.cend(), compare); found != list.cend())
		{
			return found->weak();
		}

		return WeakObject<Actor>{};
	}

	WeakObject<Actor> ActorSystem::getActorByTag(const String& _tag, const TypeID& _type) const
	{
		// 型がない
		if (not contains(_type))
			return WeakObject<Actor>{};

		// タグを比較するためのラムダ式
		auto compare = [&_tag](const SharedObject<Actor>& actor)
		{
			return actor->compareTag(_tag);
		};

		auto& list = mActors.at(_type);

		// 該当するタグを持つActorが見つかれば参照を返す
		if (auto found = std::find_if(list.cbegin(), list.cend(), compare); found != list.cend())
		{
			return found->weak();
		}

		return WeakObject<Actor>{};
	}

	Array<WeakObject<Actor>> ActorSystem::getActors(const TypeID& _type) const
	{
		Array<WeakObject<Actor>> result;

		if (contains(_type))
		{
			result.reserve(count(_type));
			for (auto& actor : mActors.at(_type))
			{
				result << actor.weak();
			}
		}

		return result;
	}

	Array<WeakObject<Actor>> ActorSystem::getActorsByTag(const String& _tag, const TypeID& _type) const
	{
		Array<WeakObject<Actor>> result;

		if (not contains(_type))
			return result;

		// タグを比較するためのラムダ式
		auto compare = [&_tag](const SharedObject<Actor>& actor)
		{
			return actor->compareTag(_tag);
		};

		auto& list = mActors.at(_type);

		// 該当するタグのActorの配列を取得
		if (auto tmp = list.filter(compare); tmp)
		{
			result.reserve(tmp.size());
			for (auto& actor : tmp)
			{
				result << actor.weak();
			}
		}

		return result;
	}

	Array<WeakObject<Actor>> ActorSystem::getAllActorsByTag(const String& _tag) const
	{
		// タグを比較するためのラムダ式
		auto compare = [&_tag](const SharedObject<Actor>& actor)
		{
			return actor->compareTag(_tag);
		};

		Array<WeakObject<Actor>> result{ Arg::reserve = count() };

		// すべてのActor型を走査
		for (auto& [type, list] : mActors)
		{
			// 該当するタグを持つActorが見つかればresultに追加
			if (auto tmp = list.filter(compare); tmp)
			{
				for (auto& actor : tmp)
				{
					// 弱参照を渡す
					result << actor.weak();
				}
			}
		}

		return result;
	}

	size_t ActorSystem::count(const TypeID& _type) const
	{
		if (contains(_type))
		{
			return mActors.at(_type).size();
		}

		return 0;
	}

	size_t ActorSystem::count() const
	{
		size_t s{0};
		for (auto& [type, list] : mActors)
		{
			s += list.size();
		}
		return s;
	}

	bool ActorSystem::contains(const TypeID& _type) const
	{
		return mActors.contains(_type);
	}
}
