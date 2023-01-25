#pragma once

#include <Framework/Component.hpp>

namespace eagle
{
	class Behavior : public Component
	{
	public:

		Behavior() = default;

		virtual ~Behavior() = default;

	protected:

		virtual void awake()override {}

		virtual void dispose()override {}

		virtual void start()override {}

		virtual void lateUpdate()override {}

		virtual void fixedUpdate()override {}

		virtual void onEnable()override {}

		virtual void onDisable()override {}

	};
}
