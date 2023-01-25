#include <Framework/Detail/RenderSystem.hpp>

#include <Siv3D/Shader.hpp>
#include <Siv3D/Graphics3D.hpp>
#include <Siv3D/ScopedRenderTarget3D.hpp>
#include <Siv3D/Palette.hpp>
#include <Siv3D/Math.hpp>

#include <Framework/Renderer2D.hpp>
#include <Framework/Renderer3D.hpp>

using namespace s3d::MathLiterals;

using s3d::Vec3;

namespace eagle::backend
{
	RenderSystem::RenderSystem()
		: m2D{ .list{},.queue{},.camera{MakeShared<Camera2D>(s3d::Scene::CenterF())},.notifyRemove{false},.notifySort{false} }
		, m3D{ .list{},.queue{},.camera{MakeShared<DebugCamera3D>(s3d::Scene::Size(),30_deg,Vec3{0,0,-5},Vec3{0,0,0})},.notifyRemove{false} }
		, mRenderTexture(s3d::Scene::Size(), s3d::HasDepth::Yes)
		, mBackgroundColor(s3d::Palette::DefaultBackground)
	{
	}

	RenderSystem::~RenderSystem()
	{
	}

	void RenderSystem::update()
	{
		// 3D 削除
		if (m3D.notifyRemove)
		{
			auto compare = [](const WeakObject<Renderer3D>& ref)
			{
				auto component = ref.lock();
				return (not component) or component->isPendingKill();
			};
			m3D.list.remove_if(compare);
			m3D.notifyRemove = false;
		}

		// 2D 削除
		if (m2D.notifyRemove)
		{
			auto compare = [](const WeakObject<Renderer2D>& ref)
			{
				auto component = ref.lock();
				return (not component) or component->isPendingKill();
			};
			m2D.list.remove_if(compare);
			m2D.notifyRemove = false;
		}

		// 3D 追加
		if (m3D.queue)
		{
			m3D.list.append(m3D.queue);
			m3D.queue.clear();
		}

		// 2D 追加
		if (m2D.queue)
		{
			m2D.list.append(m2D.queue);
			m2D.queue.clear();
			m2D.notifySort = true;
		}

		// 2D ソート
		if (m2D.notifySort)
		{
			auto compare = [](const WeakObject<Renderer2D>& a, const WeakObject<Renderer2D>& b)
			{
				return a.lock()->getDrawOrder() < b.lock()->getDrawOrder();
			};

			m2D.list.stable_sort_by(compare);
			m2D.notifySort = false;
		}
	}

	void RenderSystem::draw() const
	{
		// 3D 描画
		if (m3D.list)
		{
			draw3D();
		}

		// 2D 描画
		if (m2D.list)
		{
			draw2D();
		}
	}

	void RenderSystem::add(const WeakObject<Renderer2D>& _renderer)
	{
		m2D.queue.push_back(_renderer);
	}

	void RenderSystem::add(const WeakObject<Renderer3D>& _renderer)
	{
		m3D.queue.push_back(_renderer);
	}

	void RenderSystem::notifyChageDrawOrder()
	{
		m2D.notifySort = true;
	}

	void RenderSystem::draw2D() const
	{
		const auto transformer = m2D.camera->createTransformer();

		for (auto& ref : m2D.list)
		{
			auto component = ref.lock();
			if ((not component) or component->isPendingKill())
			{
				m2D.notifyRemove = true;
				continue;
			}
			component->draw();
		}
	}

	void RenderSystem::draw3D() const
	{
		s3d::Graphics3D::SetCameraTransform(*m3D.camera);

		//描画処理
		{
			// renderTexture を背景色で塗りつぶし、
			// renderTexture を 3D 描画のレンダーターゲットに
			const s3d::ScopedRenderTarget3D target{ mRenderTexture.clear(mBackgroundColor.removeSRGBCurve()) };

			for (auto& ref : m3D.list)
			{
				auto component = ref.lock();
				if ((not component) or component->isPendingKill())
				{
					m3D.notifyRemove = true;
					continue;
				}
				component->draw();
			}
		}

		//3D描画を2D描画に転写
		{
			// renderTexture を resolve する前に 3D 描画を実行する
			s3d::Graphics3D::Flush();

			// マルチサンプル・テクスチャのリゾルブ
			mRenderTexture.resolve();

			// リニアレンダリングされた renderTexture をシーンに転送
			s3d::Shader::LinearToScreen(mRenderTexture);
		}
	}
}
