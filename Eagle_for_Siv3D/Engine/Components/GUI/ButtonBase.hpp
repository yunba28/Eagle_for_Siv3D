#pragma once

#include <Components/GUI/Detail/Selectable.hpp>

namespace eagle::GUI
{
	class ButtonBase : public backend::Selectable
	{
	public:

		struct ColorPalette
		{
			Color neutral{ 255,255,255,64 };
			Color hovered{ 255,255,255,128 };
			Color pressed{ 192,192,192,128 };
			Color disable{ 64,64,64,64 };
		};

		using Super = ButtonBase;

	public:

		ButtonBase();

		virtual ~ButtonBase();

		void setDrawOrder(size_t _order);

		virtual void setGUIRect(const GUIRect& _GUIRect)override;

		virtual void setRegion(RectF _region)override;

		virtual void setSize(SizeF _size)override;

		virtual void setAnchor(Vec2 _anchor)override;

		virtual void setPivot(Vec2 _pivot)override;

		void setColorPallete(const ColorPalette& _colorPalette);

		ObjectHandle<class Background> getBackground()const noexcept;

		const ColorPalette& getColorPalette()const noexcept;

	protected:

		virtual void awake()override;

		virtual void onEnable()override;

		virtual void onDisable()override;

		virtual void onNeutral()override;

		virtual void onHovered()override;

		virtual void onClicked()override;

		virtual void onPressed()override;

		virtual void onReleased()override;

	private:

		WeakObject<class Background> mBackground;

		ColorPalette mColorPalette;

	};
}
