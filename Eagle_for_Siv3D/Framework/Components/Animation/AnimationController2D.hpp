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

		void clear();

	private:

		void start()override;

		void update()override;

		void draw()const override;

		bool conditionUpdate();

		bool loadAnim(const FilePath& _path);

		bool readBoolState(const INISection& _data);

		bool readIntState(const INISection& _data);

		bool readDoubleState(const INISection& _data);

		bool readAnimation(const INISection& _data);

		bool readTransition(const INISection& _data);

		std::pair<String, String> getStringPair(const String& _data);

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
	};
}
