#include <Components/GUI/Detail/Selectable.hpp>

#include <Framework/Actor.hpp>
#include <Common/InputAction.hpp>

namespace eagle::GUI::backend
{
	String Selectable::sClickedAction = U"Clicked";

	String Selectable::sPressedAction = U"Pressed";

	Selectable::Selectable()
		: mState(State::Neutral)
		, mSelected(false)
	{
	}

	Selectable::~Selectable()
	{
	}

	void Selectable::setGUIRect(const GUIRect& _GUIRect)
	{
		mGUIRect = _GUIRect;
	}

	void Selectable::setRegion(RectF _region)
	{
		mGUIRect.region = _region;
	}

	void Selectable::setSize(SizeF _size)
	{
		mGUIRect.size = _size;
	}

	void Selectable::setAnchor(Vec2 _anchor)
	{
		mGUIRect.anchor = _anchor;
	}

	void Selectable::setPivot(Vec2 _pivot)
	{
		mGUIRect.pivot = _pivot;
	}

	void Selectable::onSelect()
	{
		mSelected = true;
	}

	bool Selectable::isSelected() const noexcept
	{
		return mSelected;
	}

	void Selectable::update()
	{
		if (mGUIRect.region.isEmpty())
			return;

		const auto transform = getTransform();
		const auto pos = transform->getPos2D();
		const auto scale = transform->getScale2D();

		const auto viewRect = mGUIRect.getViewRect(pos, scale);

		const auto onRect = mGUIRect.region.mouseOver() and viewRect.mouseOver();

		// Selectableにカーソルが乗っているか
		if (onRect or mSelected)
		{
			// Selectableがクリックされているか
			if (InputAction(sClickedAction))
			{
				mState = State::Click;
			}
			// Selectableが押され続けているかどうか
			else if (InputAction(sPressedAction))
			{
				mState = State::Pressed;
			}
			// ボタンは押されていない
			else
			{
				// ボタンが離された瞬間
				if (mState == State::Pressed)
				{
					mState = State::Released;
				}
				// ボタンが話された瞬間（人間の反応速度的に恐らくあり得ない）
				else if (mState == State::Click) [[unlikely]]
				{
					mState = State::Released;
				}
				else
				{
					mState = onRect ? State::Hover : State::Unhover;
				}
			}
		}
		// 通常時の処理
		else
		{
			if (mState == State::Unhover)
			{
				mState = State::Neutral;
			}
			else if (mState != State::Neutral)
			{
				mState = State::Unhover;
			}
		}

		mSelected = false;

		switch (mState)
		{
		case State::Neutral: onNeutral(); break;
		case State::Hover: onHovered(); break;
		case State::Unhover: onUnhovered(); break;
		case State::Click: onClicked(); break;
		case State::Pressed: onPressed(); break;
		case State::Released: onReleased(); break;
		[[unlikely]] default: break;
		}
	}

	void Selectable::SetClickedAction(const String& actionState)
	{
		sClickedAction = actionState;
	}

	void Selectable::SetPressedAction(const String& actionState)
	{
		sPressedAction = actionState;
	}

	const String& Selectable::GetClickedAction()
	{
		return sClickedAction;
	}

	const String& Selectable::GetPressedAction()
	{
		return sPressedAction;
	}
}
