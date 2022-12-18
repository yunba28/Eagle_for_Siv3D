#include "ScenePimpl.hpp"

#include <Core/Component/ComponentSystem.hpp>
#include <Core/FixedSystem/FixedSystem.hpp>
#include <Core/RenderSystem/RenderSystem.hpp>

namespace eagle::Internal
{
	ScenePimpl::ScenePimpl()
		: mThis()
		, mActorSystem(MakeShared<ActorSystem>())
		, mComponentSystem(MakeShared<ComponentSystem>())
		, mFixedSystem(MakeShared<FixedSystem>())
		, mRenderSystem(MakeShared<RenderSystem>())
		, mNameAsset()
		, mTagAsset()
	{

	}

	ScenePimpl::~ScenePimpl()
	{

	}

	void ScenePimpl::update()
	{
		mActorSystem->update();
		mComponentSystem->update();
		mFixedSystem->update();
		mRenderSystem->update();
	}

	void ScenePimpl::draw() const
	{
		mRenderSystem->draw();
	}

	void ScenePimpl::destroyActorsByTag(const String& _tag)
	{
		if (auto tmp = mActorSystem->getAllActorsByTag(_tag); tmp)
		{
			for (auto& actor : tmp)
			{
				Actor::Destroy(actor);
			}
		}
	}

	WeakObject<Camera2D> ScenePimpl::getCamera2D() const noexcept
	{
		return mRenderSystem->getCamera2D();
	}

	WeakObject<DebugCamera3D> ScenePimpl::getCamera3D() const noexcept
	{
		return mRenderSystem->getCamera3D();
	}

	ObjectHandle<ActorSystem> ScenePimpl::_getActorSystem() const noexcept
	{
		return mActorSystem.weak().lock();
	}

	ObjectHandle<ComponentSystem> ScenePimpl::_getComponentSystem() const noexcept
	{
		return mComponentSystem.weak().lock();
	}

	ObjectHandle<FixedSystem> ScenePimpl::_getFixedSystem() const noexcept
	{
		return mFixedSystem.weak().lock();
	}

	ObjectHandle<RenderSystem> ScenePimpl::_getRenderSystem() const noexcept
	{
		return mRenderSystem.weak().lock();
	}

	Name ScenePimpl::_makeName(const String& _name)
	{
		return mNameAsset.secured(_name);
	}

	Tag ScenePimpl::_makeTag(const String& _tag)
	{
		return mTagAsset.secured(_tag);
	}

	void ScenePimpl::_setupThis(const WeakObject<ScenePimpl>& _this)
	{
		// thisポインタと比較して自身のポインタであれば登録
		if (&(*_this.lock()) == this)
		{
			mThis = _this;
		}
	}
}
