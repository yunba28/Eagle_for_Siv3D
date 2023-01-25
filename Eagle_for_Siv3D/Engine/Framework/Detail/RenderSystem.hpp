#pragma once

#include <Framework/FrameworkFwd.hpp>
#include <Common/SmartObject.hpp>

#include <Siv3D/Camera2D.hpp>
#include <Siv3D/DebugCamera3D.hpp>
#include <Siv3D/MSRenderTexture.hpp>
#include <Siv3D/Color.hpp>

using s3d::Camera2D;


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
		}m2D;

		MSRenderTexture mRenderTexture;

		ColorF mBackgroundColor;
	};
}
