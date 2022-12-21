#pragma once

#include "Animation2D.hpp"
#include "Compares.hpp"

namespace eagle
{
	class AnimationController2D : public DrawableComponent2D
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

		Optional<Animation2D> getAnimation(const String& _state)const;

		Array<Transition> getTransition(const String& _state)const;

		void clear();

	private:

		void start()override;

		void update()override;

		void draw()const override;

		bool conditionUpdate();

		static bool LoadBoolState(const INISection& data, AnimationController2D& controller);
		static bool LoadIntState(const INISection& data, AnimationController2D& controller);
		static bool LoadDoubleState(const INISection& data, AnimationController2D& controller);
		static bool LoadAnimation(const INISection& data, AnimationController2D& controller);
		static bool LoadTransition(const INISection& data, AnimationController2D& controller);

		static bool SaveBoolState(INI& ini, AnimationController2D& controller);
		static bool SaveIntState(INI& ini, AnimationController2D& controller);
		static bool SaveDoubleState(INI& ini, AnimationController2D& controller);
		static bool SaveAnimation(INI& ini, AnimationController2D& controller);
		static bool SaveTransition(INI& ini, AnimationController2D& controller);

		static std::pair<String, String> GetStringPair(const String& data);

	private:

		HashTable<String, bool> mBoolState;

		HashTable<String, int32> mIntState;

		HashTable<String, double> mDoubleState;

		HashTable<String, Animation2D> mAnimations;

		HashTable<String, Array<Transition>> mTransitions;

		String mCurrentState;

		Optional<String> mNextState;

		template<class Type>
		friend bool Load(const String& path, Type& controller);

		template<class Type>
		friend bool Save(const String& path, Type& controller);
	};
}
