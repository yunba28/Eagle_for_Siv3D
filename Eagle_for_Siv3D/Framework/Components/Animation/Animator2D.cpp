#include "Animator2D.hpp"

#include <Components/Animation/Animation2D.hpp>
#include <Components/Transform.hpp>

namespace eagle
{
	Animator2D::Animator2D()
		: mBoolState()
		, mIntState()
		, mDoubleState()
		, mAnimations()
		, mTransitions()
		, mNextState(unspecified)
	{
		setFunctionEnable(false, false, false, false);
	}

	Animator2D::~Animator2D()
	{
	}

	void Animator2D::addBoolState(const String& _state, bool _default)
	{
		mBoolState.emplace(_state, _default);
	}

	void Animator2D::addIntState(const String& _state, int32 _default)
	{
		mIntState.emplace(_state, _default);
	}

	void Animator2D::addDoubleState(const String& _state, double _default)
	{
		mDoubleState.emplace(_state, _default);
	}

	bool Animator2D::removeBoolState(const String& _state)
	{
		if (mBoolState.contains(_state))
		{
			mBoolState.erase(_state);
			return true;
		}
		return false;
	}

	bool Animator2D::removeIntState(const String& _state)
	{
		if (mIntState.contains(_state))
		{
			mIntState.erase(_state);
			return true;
		}
		return false;
	}

	bool Animator2D::removeDoubleState(const String& _state)
	{
		if (mDoubleState.contains(_state))
		{
			mDoubleState.erase(_state);
			return true;
		}
		return false;
	}

	void Animator2D::setBool(const String& _state, bool _value)
	{
		if (mBoolState.contains(_state))
		{
			mBoolState[_state] = _value;
		}
	}

	void Animator2D::setInt(const String& _state, int32 _value)
	{
		if (mIntState.contains(_state))
		{
			mIntState[_state] = _value;
		}
	}

	void Animator2D::setDouble(const String& _state, double _value)
	{
		if (mDoubleState.contains(_state))
		{
			mDoubleState[_state] = _value;
		}
	}

	bool Animator2D::getBool(const String& _state)
	{
		return mBoolState.at(_state);
	}

	uint32 Animator2D::getInt(const String& _state)
	{
		return mIntState.at(_state);
	}

	double Animator2D::getDouble(const String& _state)
	{
		return mDoubleState.at(_state);
	}

	Animation2D& Animator2D::addAnimation(const String& _state)
	{
		if (mAnimations.empty())
		{
			mCurrentState = _state;
			setFunctionEnable(true, false, false, true);
		}
		mAnimations.emplace(_state, Animation2D{});
		return mAnimations[_state];
	}

	Animator2D::Transition& Animator2D::addTransition(const String& _animState)
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

	Animation2D& Animator2D::getAnimation(const String& _state)
	{
		if (mAnimations.contains(_state))
		{
			return mAnimations.at(_state);
		}
	}

	const Animation2D& Animator2D::getAnimation(const String& _state) const
	{
		if (mAnimations.contains(_state))
		{
			return mAnimations.at(_state);
		}
	}

	Array<Animator2D::Transition>& Animator2D::getTransition(const String& _state)
	{
		if (mTransitions.contains(_state))
		{
			return mTransitions.at(_state);
		}
	}

	const Array<Animator2D::Transition>& Animator2D::getTransition(const String& _state) const
	{
		if (mTransitions.contains(_state))
		{
			return mTransitions.at(_state);
		}
	}

	void Animator2D::clear()
	{
		mAnimations.clear();
		mTransitions.clear();
		mBoolState.clear();
		mIntState.clear();
		mDoubleState.clear();
		mCurrentState = U"";
		mNextState = unspecified;
	}

	void Animator2D::start()
	{
		if (mCurrentState)
		{
			mAnimations[mCurrentState].start();
		}
	}

	void Animator2D::update()
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

	void Animator2D::draw() const
	{
		mAnimations.at(mCurrentState).draw(getTransform()->getWorldPos2D());
	}

	bool Animator2D::conditionUpdate()
	{
		bool changed = false;

		bool isTransition = true;

		for (const auto& transition : mTransitions[mCurrentState])
		{
			isTransition = true;

			for (const auto& [state, f] : transition.boolConditional)
			{
				if (not f(mBoolState[state]))
				{
					isTransition = false;
					continue;
				}
			}

			for (const auto& [state, f] : transition.intConditional)
			{
				if (not f(mIntState[state]))
				{
					isTransition = false;
					continue;
				}
			}

			for (const auto& [state, f] : transition.doubleConditional)
			{
				if (not f(mDoubleState[state]))
				{
					isTransition = false;
					continue;
				}
			}

			if (isTransition)
			{
				if (transition.hasExit)
				{
					mNextState = transition.next;
					return false;
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

		return false;
	}
}
