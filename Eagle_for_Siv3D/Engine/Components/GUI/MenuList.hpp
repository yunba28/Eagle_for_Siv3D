#pragma once

#include <Components/GUI/Detail/Selectable.hpp>

namespace eagle::GUI
{

	class MenuList : public backend::Selectable
	{
	public:

		MenuList();

		virtual ~MenuList();



	protected:



	private:

		WeakObject<class Background> mBackground;

		Array<WeakObject<backend::Selectable>> mItems;

	};
}
