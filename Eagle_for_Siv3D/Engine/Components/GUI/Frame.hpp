#pragma once

#include <Framework/Renderer2D.hpp>
#include <Components/GUI/Detail/IWidget.hpp>

namespace eagle::GUI
{
	class Frame : public Renderer2D, public backend::IWidget
	{
	public:

		Frame();

		virtual ~Frame();

		virtual void setGUIRect(const GUIRect& _GUIRect)override;

		virtual void setRegion(RectF _region)override;

		virtual void setSize(SizeF _size)override;

		virtual void setAnchor(Vec2 _anchor)override;

		virtual void setPivot(Vec2 _pivot)override;

		void setColor(const Color& _color)noexcept;

		void setThickness(double _thickness)noexcept;

		void setThickness(double _innerThickness, double _outerThickness)noexcept;

		void setThickness(Vec2 _thickness)noexcept;

		const Color& getColor()const noexcept;

		Vec2 getThickness()const noexcept;

	private:

		virtual void draw()const override {}

		virtual void drawScreen()const override;

	private:

		Color mColor;

		struct
		{
			double inner{ 1.0 };
			double outer{ 1.0 };
		}mThickness;

	};
}
