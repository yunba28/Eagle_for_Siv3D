#pragma once

#include <Core/Actor/Actor.hpp>
#include <Core/Actor/ActorSystem.hpp>

namespace eagle::Internal
{
	class ScenePimpl final : Uncopyable
	{
	public:

		ScenePimpl();

		~ScenePimpl();

		void update();

		void draw()const;

		template<class T>
		WeakObject<T> createActor(const String& _name)requires(std::derived_from<T, Actor>);

		template<class T>
		WeakObject<T> getActor(const String& _name)const requires(std::derived_from<T, Actor>);

		template<class T>
		WeakObject<T> getActorByTag(const String& _tag)const requires(std::derived_from<T, Actor>);

		template<class T>
		Array<WeakObject<T>> getActors()const requires(std::derived_from<T, Actor>);

		template<class T>
		Array<WeakObject<T>> getActorsByTag(const String& _tag)const requires(std::derived_from<T, Actor>);

		template<class T>
		void destroyActor(const String& _name)requires(std::derived_from<T, Actor>);

		void destroyActorsByTag(const String& _tag);

		WeakObject<Camera2D> getCamera2D()const noexcept;

		WeakObject<DebugCamera3D> getCamera3D()const noexcept;

		ObjectHandle<ActorSystem> _getActorSystem()const noexcept;

		ObjectHandle<ComponentSystem> _getComponentSystem()const noexcept;

		ObjectHandle<FixedSystem> _getFixedSystem()const noexcept;

		ObjectHandle<RenderSystem> _getRenderSystem()const noexcept;

		Name _makeName(const String& _name);

		Tag _makeTag(const String& _tag);

		void _setupThis(const WeakObject<ScenePimpl>& _this);

	private:

		WeakObject<ScenePimpl> mThis;

		SharedObject<ActorSystem> mActorSystem;

		SharedObject<ComponentSystem> mComponentSystem;

		SharedObject<FixedSystem> mFixedSystem;

		SharedObject<RenderSystem> mRenderSystem;

		NameAsset mNameAsset;

		TagAsset mTagAsset;
	};
}

#include "ScenePimpl.ipp"
