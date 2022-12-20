#include "Component.hpp"

#include <Core/Actor/Actor.hpp>
#include <Core/Component/ComponentSystem.hpp>
#include <Core/FixedSystem/FixedSystem.hpp>
#include <Core/Scene/ScenePimpl.hpp>

#include <Components/Transform.hpp>
#include <Components/Collider/Collider2D.hpp>

namespace eagle
{
	Component::Component()
		: mThis()
		, mActor()
		, mType(typeid(Component))
		, mIsEnabled(true)
		, mIsStarted(false)
		, mIsPendingKill(false)
		, mEnabledUpdate(true)
		, mEnabledLateUpdate(true)
		, mEnabledFixedUpdate(false)
	{
	}

	Component::~Component()
	{
	}

	void Component::setEnable(bool _enable) noexcept
	{
		// Componentに破棄命令が出ている
		if (mIsPendingKill)
			return;

		// 変更なし
		if (mIsEnabled == _enable)
			return;

		setEnableUncalledEvent(_enable);

		mIsEnabled ? onEnable() : onDisable();
	}

	void Component::setFunctionEnable(bool _update, bool _lateUpdate, bool _fixedUpdate) noexcept
	{
		mEnabledUpdate = _update;
		mEnabledLateUpdate = _lateUpdate;
		mEnabledFixedUpdate = _fixedUpdate;
	}

	Component::Actor_handle Component::getActor() const noexcept
	{
		return mActor.lock();
	}

	Component::Transform_handle Component::getTransform() const noexcept
	{
		if (auto actor = getActor(); actor)
		{
			return actor->getTransform();
		}

		return Transform_ref{}.lock();
	}

	const TypeID& Component::getType() const noexcept
	{
		return mType;
	}

	bool Component::compareType(const TypeID& _type) const noexcept
	{
		return mType == _type;
	}

	bool Component::compareName(const String& _name) const noexcept
	{
		if (auto actor = getActor(); actor)
		{
			return actor->compareName(_name);
		}

		return false;
	}

	bool Component::compareTag(const String& _tag) const noexcept
	{
		if (auto actor = getActor(); actor)
		{
			return actor->compareTag(_tag);
		}

		return false;
	}

	bool Component::isEnable() const noexcept
	{
		return mIsEnabled;
	}

	bool Component::isPendingKill() const noexcept
	{
		return mIsPendingKill;
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
		// updateが無効
		if (not mEnabledUpdate)
			return;

		auto actor = getActor();

		// Actorが有効であれば更新する
		if (actor and actor->isActive())
		{
			// 最初の更新を行う
			if (not mIsStarted)
			{
				start();
				mIsStarted = true;
			}

			update();
		}
	}

	void Component::_internalLateUpdate()
	{
		// lateUpdateが無効
		if (not mEnabledLateUpdate)
			return;

		auto actor = getActor();

		// Actorが有効であれば更新する
		if (actor and actor->isActive())
		{
			lateUpdate();
		}
	}

	void Component::_internalFixedUpdate()
	{
		// fixedUpdateが無効
		if (not mEnabledFixedUpdate)
			return;

		auto actor = getActor();

		// Actorが有効であれば更新する
		if (actor and actor->isActive())
		{
			fixedUpdate();
		}
	}

	void Component::setEnableUncalledEvent(bool _enable)
	{
		mIsEnabled = _enable;
	}

	bool Component::isEnableUpdate() const noexcept
	{
		return mEnabledUpdate;
	}

	bool Component::isEnableLateUpdate() const noexcept
	{
		return mEnabledLateUpdate;
	}

	bool Component::isEnableFixedUpdate() const noexcept
	{
		return mEnabledFixedUpdate;
	}

	void Component::joinFixedSystem()
	{
		auto system = getActor()->getScene()->_getFixedSystem();
		system->add(mThis);
		mEnabledFixedUpdate = true;
		mJoinedFixedSystem = true;
	}

	void Component::joinFixedSystem(const WeakObject<class Collider2D>& _collider)
	{
		auto system = getActor()->getScene()->_getFixedSystem();
		system->add(_collider);
		system->add(mThis);
		mEnabledFixedUpdate = true;
		mJoinedFixedSystem = true;
	}

	void Component::Destroy(const Component_ref& component)
	{
		if (auto handle = component.lock(); handle)
		{
			handle->mIsPendingKill = true;
			handle->mIsEnabled = false;
			handle->_internalDispose();
		}
	}

	void Component::Destroy(const Component_handle& component)
	{
		if (component)
		{
			component->mIsPendingKill = true;
			component->mIsEnabled = false;
			component->_internalDispose();
		}
	}

	void Component::Create_impl(const Actor_ref& actor, const SharedObject<Component>& component, const std::type_index& type)
	{
		auto system = actor.lock()->getScene()->_getComponentSystem();
		system->add(component, type);
		component->mThis = component.weak();
		component->mActor = actor;
		component->mType = type;
		component->_internalAwake();
	}
}
