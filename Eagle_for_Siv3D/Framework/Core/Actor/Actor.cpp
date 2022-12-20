#include "Actor.hpp"

#include <Core/Scene/ScenePimpl.hpp>
#include <Core/Actor/ActorSystem.hpp>

#include <Components/Transform.hpp>

namespace eagle
{
	Actor::Actor()
		: mThis()
		, mScene()
		, mName()
		, mTagList()
		, mComponents()
		, mIsActive(true)
		, mIsPendingKill(false)
	{
	}

	Actor::~Actor()
	{
	}

	void Actor::setParent(const Actor_ref& _parent)
	{
		if (auto parent = _parent.lock(); parent)
		{
			getTransform()->joinCompanion(*parent->getTransform());
		}
		else
		{
			getTransform()->withdrawCompanion();
		}
	}

	void Actor::setName(const String& _name)
	{
		if (mName)
		{
			mName.set(_name);
		}
	}

	void Actor::addTag(const String& _tag)
	{
		if (auto scene = getScene();scene)
		{
			mTagList << scene->_makeTag(_tag);
		}
	}

	void Actor::removeTag(const String& _tag)
	{
		auto compare = [&_tag](const Tag& tag)
		{
			return tag == _tag;
		};

		mTagList.remove_if(compare);
	}

	void Actor::setActive(bool _active) noexcept
	{
		if (not mIsPendingKill)
		{
			mIsActive = _active;
		}
	}

	Actor::Scene_handle Actor::getScene() const noexcept
	{
		return mScene.lock();
	}

	Actor::Transform_handle Actor::getTransform() const noexcept
	{
		return mTransform.lock();
	}

	Actor::Actor_handle Actor::getParent() const noexcept
	{
		if (auto parent = mTransform.lock()->getParent(); parent)
		{
			return parent->getActor();
		}

		return Actor_ref{}.lock();
	}

	Actor::Actor_handle Actor::getRoot() const noexcept
	{
		if (auto parent = mTransform.lock()->getRoot(); parent)
		{
			return parent->getActor();
		}

		return Actor_ref{}.lock();
	}

	const String& Actor::getName() const
	{
		return *mName;
	}

	const String& Actor::getTag(size_t _index) const
	{
		return *mTagList.at(_index);
	}

	Array<String> Actor::enumrateTagList() const
	{
		Array<String> result{ Arg::reserve = mTagList.size() };
		for (auto& tag : mTagList)
		{
			result << *tag;
		}
		return result;
	}

	bool Actor::compareName(const String& _name) const noexcept
	{
		return mName == _name;
	}

	bool Actor::compareTag(const String& _tag) const noexcept
	{
		auto compare = [&_tag](const Tag& tag)
		{
			return tag == _tag;
		};
		return mTagList.any(compare);
	}

	bool Actor::isActive() const noexcept
	{
		return mIsActive;
	}

	bool Actor::isPendingKill() const noexcept
	{
		return mIsPendingKill;
	}

	const Array<Actor::Component_ref>& Actor::getAllComponents() const
	{
		return mComponents;
	}

	void Actor::_internalAwake()
	{
		awake();
	}

	void Actor::_internalDispose()
	{
		dispose();
	}

	void Actor::Destroy(const Actor_ref& actor)
	{
		if (auto act = actor.lock(); act)
		{
			act->_internalDispose();
			act->mIsActive = false;
			act->mIsPendingKill = true;
			for (auto& ref : act->mComponents)
			{
				Component::Destroy(ref);
			}
		}
	}

	void Actor::Destroy(const Actor_handle& actor)
	{
		if (actor)
		{
			actor->_internalDispose();
			actor->mIsActive = false;
			actor->mIsPendingKill = true;
			for (auto& ref : actor->mComponents)
			{
				Component::Destroy(ref);
			}
		}
	}

	void Actor::Create_impl(
		const Scene_ref& scene,
		const SharedObject<Actor>& actor,
		const std::type_index& type,
		const String& name)
	{
		auto system = scene.lock()->_getActorSystem();
		system->add(actor, type);
		actor->mThis = actor.weak();
		actor->mScene = scene;
		actor->mTransform = actor->attachComponent<Transform>();
		scene.lock()->_makeName(name, actor->mName);
		actor->_internalAwake();
	}

	void Actor::CreateEmpty_impl(
		const Scene_ref& scene,
		const SharedObject<Actor>& actor,
		const std::type_index& type,
		const String& name)
	{
		auto system = scene.lock()->_getActorSystem();
		system->add(actor, type);
		actor->mThis = actor.weak();
		actor->mScene = scene;
		scene.lock()->_makeName(name, actor->mName);
		actor->_internalAwake();
	}
}
