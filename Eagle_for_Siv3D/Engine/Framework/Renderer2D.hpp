#pragma once

#include <Framework/Component.hpp>

namespace eagle
{
	class Renderer2D : public Component
	{
	public:

		struct DrawEnable
		{
			constexpr DrawEnable()
				: draw(true)
				, drawScreen(true)
			{}

			constexpr DrawEnable(bool _draw, bool _drawScreen)
				: draw(_draw)
				, drawScreen(_drawScreen)
			{}

			/// @brief draw関数の有効・無効
			bool draw;

			/// @brief drawScreen関数の有効・無効
			bool drawScreen;
		};

	public:

		Renderer2D();

		virtual ~Renderer2D();

		void setDrawOrder(size_t _order);

		using Component::setEnable;

		void setEnable(Enable _enable, DrawEnable _drawEnable);

		size_t getDrawOrder()const noexcept;

		bool isDrawEnable()const noexcept;

		bool isDrawScreenEnable()const noexcept;

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

		virtual void drawScreen()const {}

		virtual void _internalDraw()const;

		virtual void _internalDrawScreen()const;

	private:

		size_t mDrawOrder;

		DrawEnable mDrawEnable;

	};
}
