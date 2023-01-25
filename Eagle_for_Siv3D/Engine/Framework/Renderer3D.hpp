#pragma once

#include <Framework/Component.hpp>

namespace eagle
{
	class Renderer3D : public Component
	{
	public:

		struct DrawEnable
		{
			constexpr DrawEnable()
				: draw(true)
			{}

			constexpr DrawEnable(bool _draw)
				: draw(_draw)
			{}

			/// @brief draw関数の有効無効
			bool draw;
		};

	public:

		Renderer3D();

		virtual ~Renderer3D();

		using Component::setEnable;

		void setEnable(Enable _enable, DrawEnable _drawEnable);

		bool isDrawEnable()const noexcept;

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

		virtual void _internalDraw();

	private:

		DrawEnable mDrawEnable;

	};
}
