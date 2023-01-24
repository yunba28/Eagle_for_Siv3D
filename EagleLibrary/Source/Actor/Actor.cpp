#include <Framework/Actor.hpp>

#include <algorithm>
#include <iterator>

#include <Framework/SceneObject.hpp>

namespace eagle
{
	Actor::Actor()
		: mSelf()
		, mSceneObject()
		, mActive(true)
		, mPendingKill(false)
		, mAttachedComponent()
	{
	}

	Actor::~Actor()
	{
	}

	ObjectHandle<Internal::SceneObjectDetail> Actor::sceneObject() const noexcept
	{
		return mSceneObject.lock();
	}

	void Actor::setName(const std::u32string& _name)
	{
		mName.set(_name);
	}

	void Actor::addTag(const std::u32string& _tag)
	{
		auto scene = mSceneObject.lock();

		if (not scene)
			return;

		if (std::count(mTagList.begin(), mTagList.end(), _tag) == 0)
		{
			auto& tag = mTagList.emplace_back();
			scene->securedTag(_tag, tag);
		}
	}

	void Actor::removeTag(const std::u32string& _tag)
	{
		auto scene = mSceneObject.lock();

		if (not scene)
			return;

		auto ret = std::remove(mTagList.begin(), mTagList.end(), _tag);

		if (ret != mTagList.end())
		{
			mTagList.erase(ret, mTagList.end());
		}
	}

	void Actor::setActive(bool _active) noexcept
	{
		if (not mPendingKill)
		{
			mActive = _active;
		}
	}

	const std::u32string& Actor::getName() const
	{
		return mName.get();
	}

	const std::vector<std::u32string> Actor::enumrateTag() const
	{
		std::vector<std::u32string> result{};
		std::transform(
			mTagList.begin(), 
			mTagList.end(), 
			std::back_inserter(result), 
			[](const CommonTag& tag) {return tag.get(); });
		return result;
	}

	bool Actor::compareName(const std::u32string& _name) const noexcept
	{
		return mName == _name;
	}

	bool Actor::compareTag(const std::u32string& _tag) const noexcept
	{
		if (mTagList.empty())
			return false;

		return std::count(mTagList.begin(), mTagList.end(), _tag) > 0;
	}

	bool Actor::isActive() const noexcept
	{
		return mActive;
	}

	bool Actor::isPendingKill() const noexcept
	{
		return mPendingKill;
	}

	WeakObject<Component> Actor::getComponent(const TypeInfo& _info) const
	{
		for (const auto& weak : mAttachedComponent)
		{
			if (auto component = weak.lock(); component->getTypeInfo() == _info)
			{
				return weak;
			}
		}
		return WeakObject<Component>{};
	}

	void Actor::_internalAwake()
	{
		awake();
	}
	
	void Actor::_internalDispose()
	{
		dispose();
	}

	const WeakObject<Actor>& Actor::self() const noexcept
	{
		return mSelf;
	}

	void Actor::addComponent(const SharedObject<Component>& _component, const TypeInfo& _type)
	{
		auto scene = mSceneObject.lock();
		scene->addComponent(_component, _type);
		Component::_InternalSetup(mSelf, _component, _type);
	}

	void Actor::Destroy(Actor& actor)
	{
		if (not actor.mPendingKill)
		{
			actor.mActive = false;
			actor.mPendingKill = true;
			actor._internalDispose();
		}
	}

	bool Actor::_InternalSetup(const WeakObject<Internal::SceneObjectDetail>& scene, const SharedObject<Actor>& actor, const std::u32string& name)
	{
		auto sc = scene.lock();

		if (actor and sc)
		{
			actor->mSelf = actor.weak();
			actor->mSceneObject = scene;
			sc->securedName(name, actor->mName);
			actor->_internalAwake();
			return true;
		}
		return false;
	}
}