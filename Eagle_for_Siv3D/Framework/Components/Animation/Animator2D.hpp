#pragma once

#include <Core/RenderSystem/DrawableComponent.hpp>
#include <Utility/Compares.hpp>

namespace eagle
{
	class Animation2D;

	class Animator2D : public DrawableComponent2D
	{
	private:

		using BoolConditional = Array<std::pair<String, std::function<bool(bool)>>>;
		using IntConditional = Array<std::pair<String, std::function<bool(int32)>>>;
		using DoubleConditional = Array<std::pair<String, std::function<bool(double)>>>;

	public:

		struct Transition
		{
			String next;
			BoolConditional boolConditional;
			IntConditional intConditional;
			DoubleConditional doubleConditional;
			bool hasExit;
		};

	public:

		Animator2D();

		~Animator2D();

		void addBoolState(const String& _state, bool _default = false);

		void addIntState(const String& _state, int32 _default = 0);

		void addDoubleState(const String& _state, double _default = 0);

		bool removeBoolState(const String& _state);

		bool removeIntState(const String& _state);

		bool removeDoubleState(const String& _state);

		void setBool(const String& _state, bool _value);

		void setInt(const String& _state, int32 _value);

		void setDouble(const String& _state, double _value);

		bool getBool(const String& _state);

		uint32 getInt(const String& _state);

		double getDouble(const String& _state);

		Animation2D& addAnimation(const String& _state);

		Transition& addTransition(const String& _animState);

		Animation2D& getAnimation(const String& _state);

		const Animation2D& getAnimation(const String& _state)const;

		Array<Transition>& getTransition(const String& _state);

		const Array<Transition>& getTransition(const String& _state)const;

		void clear();

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
