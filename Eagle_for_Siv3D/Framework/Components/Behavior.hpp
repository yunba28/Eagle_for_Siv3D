#pragma once

#include <Framework/Component.hpp>

#include "../Fwd.hpp"

namespace eagle
{
	class Behavior : public Component
	{
	public:

		Behavior();

		virtual ~Behavior() = default;

		ObjectHandle<Transform> getTransform()const noexcept;

	protected:

		virtual void awake()override {}

		virtual void dispose()override {}

		virtual void start()override {}

		virtual void update()override {}

		virtual void lateUpdate()override {}

		virtual void fixedUpdate()override {}

		virtual void onEnable()override {}

		virtual void onDisable()override {}

		virtual void _internalAwake()override;

	private:

		WeakObject<Transform> mTransform;

	};
}
