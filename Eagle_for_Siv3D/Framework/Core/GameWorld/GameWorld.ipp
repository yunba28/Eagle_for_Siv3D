#pragma once

namespace eagle
{
	template <class Data>
	GameWorld<Data>::GameWorld()
		: mGameWorldPtr{ std::make_unique<GameWorld*>(this) }
	{
		if constexpr (not std::is_void_v<Data>)
		{
			mData = MakeShared<Data>();
		}
	}

	template <class Data>
	GameWorld<Data>::GameWorld(GameWorld&& other) noexcept
		: mGameWorldPtr{ std::move(other.mGameWorldPtr) }
		, mFactories{ std::move(other.mFactories) }
		, mData{ std::move(other.mData) }
		, mCurrent{ std::move(other.mCurrent) }
		, mNext{ std::move(other.mNext) }
		, mCurrentState{ std::move(other.mCurrentState) }
		, mNextState{ std::move(other.mNextState) }
		, mFirst{ std::move(other.mFirst) }
		, mTransitionState{ std::move(other.mTransitionState) }
		, mStopwatch{ std::move(other.mStopwatch) }
		, mTransitionTimeMillisec{ std::move(other.mTransitionTimeMillisec) }
		, mFadeColor{ std::move(other.mFadeColor) }
		, mCrossFade{ std::move(other.mCrossFade) }
		, mError{ std::move(other.mError) }
	{
		*mGameWorldPtr = this;
	}

	template <class Data>
	GameWorld<Data>::GameWorld(const std::shared_ptr<Data>& data)
		: mGameWorldPtr{ std::make_unique<GameWorld*>(this) }
		, mData{ data } {}

	template <class Data>
	GameWorld<Data>& GameWorld<Data>::operator=(GameWorld&& other) noexcept
	{
		mGameWorldPtr = std::move(other.mGameWorldPtr);
		mFactories = std::move(other.mFactories);
		mData = std::move(other.mData);
		mCurrent = std::move(other.mCurrent);
		mNext = std::move(other.mNext);
		mCurrentState = std::move(other.mCurrentState);
		mNextState = std::move(other.mNextState);
		mFirst = std::move(other.mFirst);
		mTransitionState = std::move(other.mTransitionState);
		mStopwatch = std::move(other.mStopwatch);
		mTransitionTimeMillisec = std::move(other.mTransitionTimeMillisec);
		mFadeColor = std::move(other.mFadeColor);
		mCrossFade = std::move(other.mCrossFade);
		mError = std::move(other.mError);
		*mGameWorldPtr = this;
		return *this;
	}

	template <class Data>
	template <class SceneType>
	GameWorld<Data>& GameWorld<Data>::add(const State_t& state)
	{
		typename SceneType::InitData initData{ state, mData, *mGameWorldPtr };

		auto factory = [=]() {
			return MakeShared<SceneType>(initData);
		};

		auto it = mFactories.find(state);

		if (it != mFactories.end())
		{
			it->second = factory;
		}
		else
		{
			mFactories.emplace(state, factory);

			if (not mFirst)
			{
				mFirst = state;
			}
		}

		return *this;
	}

	template <class Data>
	bool GameWorld<Data>::init(const State_t& state)
	{
		if (mCurrent)
		{
			return false;
		}

		auto it = mFactories.find(state);

		if (it == mFactories.end())
		{
			return false;
		}

		mCurrentState = state;

		mCurrent = it->second();

		if (hasError())
		{
			return false;
		}

		mTransitionState = TransitionState::FadeIn;

		mStopwatch.restart();

		return true;
	}

	template <class Data>
	bool GameWorld<Data>::updateScene()
	{
		if (hasError())
		{
			return false;
		}

		if (not mCurrent)
		{
			if (not mFirst)
			{
				return true;
			}
			else if (not init(*mFirst))
			{
				return false;
			}
		}

		if (mCrossFade)
		{
			return updateCross();
		}
		else
		{
			return updateSingle();
		}
	}

	template <class Data>
	void GameWorld<Data>::drawScene() const
	{
		if (not mCurrent)
		{
			return;
		}

		if ((mTransitionState == TransitionState::Active)
			|| (mTransitionTimeMillisec <= 0))
		{
			mCurrent->_internalDraw();
		}

		const double elapsed = mStopwatch.msF();
		const double t = (mTransitionTimeMillisec ? (elapsed / mTransitionTimeMillisec) : 1.0);

		if (mTransitionState == TransitionState::FadeIn)
		{
			mCurrent->drawFadeIn(t);
		}
		else if (mTransitionState == TransitionState::FadeOut)
		{
			mCurrent->drawFadeOut(t);
		}
		else if (mTransitionState == TransitionState::FadeInOut)
		{
			mCurrent->drawFadeOut(t);

			if (mNext)
			{
				mNext->drawFadeIn(t);
			}
		}
	}

