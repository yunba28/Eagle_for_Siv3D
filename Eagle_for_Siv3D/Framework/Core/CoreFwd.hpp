#pragma once

#include <Siv3D.hpp>
#include <Utility/SmartObject.hpp>

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

	class Collision;

	using Scene_ref = WeakObject<Internal::ScenePimpl>;

	using Scene_handle = ObjectHandle<Internal::ScenePimpl>;

	using Actor_ref = WeakObject<Actor>;

	using Actor_handle = ObjectHandle<Actor>;

	using Component_ref = WeakObject<Component>;

	using Component_handle = ObjectHandle<Component>;

	using Transform_ref = WeakObject<Transform>;

	using Transform_handle = ObjectHandle<Transform>;
}
