#include "AnimationController2D.hpp"

#include <Components/Transform.hpp>
#include <Utility/Loader/Load.hpp>

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

	void AnimationController2D::clear()
	{
		mAnimations.clear();
		mTransitions.clear();
		mBoolState.clear();
		mIntState.clear();
		mDoubleState.clear();
		mCurrentState = U"";
		mNextState = unspecified;
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

	bool AnimationController2D::loadAnim(const FilePath& _path)
	{
		if (FileSystem::Extension(_path) != U"anim")
			return false;

		INI ini{ _path };

		if (ini.isEmpty())
			return false;

		bool success = true;

		for (const auto& data : ini.sections())
		{
			if (data.section.includes(U"State:Bool"))
			{
				success |= readBoolState(data);
			}
			else if (data.section.includes(U"State:Int"))
			{
				success |= readIntState(data);
			}
			else if (data.section.includes(U"State:Double"))
			{
				success |= readDoubleState(data);
			}
			else if (data.section.includes(U"Animation:"))
			{
				success |= readAnimation(data);
			}
			else if (data.section.includes(U"Transition:"))
			{
				success |= readTransition(data);
			}

			if (not success)
			{
				clear();
				return false;
			}
		}

		return success;
	}

	bool AnimationController2D::readBoolState(const INISection& _data)
	{
		for (auto& key : _data.keys)
		{
			addBoolState(key.name, ParseOr<bool>(key.value, false));
		}

		return true;
	}

	bool AnimationController2D::readIntState(const INISection& _data)
	{
		for (auto& key : _data.keys)
		{
			addIntState(key.name, ParseOr<int32>(key.value, 0));
		}

		return true;
	}

	bool AnimationController2D::readDoubleState(const INISection& _data)
	{
		for (auto& key : _data.keys)
		{
			addDoubleState(key.name, ParseOr<double>(key.value, 0.0));
		}

		return true;
	}

	bool AnimationController2D::readAnimation(const INISection& _data)
	{
		if (_data.keys.size() < 8)
			return false;

		String name{};
		String assetTag{};
		Point division{};
		Point range{};
		bool loop{};
		Vec2 pivot{};
		Vec2 offset{};
		Duration duration{};

		try
		{
			name = _data.keys[0].value;
			assetTag = _data.keys[1].value;
			division = Parse<Point>(_data.keys[2].value);
			range = Parse<Point>(_data.keys[3].value);
			loop = ParseBool(_data.keys[4].value);
			pivot = Parse<Vec2>(_data.keys[5].value);
			offset = Parse<Vec2>(_data.keys[6].value);
			duration = Duration{ ParseFloat<double>(_data.keys[7].value) };
		}
		catch (ParseError&)
		{
			return false;
		}

		auto& anim = addAnimation(name);
		anim.setTexture(TextureAsset(assetTag));
		anim.setParam({ division,range,loop });
		anim.setPivot(pivot);
		anim.setOffset(offset);
		anim.setDuration(duration);

		return true;
	}

	bool AnimationController2D::readTransition(const INISection& _data)
	{
		if (_data.keys.size() < 4)
			return false;

		String current{};
		String next{};
		bool hasExit{};
		BoolConditional boolConditional{};
		IntConditional intConditional{};
		DoubleConditional doubleConditional{};

		try
		{
			current = _data.keys[0].value;
			next = _data.keys[1].value;
			hasExit = ParseBool(_data.keys[2].value);

			for (int32 i = 3; i < _data.keys.size();++i)
			{
				auto& key = _data.keys[i];
				auto [f,val] = getStringPair(key.value);

				if (f == U"BoolEq")
				{
					boolConditional << std::make_pair(key.name, EqualTo{ ParseBool(val) });
				}
				else if (f == U"IntEq")
				{
					intConditional << std::make_pair(key.name, EqualTo{ ParseInt<int32>(val) });
				}
				else if (f == U"IntLess")
				{
					intConditional << std::make_pair(key.name, LessTo{ ParseInt<int32>(val) });
				}
				else if (f == U"IntGreater")
				{
					intConditional << std::make_pair(key.name, GreaterTo{ ParseInt<int32>(val) });
				}
				else if (f == U"DoubleEq")
				{
					doubleConditional << std::make_pair(key.name, EqualTo{ ParseFloat<double>(val) });
				}
				else if (f == U"DoubleLess")
				{
					doubleConditional << std::make_pair(key.name, LessTo{ ParseFloat<double>(val) });
				}
				else if (f == U"DoubleGreater")
				{
					doubleConditional << std::make_pair(key.name, GreaterTo{ ParseFloat<double>(val) });
				}
			}
		}
		catch (ParseError&)
		{
			return false;
		}

		auto& transition = addTransition(current);
		transition.next = next;
		transition.hasExit = hasExit;
		transition.boolConditional = std::move(boolConditional);
		transition.intConditional = std::move(intConditional);
		transition.doubleConditional = std::move(doubleConditional);

		return true;
	}

	std::pair<String, String> AnimationController2D::getStringPair(const String& _data)
	{
		String data = _data.removed_if([](const char32_t& c) {return c == U' ' or c == U'(' or c == U')'; });

		auto it = std::find(data.begin(), data.end(), U',');

		return std::make_pair(String{ data.begin(),it }, String{ it + 1,data.end() });
	}

	template<>
	bool Load<AnimationController2D>(const String& path, AnimationController2D& controller)
	{
		return controller.loadAnim(path);
	}
}
