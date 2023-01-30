#include <Components/GUI/Detail/Selectable.hpp>

#include <Framework/Actor.hpp>
#include <Common/InputAction.hpp>

namespace eagle::GUI::backend
{
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

		// Selectableにカーソルが乗っているか
		if (viewRect.contains(Cursor::PosF()) or mSelected)
		{
			// Selectableがクリックされているか
			if (InputAction(U"Clicked"))
			{
				mState = State::Click;
			}
			// Selectableが押され続けているかどうか
			else if (InputAction(U"Pressed"))
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
					mState = State::Hover;
				}
			}
		}
		// 通常時の処理
		else
		{
			if (not InputAction(U"Pressed"))
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
					mState = State::Neutral;
				}
			}
			// 対応しているボタンを押したまま離れた
			else
			{
				mState = State::Neutral;
			}
		}

		mSelected = false;

		switch (mState)
		{
		case State::Neutral: onNeutral(); break;
		case State::Hover: onHovered(); break;
		case State::Click: onClicked(); break;
		case State::Pressed: onPressed(); break;
		case State::Released: onReleased(); break;
		[[unlikely]] default: break;
		}
	}
}
