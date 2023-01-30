#pragma once

#include <Components/GUI/Detail/GUIRect.hpp>

namespace eagle::GUI::backend
{
	class IWidget
	{
	public:

		virtual void setGUIRect(const GUIRect&) = 0;

		virtual void setRegion(RectF) = 0;

		virtual void setSize(SizeF) = 0;

		virtual void setAnchor(Vec2) = 0;

		virtual void setPivot(Vec2) = 0;

		const GUIRect& getGUIRect()const noexcept { return mGUIRect; }

		RectF getRegion()const noexcept { return mGUIRect.region; }

		SizeF getSize()const noexcept { return mGUIRect.size; }

		Vec2 getAnchor()const noexcept { return mGUIRect.anchor; }

		Vec2 getPivot()const noexcept { return mGUIRect.pivot; }

	protected:

		GUIRect mGUIRect;

	};
}
