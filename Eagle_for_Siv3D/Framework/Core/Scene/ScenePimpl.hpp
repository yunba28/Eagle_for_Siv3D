﻿#pragma once

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

		template<class T = Actor>
		WeakObject<T> createActor(const String& _name)requires(std::derived_from<T, Actor>);

		template<class T, class TransformT>
		WeakObject<T> createActor(const String& _name)requires(std::derived_from<T, Actor> or std::derived_from<TransformT, Transform>);

		template<class T = Actor>
		WeakObject<T> getActor(const String& _name)const requires(std::derived_from<T, Actor>);

		template<class T = Actor>
		WeakObject<T> getActorByTag(const String& _tag)const requires(std::derived_from<T, Actor>);

		template<class T = Actor>
		Array<WeakObject<T>> getActors()const requires(std::derived_from<T, Actor>);

		template<class T = Actor>
		Array<WeakObject<T>> getActorsByTag(const String& _tag)const requires(std::derived_from<T, Actor>);

		template<class T = Actor>
		void destroyActor(const String& _name)requires(std::derived_from<T, Actor>);

		void destroyActorsByTag(const String& _tag);

		/// @brief シーンで使われる2Dカメラを取得します
		WeakObject<Camera2D> getCamera2D()const noexcept;

		/// @brief シーンで使われる3Dカメラを取得します
		WeakObject<DebugCamera3D> getCamera3D()const noexcept;

		/// @brief シーンの背景色を設定します
		void setBackgroundColor(const ColorF& _color);

		/// @brief シーンの背景色を取得します
		const ColorF& getBackgroundColor()const;

		ObjectHandle<ActorSystem> _getActorSystem()const noexcept;

		ObjectHandle<ComponentSystem> _getComponentSystem()const noexcept;

		ObjectHandle<FixedSystem> _getFixedSystem()const noexcept;

		ObjectHandle<RenderSystem> _getRenderSystem()const noexcept;

		void _makeName(const String& _name, Name& _outName);

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
