#pragma once

#include <Framework/Renderer2D.hpp>
#include <Components/GUI/Detail/IWidget.hpp>

namespace eagle::GUI
{
	class Background : public Renderer2D, public backend::IWidget
	{
	public:

		Background();

		virtual ~Background();

		virtual void setGUIRect(const GUIRect& _GUIRect)override;

		virtual void setRegion(RectF _region)override;

		virtual void setSize(SizeF _size)override;

		virtual void setAnchor(Vec2 _anchor)override;

		virtual void setPivot(Vec2 _pivot)override;

		void setTexture(const TextureRegion& _texture);

		void setColor(const Color& _color);

		const TextureRegion& getTexture()const noexcept;

		const Color& getColor()const noexcept;

	private:

		virtual void draw()const override {}

		virtual void drawScreen()const override;

	private:

		TextureRegion mTextureRegion;

		Color mColor;

	};
}
