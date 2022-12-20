#pragma once

#include "Animation2D.hpp"

namespace eagle
{
	class AnimationController2D : public DrawableComponent2D
	{
	public:

		struct Transition
		{
			String next;
			Array<std::pair<String, bool(*)(bool)>> boolConditional;
			Array<std::pair<String, bool(*)(int32)>> intConditional;
			Array<std::pair<String, bool(*)(double)>> doubleConditional;
			bool hasExit;
		};

	public:

		AnimationController2D();

		~AnimationController2D();

		void addBoolState(const String& _state, bool _default = false);

		void addIntState(const String& _state, int32 _default = 0);

		void addDoubleState(const String& _state, double _default = 0);

		bool removeBoolState(const String& _state);

		bool removeIntState(const String& _state);

		bool removeDoubleState(const String& _state);

		void setBool(const String& _state, bool _value);

		void setInt(const String& _state, int32 _value);

		void setDouble(const String& _state, double _value);

		Animation2D& addAnimation(const String& _state);

		Transition& addTransition(const String& _animState);

	private:

		void start()override;

		void update()override;

		void draw()const override;

		bool conditionUpdate();

	private:

		HashTable<String, bool> mBoolState;

		HashTable<String, int32> mIntState;

		HashTable<String, double> mDoubleState;

		HashTable<String, Animation2D> mAnimations;

		HashTable<String, Array<Transition>> mTransitions;

		String mCurrentState;

		Optional<String> mNextState;
	};
}
