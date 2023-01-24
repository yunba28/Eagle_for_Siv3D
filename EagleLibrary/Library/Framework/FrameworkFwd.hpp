#pragma once

#include <typeindex>

namespace eagle
{
	class SceneObject;
	namespace Internal
	{
		class SceneObjectDetail;
	}

	class Actor;
	class Component;

	using TypeInfo = std::type_index;

	using std::enable_if_t;
	using std::is_base_of_v;
}