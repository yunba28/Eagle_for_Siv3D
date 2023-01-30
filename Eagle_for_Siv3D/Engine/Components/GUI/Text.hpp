#pragma once

#include <Framework/Renderer2D.hpp>
#include <Components/GUI/Detail/IWidget.hpp>

namespace eagle::GUI
{
	enum class TextAligne : uint8
	{
		TopLeft,
		BottomLeft,
		TopRight,
		BottomRight,
		Center,
		TopCenter,
		BottomCenter,
		LeftCenter,
		RightCenter
	};

	class Text : public Renderer2D, public backend::IWidget
	{
	public:

		Text();

		virtual ~Text();

		virtual void setGUIRect(const GUIRect& _GUIRect)override;

		virtual void setRegion(RectF _region)override;

		virtual void setSize(SizeF _size)override;

		virtual void setAnchor(Vec2 _anchor)override;

		virtual void setPivot(Vec2 _pivot)override;

		void setText(const String& _text);

		void setFont(const Font& _font);

		void setColor(const Color& _color);

		void setTextAligne(TextAligne _textAligne)noexcept;

		const String& getText()const noexcept;

		const Font& getFont()const noexcept;

		const Color& getColor()const noexcept;

		TextAligne getTextAligne()const noexcept;

	private:

		virtual void draw()const override {};

		virtual void drawScreen()const override;

	private:

		String mText;

		Font mFont;

		Color mColor;

		TextAligne mTextAligne;

	};
}
