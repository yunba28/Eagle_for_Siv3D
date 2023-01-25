#pragma once

#include <Framework/Component.hpp>

namespace eagle
{
	class Renderer2D : public Component
	{
	public:

		Renderer2D();

		virtual ~Renderer2D();

		void setDrawOrder(size_t _order);

		size_t getDrawOrder()const noexcept;

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

	private:

		size_t mDrawOrder;

	};
}
