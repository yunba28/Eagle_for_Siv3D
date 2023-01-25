#pragma once

#include <Framework/FrameworkFwd.hpp>
#include <Common/SmartObject.hpp>

#include <Siv3D/Camera2D.hpp>
#include <Siv3D/DebugCamera3D.hpp>
#include <Siv3D/MSRenderTexture.hpp>
#include <Siv3D/Color.hpp>

using s3d::Camera2D;
using s3d::DebugCamera3D;
using s3d::MSRenderTexture;
using s3d::Color;

namespace eagle::backend
{
	class RenderSystem final
	{
	public:

		RenderSystem();

		~RenderSystem();

		void update();

		void draw()const;

		void add(const WeakObject<Renderer2D>& _renderer);

		void add(const WeakObject<Renderer3D>& _renderer);

		void notifyChageDrawOrder();

		void setBackgroundColor(const Color& _background);

		WeakObject<Camera2D> getCamera2D()const noexcept;

		WeakObject<DebugCamera3D> getCamera3D()const noexcept;

		const Color& getBackgroundColor()const noexcept;

	private:

		void draw2D()const;

		void draw3D()const;

	private:

		struct
		{
			Array<WeakObject<Renderer2D>> list;
			Array<WeakObject<Renderer2D>> queue;
			SharedObject<Camera2D> camera;
			mutable bool notifyRemove;
			bool notifySort;
		}m2D;

		struct
		{
			Array<WeakObject<Renderer3D>> list;
			Array<WeakObject<Renderer3D>> queue;
			SharedObject<DebugCamera3D> camera;
			mutable bool notifyRemove;
		}m3D;

		MSRenderTexture mRenderTexture;

		Color mBackgroundColor;
	};
}
