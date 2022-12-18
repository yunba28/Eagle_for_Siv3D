#pragma once

#include <Siv3D.hpp>

namespace eagle
{
	struct EmptyData {};

	template<class Data = EmptyData>
	class GameWorld;
	template<class Data = EmptyData>
	class BasicScene;

	class Actor;
	class Component;

	class Transform;

	enum class Space;

	class DrawableComponent2D;
	class DrawableComponent3D;

	namespace Internal
	{
		class ScenePimpl;
		class ActorSystem;
		class ComponentSystem;
		class FixedSystem;
		class RenderSystem;
	}

	using DefaultWorld = GameWorld<EmptyData>;

	using TypeID = std::type_index;
}
