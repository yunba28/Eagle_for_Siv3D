#include "DrawableComponent.hpp"

#include <Core/RenderSystem/RenderSystem.hpp>
#include <Core/Scene/ScenePimpl.hpp>

namespace eagle
{
	DrawableComponent2D::DrawableComponent2D()
		: mDrawOrder(1000)
		, mEnabledDraw(true)
	{
	}

	void DrawableComponent2D::setFunctionEnable(bool _update, bool _lateUpdate, bool _fixedUpdate, bool _draw) noexcept
	{
		Component::setFunctionEnable(_update, _lateUpdate, _fixedUpdate);
		mEnabledDraw = _draw;
	}

	void DrawableComponent2D::setDrawOrder(uint64 _order) noexcept
	{
		mDrawOrder = Min(_order, 9999ui64);

		auto system = getActor()->getScene()->_getRenderSystem();
		system->notifyChange2DPriority();
	}

	uint64 DrawableComponent2D::getDrawOrder() const noexcept
	{
		return mDrawOrder;
	}

	void DrawableComponent2D::_internalAwake()
	{
		// 描画システムに追加する
		{
			auto system = getActor()->getScene()->_getRenderSystem();
			system->add(Cast<DrawableComponent2D>(mThis));
		}

		Component::_internalAwake();
	}

	void DrawableComponent2D::_internalDraw() const
	{
		// draw関数が無効
		if (not mEnabledDraw)
			return;

		auto actor = getActor();

		// Actorが有効であれば更新する
		if (actor and actor->isActive())
		{
			draw();
		}
	}

	bool DrawableComponent2D::isEnableDraw() const noexcept
	{
		return mEnabledDraw;
	}

	DrawableComponent3D::DrawableComponent3D()
		: mEnabledDraw(true)
	{
	}

	void DrawableComponent3D::setFunctionEnable(bool _update, bool _lateUpdate, bool _fixedUpdate, bool _draw) noexcept
	{
		Component::setFunctionEnable(_update, _lateUpdate, _fixedUpdate);
		mEnabledDraw = _draw;
	}

	void DrawableComponent3D::_internalAwake()
	{
		// 描画システムに追加する
		{
			auto system = getActor()->getScene()->_getRenderSystem();
			system->add(Cast<DrawableComponent3D>(mThis));
		}

		Component::_internalAwake();
	}

	void DrawableComponent3D::_internalDraw() const
	{
		// draw関数が無効
		if (not mEnabledDraw)
			return;

		auto actor = getActor();

		// Actorが有効であれば更新する
		if (actor and actor->isActive())
		{
			draw();
		}
	}
	bool DrawableComponent3D::isEnableDraw() const noexcept
	{
		return mEnabledDraw;
	}
}
