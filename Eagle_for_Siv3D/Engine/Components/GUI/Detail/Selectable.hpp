#pragma once

#include <Framework/Behavior.hpp>
#include <Components/GUI/Detail/IWidget.hpp>

namespace eagle::GUI::backend
{
	class Selectable : public Behavior, public IWidget
	{
	public:

		enum class State
		{
			Neutral,
			Hover,
			Unhover,
			Click,
			Pressed,
			Released
		};

	public:

		Selectable();

		virtual ~Selectable();

		virtual void setGUIRect(const GUIRect& _GUIRect)override;

		virtual void setRegion(RectF _region)override;

		virtual void setSize(SizeF _size)override;

		virtual void setAnchor(Vec2 _anchor)override;

		virtual void setPivot(Vec2 _pivot)override;

		virtual void onSelect();

		bool isSelected()const noexcept;

	public:

		static void SetClickedAction(const String& actionState);

		static void SetPressedAction(const String& actionState);

		static const String& GetClickedAction();

		static const String& GetPressedAction();

	protected:

		/// @brief 通常時のSelectable更新関数
		virtual void onNeutral() {};

		/// @brief カーソルが重なっている間のSelectable更新関数
		virtual void onHovered() {};

		/// @brief カーソルがSelectableから離れた瞬間の更新関数
		virtual void onUnhovered() {};

		/// @brief Selectableがクリックされた瞬間の更新関数
		virtual void onClicked() {};

		/// @brief Selectableが押され続けている間の更新関数
		virtual void onPressed() {};

		/// @brief Selectableが離された瞬間の更新処理
		virtual void onReleased() {};

		virtual void update()override;

	private:

		State mState;

		bool mSelected;

		static String sClickedAction;

		static String sPressedAction;

	};
}
