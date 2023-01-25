#pragma once

#include <Framework/FrameworkFwd.hpp>
#include <Common/SmartObject.hpp>

namespace eagle::backend
{
	class RenderSystem final
	{
	public:

	private:

		struct
		{
			Array<WeakObject<Renderer2D>> list;
			Array<WeakObject<Renderer2D>> queue;
			mutable bool notifyRemove;
			bool notifySort;
		};

	};
}
