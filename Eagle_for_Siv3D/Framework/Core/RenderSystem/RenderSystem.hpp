#pragma once

#include <Core/CoreFwd.hpp>
#include <Utility/SmartObject.hpp>

namespace eagle::Internal
{
	class RenderSystem final
	{
	public:

		RenderSystem();

		void update();

		void draw()const;

		void add(const WeakObject<DrawableComponent2D>& _component);

		void add(const WeakObject<DrawableComponent3D>& _component);

		WeakObject<Camera2D> getCamera2D()const noexcept;

		WeakObject<DebugCamera3D> getCamera3D()const noexcept;

		void setBackgroundColor(const ColorF& _color);

		const ColorF& getBackgroundColor()const;

		void notifyChange2DPriority();

	private:

		void draw2d()const;

		void draw3d()const;

	private:

		struct
		{
			Array<WeakObject<DrawableComponent2D>> list;
			Array<WeakObject<DrawableComponent2D>> pending;
			SharedObject<Camera2D> camera;
			mutable bool needRemove;
			bool needSort;
		}m2D;

		struct
		{
			Array<WeakObject<DrawableComponent3D>> list;
			Array<WeakObject<DrawableComponent3D>> pending;
			SharedObject<DebugCamera3D> camera;
			mutable bool needRemove;
		}m3D;

		MSRenderTexture mRenderTexture;

		ColorF mBackgroundColor;
	};
}
