#include <Framework/SceneObject.hpp>

#include "ActorSystem.hpp"
#include "ComponentSystem.hpp"

namespace eagle::Internal
{
	SceneObjectDetail::SceneObjectDetail()
		: mSelf()
		, mActorSystem(new ActorSystem())
		, mComponentSystem(new ComponentSystem())
		, mNameAsset()
	{

	}

	SceneObjectDetail::~SceneObjectDetail()
	{

	}

	void SceneObjectDetail::update()
	{
		mActorSystem->update();
		mComponentSystem->update();
	}

	void SceneObjectDetail::addActor(const SharedObject<Actor>& _actor, const TypeInfo& _type)
	{
		mActorSystem->add(_actor, _type);
	}

	void SceneObjectDetail::addComponent(const SharedObject<Component>& _component, const TypeInfo& _type)
	{
		mComponentSystem->add(_component, _type);
	}

	size_t SceneObjectDetail::countActor() const noexcept
	{
		return mActorSystem->count();
	}

	size_t SceneObjectDetail::countComponent() const noexcept
	{
		return mComponentSystem->count();
	}

	void SceneObjectDetail::securedName(const std::u32string& _name, UniqueTag& _tag)
	{
		mNameAsset.secured(_name, _tag);
	}

	void SceneObjectDetail::securedTag(const std::u32string& _name, CommonTag& _tag)
	{
		mTagAsset.secured(_name, _tag);
	}

	void SceneObjectDetail::_InternalSetup(const SharedObject<SceneObjectDetail>& scene)
	{
		scene->mSelf = scene.weak();
	}
}

namespace eagle
{
	SceneObject::SceneObject()
		: mDetail(MakeShared<Internal::SceneObjectDetail>())
	{
		Internal::SceneObjectDetail::_InternalSetup(mDetail);
	}

	SceneObject::~SceneObject()
	{

	}

	void SceneObject::update()
	{
		mDetail->update();
	}

	void SceneObject::addActor(const SharedObject<Actor>& _actor, const TypeInfo& _type)
	{
		mDetail->addActor(_actor, _type);
	}

	void SceneObject::addComponent(const SharedObject<Component>& _component, const TypeInfo& _type)
	{
		mDetail->addComponent(_component, _type);
	}

	size_t SceneObject::countActor() const noexcept
	{
		return mDetail->countActor();
	}

	size_t SceneObject::countComponent() const noexcept
	{
		return mDetail->countComponent();
	}

	void SceneObject::securedName(const std::u32string& _name, UniqueTag& _tag)
	{
		mDetail->securedName(_name, _tag);
	}

	void SceneObject::securedTag(const std::u32string& _name, CommonTag& _tag)
	{
		mDetail->securedTag(_name, _tag);
	}
}