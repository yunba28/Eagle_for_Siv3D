#include <Framework/Component.hpp>

#include <Framework/Actor.hpp>

namespace eagle
{
	Component::Component() noexcept
		: mSelf()
		, mActor()
		, mTypeInfo(typeid(Component))
		, mEnable(true, true, false, true)
	{
	}

	void Component::setEnable(bool _enable) noexcept
	{
		// éÄÇÒÇ≈Ç¢ÇÈèÍçáÇÕïœçXÇµÇ»Ç¢
		if (mEnable.pendingKill)
			return;

		// ïœçXÇ»Çµ
		if (mEnable.value == _enable)
			return;

		(mEnable.value = _enable)
			? onEnable()
			: onDisable();
	}

	void Component::setEnable(Enable _enable) noexcept
	{
		mEnable.update = _enable.update;
		mEnable.lateUpdate = _enable.lateUpdate;
		mEnable.fixedUpdate = _enable.fixedUpdate;
	}

	ObjectHandle<Actor> Component::getActor()const noexcept
	{
		return mActor.lock();
	}

	const TypeInfo& Component::getTypeInfo() const noexcept
	{
		return mTypeInfo;
	}

	bool Component::compareName(const std::u32string& _name) const
	{
		if (auto actor = getActor();actor)
		{
			return actor->compareName(_name);
		}
		return false;
	}

	bool Component::compareTag(const std::u32string& _tag) const noexcept
	{
		if (auto actor = getActor(); actor)
		{
			return actor->compareTag(_tag);
		}
		return false;
	}

	bool Component::isEnable() const noexcept
	{
		return mEnable.value;
	}

	bool Component::isUpdateEnable() const noexcept
	{
		return mEnable.update;
	}

	bool Component::isStarted() const noexcept
	{
		return mEnable.started;
	}

	bool Component::isPendingKill() const noexcept
	{
		return mEnable.pendingKill;
	}

	bool Component::isLateUpdateEnable() const noexcept
	{
		return mEnable.lateUpdate;
	}

	bool Component::isFixedUpdateEnable() const noexcept
	{
		return mEnable.fixedUpdate;
	}

	void Component::_internalAwake()
	{
		awake();
	}

	void Component::_internalDispose()
	{
		dispose();
	}

	void Component::_internalUpdate()
	{
		if (not mEnable.started)
		{
			start();
			mEnable.started = true;
		}

		if (mEnable.update)
		{
			update();
		}
	}

	void Component::_internalLateUpdate()
	{
		if (mEnable.lateUpdate)
		{
			lateUpdate();
		}
	}

	void Component::_internalFixedUpdate()
	{
		if (mEnable.fixedUpdate)
		{
			fixedUpdate();
		}
	}

	void Component::Destroy(Component& component)
	{
		if (not component.isPendingKill())
		{
			component.mEnable.value = false;
			component.mEnable.pendingKill = true;
			component._internalDispose();
		}
	}

	bool Component::_InternalSetup(const WeakObject<Actor>& actor, const SharedObject<Component>& component, const TypeInfo& type)
	{
		if (component)
		{
			component->mSelf = component.weak();
			component->mActor = actor;
			component->mTypeInfo = type;
			component->_internalAwake();
			return true;
		}
		return false;
	}

	const WeakObject<Component>& Component::self() const noexcept
	{
		return mSelf;
	}
}