	template <class Data>
	bool GameWorld<Data>::update()
	{
		if (not updateScene())
		{
			return false;
		}

		drawScene();

		return true;
	}

	template <class Data>
	SharedObject<Data> GameWorld<Data>::get() noexcept
	{
		return mData;
	}

	template <class Data>
	const SharedObject<const Data> GameWorld<Data>::get() const noexcept
	{
		return mData;
	}

	template <class Data>
	bool GameWorld<Data>::changeScene(const State_t& state, const Duration& transitionTime, const CrossFade crossFade)
	{
		return changeScene(state, static_cast<int32>(transitionTime.count() * 1000), crossFade);
	}

	template <class Data>
	bool GameWorld<Data>::changeScene(const State_t& state, const int32 transitionTimeMillisec, CrossFade crossFade)
	{
		if (state == mCurrentState)
		{
			crossFade = CrossFade::No;
		}

		if (not mFactories.contains(state))
		{
			return false;
		}

		mNextState = state;

		mCrossFade = crossFade;

		if (crossFade)
		{
			mTransitionTimeMillisec = transitionTimeMillisec;

			mTransitionState = TransitionState::FadeInOut;

			mNext = mFactories[mNextState]();

			if (hasError())
			{
				return false;
			}

			mCurrentState = mNextState;

			mStopwatch.restart();
		}
		else
		{
			mTransitionTimeMillisec = (transitionTimeMillisec / 2);

			mTransitionState = TransitionState::FadeOut;

			mStopwatch.restart();
		}

		return true;
	}

	template <class Data>
	GameWorld<Data>& GameWorld<Data>::setFadeColor(const ColorF& color) noexcept
	{
		mFadeColor = color;

		return *this;
	}

	template <class Data>
	const ColorF& GameWorld<Data>::getFadeColor() const noexcept
	{
		return mFadeColor;
	}

	template <class Data>
	void GameWorld<Data>::notifyError() noexcept
	{
		mError = true;
	}

	template <class Data>
	bool GameWorld<Data>::updateSingle()
	{
		double elapsed = mStopwatch.msF();

		if ((mTransitionState == TransitionState::FadeOut)
			&& (mTransitionTimeMillisec <= elapsed))
		{
			mCurrent = nullptr;

			mCurrent = mFactories[mNextState]();

			if (hasError())
			{
				return false;
			}

			mCurrentState = mNextState;

			mTransitionState = TransitionState::FadeIn;

			mStopwatch.restart();

			elapsed = 0.0;
		}

		if ((mTransitionState == TransitionState::FadeIn)
			&& (mTransitionTimeMillisec <= elapsed))
		{
			mStopwatch.reset();

			mTransitionState = TransitionState::Active;
		}

		const double t = (mTransitionTimeMillisec ? (elapsed / mTransitionTimeMillisec) : 1.0);

		switch (mTransitionState)
		{
		case TransitionState::FadeIn:
			mCurrent->updateFadeIn(t);
			break;
		case TransitionState::Active:
			mCurrent->_internalUpdate();
			break;
		case TransitionState::FadeOut:
			mCurrent->updateFadeOut(t);
			break;
		default:
			return false;
		}

		return (not hasError());
	}

	template <class Data>
	bool GameWorld<Data>::updateCross()
	{
		const double elapsed = mStopwatch.msF();

		if ((mTransitionState == TransitionState::FadeInOut)
			&& (mTransitionTimeMillisec <= elapsed))
		{
			mCurrent = mNext;

			mNext = nullptr;

			mStopwatch.reset();

			mTransitionState = TransitionState::Active;
		}

		if (mTransitionState == TransitionState::Active)
		{
			mCurrent->_internalUpdate();
		}
		else
		{
			assert(mTransitionTimeMillisec);

			const double t = (mTransitionTimeMillisec ? (elapsed / mTransitionTimeMillisec) : 1.0);

			mCurrent->updateFadeOut(t);

			if (hasError())
			{
				return false;
			}

			mNext->updateFadeIn(t);
		}

		return (not hasError());
	}

	template <class Data>
	bool GameWorld<Data>::hasError() const noexcept
	{
		return mError;
	}
}
