#include "RenderSystem.hpp"

#include <Core/RenderSystem/DrawableComponent.hpp>

namespace eagle::Internal
{
	RenderSystem::RenderSystem()
		: m2D{ .list{},.pending{},.camera{MakeShared<Camera2D>(Scene::CenterF())},.needRemove{false},.needSort{false} }
		, m3D{ .list{},.pending{},.camera{MakeShared<DebugCamera3D>(Scene::Size(),30_deg,Vec3{0,0,-5},Vec3{0,0,0})},.needRemove{false} }
		, mRenderTexture(Scene::Size(), HasDepth::Yes)
		, mBackgroundColor(Palette::DefaultBackground)
	{
	}

	void RenderSystem::update()
	{
		// 3D 削除
		if (m3D.needRemove)
		{
			auto compare = [](const WeakObject<DrawableComponent3D>& ref)
			{
				auto component = ref.lock();
				return (not component) or component->isPendingKill();
			};
			m3D.list.remove_if(compare);
			m3D.needRemove = false;
		}

		// 2D 削除
		if (m2D.needRemove)
		{
			auto compare = [](const WeakObject<DrawableComponent2D>& ref)
			{
				auto component = ref.lock();
				return (not component) or component->isPendingKill();
			};
			m2D.list.remove_if(compare);
			m2D.needRemove = false;
		}

		// 3D 追加
		if (m3D.pending)
		{
			m3D.list.append(m3D.pending);
			m3D.pending.clear();
		}

		// 2D 追加
		if (m2D.pending)
		{
			m2D.list.append(m2D.pending);
			m2D.pending.clear();
			m2D.needSort = true;
		}

		// 2D ソート
		if (m2D.needSort)
		{
			auto compare = [](const WeakObject<DrawableComponent2D>& a, const WeakObject<DrawableComponent2D>& b)
			{
				return a.lock()->getDrawOrder() < b.lock()->getDrawOrder();
			};

			m2D.list.stable_sort_by(compare);
			m2D.needSort = false;
		}
	}

	void RenderSystem::draw() const
	{
		// 3D 描画
		if (m3D.list)
		{
			draw3d();
		}

		// 2D 描画
		if (m2D.list)
		{
			draw2d();
		}
	}

	void RenderSystem::add(const WeakObject<DrawableComponent2D>& _component)
	{
		m2D.pending << _component;
	}

	void RenderSystem::add(const WeakObject<DrawableComponent3D>& _component)
	{
		m3D.pending << _component;
	}

	WeakObject<Camera2D> RenderSystem::getCamera2D() const noexcept
	{
		return m2D.camera.weak();
	}

	WeakObject<DebugCamera3D> RenderSystem::getCamera3D() const noexcept
	{
		return m3D.camera.weak();
	}

	void RenderSystem::setBackgroundColor(const ColorF& _color)
	{
		mBackgroundColor = _color;
	}

	const ColorF& RenderSystem::getBackgroundColor() const
	{
		return mBackgroundColor;
	}

	void RenderSystem::notifyChange2DPriority()
	{
		m2D.needSort = true;
	}

	void RenderSystem::draw2d() const
	{
		const auto transformer = m2D.camera->createTransformer();

		for (auto& ref : m2D.list)
		{
			auto component = ref.lock();
			if ((not component) or component->isPendingKill())
			{
				m2D.needRemove = true;
				continue;
			}
			component->_internalDraw();
		}
	}

	void RenderSystem::draw3d() const
	{
		Graphics3D::SetCameraTransform(*m3D.camera);

		//描画処理
		{
			// renderTexture を背景色で塗りつぶし、
			// renderTexture を 3D 描画のレンダーターゲットに
			const ScopedRenderTarget3D target{ mRenderTexture.clear(mBackgroundColor.removeSRGBCurve()) };

			for (auto& ref : m3D.list)
			{
				auto component = ref.lock();
				if ((not component) or component->isPendingKill())
				{
					m3D.needRemove = true;
					continue;
				}
				component->_internalDraw();
			}
		}

		//3D描画を2D描画に転写
		{
			// renderTexture を resolve する前に 3D 描画を実行する
			Graphics3D::Flush();

			// マルチサンプル・テクスチャのリゾルブ
			mRenderTexture.resolve();

			// リニアレンダリングされた renderTexture をシーンに転送
			Shader::LinearToScreen(mRenderTexture);
		}
	}
}
