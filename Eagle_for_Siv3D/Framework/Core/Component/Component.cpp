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
		, mIsEnabledUpdate(true)
		, mIsEnabledLateUpdate(true)
		, mIsEnabledFixedUpdate(false)
		, mIsJoinedFixedSystem(false)
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

		mIsEnabled = _enable;

		mIsEnabled ? onEnable() : onDisable();
	}

	void Component::setFunctionEnable(bool _update, bool _lateUpdate, bool _fixedUpdate) noexcept
	{
		mIsEnabledUpdate = _update;
		mIsEnabledLateUpdate = _lateUpdate;
		mIsEnabledFixedUpdate = _fixedUpdate;
	}

	Actor_handle Component::getActor() const noexcept
	{
		return mActor.lock();
	}

	Transform_handle Component::getTransform() const noexcept
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

	const uint64 Component::getCode() const noexcept
	{
		return mType.hash_code();
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
		if (not mIsEnabledUpdate)
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
		if (not mIsEnabledLateUpdate)
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
		if (not mIsEnabledFixedUpdate)
			return;

		auto actor = getActor();

		// Actorが有効であれば更新する
		if (actor and actor->isActive())
		{
			fixedUpdate();
		}
	}

	void Component::setEnable(bool _enable, bool _callEvent)
	{
		if (_callEvent)
		{
			setEnable(_enable);
		}
		else
		{
			mIsEnabled = _enable;
		}
	}

	bool Component::isEnableUpdate() const noexcept
	{
		return mIsEnabledUpdate;
	}

	bool Component::isEnableLateUpdate() const noexcept
	{
		return mIsEnabledLateUpdate;
	}

	bool Component::isEnableFixedUpdate() const noexcept
	{
		return mIsEnabledFixedUpdate;
	}

	void Component::joinFixedSystem()
	{
		if (not mIsJoinedFixedSystem)
		{
			auto system = getActor()->getScene()->_getFixedSystem();
			system->add(mThis);
			mIsEnabledFixedUpdate = true;
			mIsJoinedFixedSystem = true;
		}
	}

	void Component::joinFixedSystem(const WeakObject<class Collider2D>& _collider)
	{
		if (not mIsJoinedFixedSystem)
		{
			auto system = getActor()->getScene()->_getFixedSystem();
			system->add(_collider);
			system->add(mThis);
			mIsEnabledFixedUpdate = true;
			mIsJoinedFixedSystem = true;
		}
		else
		{
			auto system = getActor()->getScene()->_getFixedSystem();
			system->add(_collider);
		}
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
