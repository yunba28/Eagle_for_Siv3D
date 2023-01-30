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

		void onSelect();

		bool isSelected()const noexcept;

	protected:

		/// @brief 通常時のSelectable更新関数
		virtual void onNeutral() = 0;

		/// @brief カーソルが重なっている間のSelectable更新関数
		virtual void onHovered() = 0;

		/// @brief Selectableがクリックされた瞬間の更新関数
		virtual void onClicked() = 0;

		/// @brief Selectableが押され続けている間の更新関数
		virtual void onPressed() = 0;

		/// @brief Selectableが離された瞬間の更新処理
		virtual void onReleased() = 0;

		virtual void update()override;

	private:

		State mState;

		bool mSelected;

	};
}
