#include "BasicScene.hpp"
#pragma once

namespace eagle
{
	template<class Data>
	BasicScene<Data>::InitData::InitData(const State_t& _state, const SharedObject<Data_t>& _data, GameWorld<Data_t>*& _world)
		: state(_state)
		, data(_data)
		, world(_world)
	{

	}

	template<class Data>
	BasicScene<Data>::BasicScene(const InitData& _ini)
		: mScenePimpl(MakeShared<Internal::ScenePimpl>())
		, mState(_ini.state)
		, mData(_ini.data)
		, mWorld(_ini.world)
	{
		mScenePimpl->_setupThis(mScenePimpl.weak());
	}

	template<class Data>
	void BasicScene<Data>::drawFadeIn(double t)const
	{
		_internalDraw();

		Transformer2D transform{ Mat3x2::Identity(), Transformer2D::Target::SetLocal };

		Scene::Rect().draw(ColorF{ mWorld->getFadeColor(), (1.0 - t) });
	}

	template<class Data>
	void BasicScene<Data>::drawFadeOut(double t)const
	{
		_internalDraw();

		Transformer2D transform{ Mat3x2::Identity(), Transformer2D::Target::SetLocal };

		Scene::Rect().draw(ColorF{ mWorld->getFadeColor(), t });
	}

	template<class Data>
	void BasicScene<Data>::_internalUpdate()
	{
		mScenePimpl->update();

		update();
	}

	template<class Data>
	void BasicScene<Data>::_internalDraw()const
	{
		mScenePimpl->draw();

		draw();
	}

	template <class Data>
	const typename BasicScene<Data>::State_t& BasicScene<Data>::getState() const
	{
		return mState;
	}

	template <class Data>
	template <class DataType, std::enable_if_t<not std::disjunction_v<std::is_array<DataType>, std::is_void<DataType>>>*>
	DataType& BasicScene<Data>::getData() const
	{
		return *mData;
	}

	template <class Data>
	bool BasicScene<Data>::changeScene(const State_t& state, const Duration& transitionTime, const CrossFade crossFade)
	{
		return changeScene(state, static_cast<int32>(transitionTime.count() * 1000), crossFade);
	}

	template <class Data>
	bool BasicScene<Data>::changeScene(const State_t& state, const int32 transitionTimeMillisec, const CrossFade crossFade)
	{
		return mWorld->changeScene(state, transitionTimeMillisec, crossFade);
	}

	template <class Data>
	void BasicScene<Data>::notifyError()
	{
		return mWorld->notifyError();
	}

	template <class Data>
	template<class T>
	WeakObject<T> BasicScene<Data>::createActor(const String& _name)requires(std::derived_from<T, Actor>)
	{
		return mScenePimpl->createActor<T>(_name);
	}

	template <class Data>
	template<class T>
	WeakObject<T> BasicScene<Data>::getActor(const String& _name)const requires(std::derived_from<T, Actor>)
	{
		return mScenePimpl->getActor<T>(_name);
	}

	template <class Data>
	template<class T>
	WeakObject<T> BasicScene<Data>::getActorByTag(const String& _tag)const requires(std::derived_from<T, Actor>)
	{
		return mScenePimpl->getActorByTag(_tag);
	}

	template <class Data>
	template<class T>
	Array<WeakObject<T>> BasicScene<Data>::getActors()const requires(std::derived_from<T, Actor>)
	{
		return mScenePimpl->getActors();
	}

	template <class Data>
	template<class T>
	Array<WeakObject<T>> BasicScene<Data>::getActorsByTag(const String& _tag)const requires(std::derived_from<T, Actor>)
	{
		return mScenePimpl->getActorsByTag(_tag);
	}

	template <class Data>
	template<class T>
	void BasicScene<Data>::destroyActor(const String& _name)requires(std::derived_from<T, Actor>)
	{
		mScenePimpl->destroyActor();
	}

	template<class Data>
	void BasicScene<Data>::destroyActorsByTag(const String& _tag)
	{
		mScenePimpl->destroyActorsByTag(_tag);
	}
}
