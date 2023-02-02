#include <Framework/Actor.hpp>

#include <Framework/SceneObject.hpp>
#include <Framework/Transform.hpp>

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
		for (auto& ref : mAttachedComponent)
		{
			if (auto component = ref.lock();component)
			{
				Component::Destroy(*component);
			}
		}
	}

	ObjectHandle<backend::SceneObjectDetail> Actor::getSceneObject() const noexcept
	{
		return mSceneObject.lock();
	}

	ObjectHandle<Transform> Actor::getTransform() const noexcept
	{
		return mTransform.lock();
	}

	void Actor::setName(const String& _name)
	{
		mName.set(_name);
	}

	void Actor::addTag(const String& _tag)
	{
		if (mSceneObject.expired())
			return;

		auto scene = mSceneObject.lock();

		if (std::count(mTagList.begin(), mTagList.end(), _tag) == 0)
		{
			auto& tag = mTagList.emplace_back();
			scene->securedTag(_tag, tag);
		}
	}

	void Actor::removeTag(const String& _tag)
	{
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

	const String& Actor::getName() const
	{
		return mName.get();
	}

	const Array<String> Actor::enumrateTag() const
	{
		return mTagList.map([](const CommonTag& tag) {return tag.get(); });
	}

	bool Actor::compareName(const String& _name) const noexcept
	{
		return mName == _name;
	}

	bool Actor::compareTag(const String& _tag) const noexcept
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

	bool Actor::_InternalSetup(const WeakObject<backend::SceneObjectDetail>& scene, const SharedObject<Actor>& actor, const String& name)
	{
		if (actor and scene)
		{
			actor->mSelf = actor.weak();
			actor->mSceneObject = scene;
			scene.lock()->securedName(name, actor->mName);
			actor->mTransform = actor->attachComponent<Transform>();
			actor->_internalAwake();
			return true;
		}
		return false;
	}
}
