#include <Components/GUI/Text.hpp>

#include <Framework/Actor.hpp>

namespace eagle::GUI
{
	Text::Text()
		: mText(U"New Text")
		, mFont(SimpleGUI::GetFont())
		, mColor(Palette::Black)
		, mTextAligne(TextAligne::Center)
	{
		setEnable({ false,false,false }, { false,true });

		setGUIRect({ Scene::Rect(),SizeF{200,50} });
	}

	Text::~Text()
	{
	}

	void Text::setGUIRect(const GUIRect& _GUIRect)
	{
		mGUIRect = _GUIRect;
	}

	void Text::setRegion(RectF _region)
	{
		mGUIRect.region = _region;
	}

	void Text::setSize(SizeF _size)
	{
		mGUIRect.size = _size;
	}

	void Text::setAnchor(Vec2 _anchor)
	{
		mGUIRect.anchor = _anchor;
	}

	void Text::setPivot(Vec2 _pivot)
	{
		mGUIRect.pivot = _pivot;
	}

	void Text::setText(const String& _text)
	{
		mText = _text;
	}

	void Text::setFont(const Font& _font)
	{
		mFont = _font;
	}

	void Text::setColor(const Color& _color)
	{
		mColor = _color;
	}

	void Text::setTextAligne(TextAligne _textAligne) noexcept
	{
		mTextAligne = _textAligne;
	}

	const String& Text::getText() const noexcept
	{
		return mText;
	}

	const Font& Text::getFont() const noexcept
	{
		return mFont;
	}

	const Color& Text::getColor() const noexcept
	{
		return mColor;
	}

	TextAligne Text::getTextAligne() const noexcept
	{
		return mTextAligne;
	}

	void Text::drawScreen() const
	{
		if (mGUIRect.region.isEmpty())
			return;

		const ScopedViewport2D viewport{ mGUIRect.region.asRect() };

		const auto transform = getTransform();
		const auto pos = transform->getPos2D();
		const auto scale = transform->getScale2D();

		const auto viewRect = mGUIRect.getViewRect(pos, scale);

		const auto text = mFont(mText);

		switch (mTextAligne)
		{
		case TextAligne::TopLeft:
			text.draw(viewRect.pos, mColor);
			break;
		case TextAligne::BottomLeft:
			text.draw(Arg::bottomLeft = viewRect.bl(), mColor);
			break;
		case TextAligne::TopRight:
			text.draw(Arg::topRight = viewRect.tr(), mColor);
			break;
		case TextAligne::BottomRight:
			text.draw(Arg::bottomRight = viewRect.br(), mColor);
			break;
		case TextAligne::Center:
			text.drawAt(viewRect.center(), mColor);
			break;
		case TextAligne::TopCenter:
			text.draw(Arg::topCenter = viewRect.topCenter(), mColor);
			break;
		case TextAligne::BottomCenter:
			text.draw(Arg::bottomCenter = viewRect.bottomCenter(), mColor);
			break;
		case TextAligne::LeftCenter:
			text.draw(Arg::leftCenter = viewRect.leftCenter(), mColor);
			break;
		case TextAligne::RightCenter:
			text.draw(Arg::rightCenter = viewRect.rightCenter(), mColor);
			break;
		[[unlikely]] default: break;
		}
	}
	
}
