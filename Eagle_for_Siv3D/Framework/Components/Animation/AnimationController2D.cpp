#include "AnimationController2D.hpp"

#include <Components/Transform.hpp>
#include <Utility/Load.hpp>

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

	Optional<Animation2D> AnimationController2D::getAnimation(const String& _state) const
	{
		if (mAnimations.contains(_state))
		{
			return mAnimations.at(_state);
		}

		return Optional<Animation2D>{unspecified};
	}

	Array<AnimationController2D::Transition> AnimationController2D::getTransition(const String& _state) const
	{
		if (mTransitions.contains(_state))
		{
			return mTransitions.at(_state);
		}

		return Array<Transition>{};
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

	bool AnimationController2D::LoadBoolState(const INISection& data, AnimationController2D& controller)
	{
		for (auto& key : data.keys)
		{
			controller.addBoolState(key.name, ParseOr<bool>(key.value, false));
		}

		return true;
	}

	bool AnimationController2D::LoadIntState(const INISection& data, AnimationController2D& controller)
	{
		for (auto& key : data.keys)
		{
			controller.addIntState(key.name, ParseOr<int32>(key.value, 0));
		}

		return true;
	}

	bool AnimationController2D::LoadDoubleState(const INISection& data, AnimationController2D& controller)
	{
		for (auto& key : data.keys)
		{
			controller.addDoubleState(key.name, ParseOr<double>(key.value, 0.0));
		}

		return true;
	}

	bool AnimationController2D::LoadAnimation(const INISection& data, AnimationController2D& controller)
	{
		if (data.keys.size() < 8)
			return false;

		String name{};
		String assetTag{};
		Point division{};
		Point range{};
		bool loop{};
		Vec2 pivot{};
		Vec2 offset{};
		Duration duration{};
		Color color{Palette::White};

		try
		{
			name = data.keys[0].value;
			assetTag = data.keys[1].value;
			division = Parse<Point>(data.keys[2].value);
			range = Parse<Point>(data.keys[3].value);
			loop = ParseBool(data.keys[4].value);
			pivot = Parse<Vec2>(data.keys[5].value);
			offset = Parse<Vec2>(data.keys[6].value);
			duration = Duration{ ParseFloat<double>(data.keys[7].value) };
			color = Parse<Color>(data.keys[8].value);
		}
		catch (ParseError&)
		{
			return false;
		}

		auto& anim = controller.addAnimation(name);
		anim.setTextureAssetTag(assetTag);
		anim.setParam({ division,range,loop });
		anim.setPivot(pivot);
		anim.setOffset(offset);
		anim.setDuration(duration);
		anim.setDiffuseColor(color);

		return true;
	}

	bool AnimationController2D::LoadTransition(const INISection& data, AnimationController2D& controller)
	{
		if (data.keys.size() < 4)
			return false;

		String current{};
		String next{};
		bool hasExit{};
		BoolConditional boolConditional{};
		IntConditional intConditional{};
		DoubleConditional doubleConditional{};

		try
		{
			current = data.keys[0].value;
			next = data.keys[1].value;
			hasExit = ParseBool(data.keys[2].value);

			for (int32 i = 3; i < data.keys.size(); ++i)
			{
				auto& key = data.keys[i];
				auto [f, val] = GetStringPair(key.value);

				if (f == U"BoolEq")
				{
					boolConditional << std::make_pair(key.name, EqualTo{ ParseBool(val) });
				}
				else if (f == U"BoolLess")
				{
					boolConditional << std::make_pair(key.name, LessTo{ ParseBool(val) });
				}
				else if (f == U"BoolGreater")
				{
					boolConditional << std::make_pair(key.name, GreaterTo{ ParseBool(val) });
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

		auto& transition = controller.addTransition(current);
		transition.next = next;
		transition.hasExit = hasExit;
		transition.boolConditional = std::move(boolConditional);
		transition.intConditional = std::move(intConditional);
		transition.doubleConditional = std::move(doubleConditional);

		return true;
	}

	bool AnimationController2D::SaveBoolState(INI& ini, AnimationController2D& controller)
	{
		ini.addSection(U"State:Bool");

		for (const auto& [name, value] : controller.mBoolState)
		{
			ini.write(U"State:Bool", name, value);
		}

		return true;
	}

	bool AnimationController2D::SaveIntState(INI& ini, AnimationController2D& controller)
	{
		ini.addSection(U"State:Int");

		for (const auto& [name, value] : controller.mIntState)
		{
			ini.write(U"State:Int", name, value);
		}

		return true;
	}

	bool AnimationController2D::SaveDoubleState(INI& ini, AnimationController2D& controller)
	{
		ini.addSection(U"State:Double");

		for (const auto& [name, value] : controller.mDoubleState)
		{
			ini.write(U"State:Double", name, value);
		}

		return true;
	}

	bool AnimationController2D::SaveAnimation(INI& ini, AnimationController2D& controller)
	{
		for (const auto& [name, anim] : controller.mAnimations)
		{
			auto section = U"Animation:{}"_fmt(name.removed(U'.'));
			const auto& param = anim.getParam();
			ini.addSection(section);
			ini.write(section, U"Name", name);
			ini.write(section, U"AssetTag", anim.getTextureAssetTag());
			ini.write(section, U"Division", param.division);
			ini.write(section, U"Range", param.range);
			ini.write(section, U"Loop", param.hasLoop);
			ini.write(section, U"Pivot", anim.getPivot());
			ini.write(section, U"Offset", anim.getOffset());
			ini.write(section, U"Duration", anim.getDuration().count());
			ini.write(section, U"DiffuseColor", anim.getDiffuseColor());
		}

		return true;
	}

	bool AnimationController2D::SaveTransition(INI& ini, AnimationController2D& controller)
	{
		for (const auto& [current,transitions] : controller.mTransitions)
		{
			for (const auto& transition : transitions)
			{
				const auto section = U"Transition:{}_to_{}"_fmt(current.removed(U'.'), transition.next.removed(U'.'));
				ini.addSection(section);
				ini.write(section, U"Current", current);
				ini.write(section, U"Next", transition.next);
				ini.write(section, U"HasExit", transition.hasExit);
				for (const auto& [state,f] : transition.boolConditional)
				{
					const auto& target = f.target_type();
					String compare{};
					if (target == typeid(EqualTo<bool>))
					{
						compare = U"BoolEq";
						ini.write(section, state, U"({},{})"_fmt(compare, f.target<EqualTo<bool>>()->value));
					}
					else if (target == typeid(LessTo<bool>))
					{
						compare = U"BoolLess";
						ini.write(section, state, U"({},{})"_fmt(compare, f.target<LessTo<bool>>()->value));
					}
					else if (target == typeid(LessTo<bool>))
					{
						compare = U"BoolGreater";
						ini.write(section, state, U"({},{})"_fmt(compare, f.target<LessTo<bool>>()->value));
					}
				}

				for (const auto& [state, f] : transition.intConditional)
				{
					const auto& target = f.target_type();
					String compare{};
					if (target == typeid(EqualTo<int32>))
					{
						compare = U"IntEq";
						ini.write(section, state, U"({},{})"_fmt(compare, f.target<EqualTo<int32>>()->value));
					}
					else if (target == typeid(LessTo<int32>))
					{
						compare = U"IntLess";
						ini.write(section, state, U"({},{})"_fmt(compare, f.target<LessTo<int32>>()->value));
					}
					else if (target == typeid(LessTo<int32>))
					{
						compare = U"IntGreater";
						ini.write(section, state, U"({},{})"_fmt(compare, f.target<LessTo<int32>>()->value));
					}
				}

				for (const auto& [state, f] : transition.doubleConditional)
				{
					const auto& target = f.target_type();
					String compare{};
					if (target == typeid(EqualTo<double>))
					{
						compare = U"DoubleEq";
						ini.write(section, state, U"({},{})"_fmt(compare, f.target<EqualTo<double>>()->value));
					}
					else if (target == typeid(LessTo<double>))
					{
						compare = U"DoubleLess";
						ini.write(section, state, U"({},{})"_fmt(compare, f.target<LessTo<double>>()->value));
					}
					else if (target == typeid(LessTo<double>))
					{
						compare = U"DoubleGreater";
						ini.write(section, state, U"({},{})"_fmt(compare, f.target<LessTo<double>>()->value));
					}
				}
			}
		}

		return true;
	}

	std::pair<String, String> AnimationController2D::GetStringPair(const String& data)
	{
		String trimmed = data.removed_if([](const char32_t& c) {return c == U' ' or c == U'(' or c == U')'; });

		auto it = std::find(trimmed.begin(), trimmed.end(), U',');

		return std::make_pair(String{ trimmed.begin(),it }, String{ it + 1,trimmed.end() });
	}

	template<>
	bool Load<AnimationController2D>(const String& path, AnimationController2D& controller)
	{
		if (FileSystem::Extension(path) != U"anim")
			return false;

		INI ini{ path };

		if (ini.isEmpty())
			return false;

		bool success = true;

		for (const auto& data : ini.sections())
		{
			if (data.section.includes(U"State:Bool"))
			{
				success |= AnimationController2D::LoadBoolState(data, controller);
			}
			else if (data.section.includes(U"State:Int"))
			{
				success |= AnimationController2D::LoadIntState(data, controller);
			}
			else if (data.section.includes(U"State:Double"))
			{
				success |= AnimationController2D::LoadDoubleState(data, controller);
			}
			else if (data.section.includes(U"Animation:"))
			{
				success |= AnimationController2D::LoadAnimation(data, controller);
			}
			else if (data.section.includes(U"Transition:"))
			{
				success |= AnimationController2D::LoadTransition(data, controller);
			}

			if (not success)
			{
				controller.clear();
				return false;
			}
		}

		return success;
	}

	template<>
	bool Save<AnimationController2D>(const String& path, AnimationController2D& controller)
	{
		String out{ path };

		if (FileSystem::Extension(out) != U"anim")
		{
			auto it = std::find(out.begin(), out.end(), U'.');
			out.erase(it + 1, out.end());
			out += U"anim";
		}

		INI ini{};

		AnimationController2D::SaveBoolState(ini, controller);
		AnimationController2D::SaveIntState(ini, controller);
		AnimationController2D::SaveDoubleState(ini, controller);
		AnimationController2D::SaveAnimation(ini, controller);
		AnimationController2D::SaveTransition(ini, controller);

		return ini.save(path);
	}
}
