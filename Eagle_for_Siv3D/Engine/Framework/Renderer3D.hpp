#pragma once

#include <Framework/Component.hpp>

namespace eagle
{
	class Renderer3D : public Component
	{
	public:

		Renderer3D();

		virtual ~Renderer3D();

	private:

		virtual void awake()override {}

		virtual void dispose()override {}

		virtual void start()override {}

		virtual void update()override {}

		virtual void lateUpdate()override {}

		virtual void fixedUpdate()override {}

		virtual void onEnable()override {}

		virtual void onDisable()override {}

		virtual void _internalAwake()override;

	public:

		virtual void draw()const = 0;

	};
}
