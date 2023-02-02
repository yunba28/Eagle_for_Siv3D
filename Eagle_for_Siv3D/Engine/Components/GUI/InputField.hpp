#pragma once

#include <Components/GUI/Detail/Selectable.hpp>
#include <Framework/Renderer2D.hpp>

namespace eagle::GUI
{
	class InputField : public backend::Selectable
	{
	private:

		struct Edit
		{
			String text{ U"" };
			size_t cursorPos{ 0 };
			bool active = false;
			bool textChanged = false;
			bool tabKey = false;
			bool enterKey = false;
			Stopwatch leftPressStopwatch{};
			Stopwatch rightPressStopwatch{};
			Stopwatch cursorStopwatch{};

# if SIV3D_PLATFORM(WEB)
			size_t lastCursorPos = 0;
# endif

			void clear()noexcept;

		};

		class Renderer final : public Renderer2D
		{
		public:

			Renderer();

			~Renderer();

		private:

			void drawScreen()const override final;

		public:

			Edit edit;

			Font font;

		};

	public:

		struct ColorBlock
		{
			Color neutral{ 255,255,255,64 };
			Color hovered{ 255,255,255,128 };
			Color pressed{ 192,192,192,128 };
			Color disable{ 64,64,64,64 };
		};

	public:

		InputField();

		virtual ~InputField();

		void setDrawOrder(size_t _order);

		virtual void setGUIRect(const GUIRect& _GUIRect)override;

		virtual void setRegion(RectF _region)override;

		virtual void setSize(SizeF _size)override;

		virtual void setAnchor(Vec2 _anchor)override;

		virtual void setPivot(Vec2 _pivot)override;

		ObjectHandle<class Background> getBackground()const noexcept;

	private:

		ObjectHandle<Renderer> getRenderer()const noexcept;

	private:

		virtual void awake()override;

		virtual void update()override;

		virtual void onEnable()override;

		virtual void onDisable()override;

		virtual void onHovered()override;

		virtual void onUnhovered()override;

		virtual void onClicked()override;

	private:

		WeakObject<class Background> mBackground;

		WeakObject<Renderer> mRenderer;

		ColorBlock mColorBlock;

	};
}
