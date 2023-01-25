#include <Framework/Detail/SceneObjectDetail.hpp>

#include <Framework/Detail/ActorSystem.hpp>
#include <Framework/Detail/ComponentSystem.hpp>

namespace eagle::backend
{
	SceneObjectDetail::SceneObjectDetail()
		: mSelf()
		, mActorSystem(new ActorSystem())
		, mComponentSystem(new ComponentSystem())
		//, mRenderSystem()
		, mNameAsset()
		, mTagAsset()
	{
	}

	SceneObjectDetail::~SceneObjectDetail()
	{
	}

	void SceneObjectDetail::addActor(const SharedObject<Actor>& _actor, const TypeInfo& _type)
	{
		mActorSystem->add(_actor, _type);
	}

	void SceneObjectDetail::addComponent(const SharedObject<Component>& _component, const TypeInfo& _type)
	{
		mComponentSystem->add(_component, _type);
	}

	void SceneObjectDetail::addRenderer(const WeakObject<Renderer2D>& _renderer)
	{
	}

	void SceneObjectDetail::addRenderer(const WeakObject<Renderer3D>& _renderer)
	{
	}

	void SceneObjectDetail::notifyChangeDrawOrder()
	{
	}

	void SceneObjectDetail::securedName(const String& _name, UniqueTag& _tag)
	{
		mNameAsset.secured(_name, _tag);
	}

	void SceneObjectDetail::securedTag(const String& _name, CommonTag& _tag)
	{
		mTagAsset.secured(_name, _tag);
	}

	void SceneObjectDetail::update()
	{
		mActorSystem->update();
		mComponentSystem->update();
	}

	void SceneObjectDetail::draw() const
	{
	}
}
