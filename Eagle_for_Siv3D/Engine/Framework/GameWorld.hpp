#pragma once

#include <Framework/BasicScene.hpp>

namespace eagle
{
	template<class State, class Data>
	class GameWorld : protected SceneManager<State, Data>
	{
	public:

		using Base_t = SceneManager<State, Data>;

		using Scene = BasicScene<State, Data>;

		using Base_t::SceneManager;

		using Base_t::add;

		using Base_t::init;

		using Base_t::update;

		using Base_t::get;

		using Base_t::changeScene;

		using Base_t::setFadeColor;

		using Base_t::getFadeColor;

		using Base_t::notifyError;

	};
}
