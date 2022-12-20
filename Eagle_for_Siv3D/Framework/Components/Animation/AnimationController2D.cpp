#include "AnimationController2D.hpp"

#include <Components/Transform.hpp>

namespace eagle
{
	AnimationController2D::AnimationController2D()
		: mBoolState()
		, mIntState()
		, mDoubleState()
		, mAnimations()
		, mTransitions()
		, mNextState(unspecified)
	{
		setFunctionEnable(false, false, false, false);
	}

	AnimationController2D::~AnimationController2D()
	{
	}

	void AnimationController2D::addBoolState(const String& _state, bool _default)
	{
		mBoolState.emplace(_state, _default);
	}

	void AnimationController2D::addIntState(const String& _state, int32 _default)
	{
		mIntState.emplace(_state, _default);
	}

	void AnimationController2D::addDoubleState(const String& _state, double _default)
	{
		mDoubleState.emplace(_state, _default);
	}

	bool AnimationController2D::removeBoolState(const String& _state)
	{
		if (mBoolState.contains(_state))
		{
			mBoolState.erase(_state);
			return true;
		}
		return false;
	}

	bool AnimationController2D::removeIntState(const String& _state)
	{
		if (mIntState.contains(_state))
		{
			mIntState.erase(_state);
			return true;
		}
		return false;
	}

	bool AnimationController2D::removeDoubleState(const String& _state)
	{
		if (mDoubleState.contains(_state))
		{
			mDoubleState.erase(_state);
			return true;
		}
		return false;
	}

	void AnimationController2D::setBool(const String& _state, bool _value)
	{
		if (mBoolState.contains(_state))
		{
			mBoolState[_state] = _value;
		}
	}

	void AnimationController2D::setInt(const String& _state, int32 _value)
	{
		if (mIntState.contains(_state))
		{
			mIntState[_state] = _value;
		}
	}

	void AnimationController2D::setDouble(const String& _state, double _value)
	{
		if (mDoubleState.contains(_state))
		{
			mDoubleState[_state] = _value;
		}
	}

	Animation2D& AnimationController2D::addAnimation(const String& _state)
	{
		if (mAnimations.empty())
		{
			mCurrentState = _state;
			setFunctionEnable(true, false, false, true);
		}
		mAnimations.emplace(_state, Animation2D{});
		return mAnimations[_state];
	}

	AnimationController2D::Transition& AnimationController2D::addTransition(const String& _animState)
	{
		if (mAnimations.contains(_animState))
		{
			if (mTransitions.empty())
			{
				mTransitions.emplace(_animState, Array<Transition>{});
			}
			mTransitions[_animState] << Transition{};
			return mTransitions[_animState].back();
		}

		throw Error{U"failed add transition."};
	}

	void AnimationController2D::start()
	{
		if (mCurrentState)
		{
			mAnimations[mCurrentState].start();
		}
	}

	void AnimationController2D::update()
	{
		if (conditionUpdate())
			return;

		auto& current = mAnimations[mCurrentState];
		current.update();

		if (current.isComplated() and mNextState)
		{
			if (mCurrentState != *mNextState)
			{
				mCurrentState = *mNextState;
				mNextState = unspecified;
				mAnimations[mCurrentState].restart();
			}
		}
	}

	void AnimationController2D::draw() const
	{
		mAnimations.at(mCurrentState).draw(getTransform()->getWorldPos2D());
	}

	bool AnimationController2D::conditionUpdate()
	{
		bool changed = false;

		for (const auto& transition : mTransitions[mCurrentState])
		{
			for (const auto& [state, f] : transition.boolConditional)
			{
				if (f(mBoolState[state]))
				{
					if (transition.hasExit)
					{
						mNextState = transition.next;
					}
					else
					{
						mCurrentState = transition.next;
						mAnimations[mCurrentState].restart();
						changed = true;
					}
					return changed;
				}
			}

			for (const auto& [state, f] : transition.intConditional)
			{
				if (f(mIntState[state]))
				{
					if (transition.hasExit)
					{
						mNextState = transition.next;
					}
					else
					{
						mCurrentState = transition.next;
						mAnimations[mCurrentState].restart();
						changed = true;
					}
					return changed;
				}
			}

			for (const auto& [state, f] : transition.doubleConditional)
			{
				if (f(mDoubleState[state]))
				{
					if (transition.hasExit)
					{
						mNextState = transition.next;
					}
					else
					{
						mCurrentState = transition.next;
						mAnimations[mCurrentState].restart();
						changed = true;
					}
					return changed;
				}
			}
		}

		return false;
	}
}
