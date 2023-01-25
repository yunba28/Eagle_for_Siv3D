#include <Framework/Renderer2D.hpp>

#include <Framework/Actor.hpp>
#include <Framework/SceneObject.hpp>

namespace eagle
{
	Renderer2D::Renderer2D()
		: mDrawOrder(1000)
		, mDrawEnable(true, false)
	{
	}

	Renderer2D::~Renderer2D()
	{
	}

	void Renderer2D::setDrawOrder(size_t _order)
	{
		mDrawOrder = _order;
		getActor()->getSceneObject()->notifyChangeDrawOrder();
	}

	void Renderer2D::setEnable(Enable _enable, DrawEnable _drawEnable)
	{
		Component::setEnable(_enable);
		mDrawEnable = _drawEnable;
	}

	size_t Renderer2D::getDrawOrder() const noexcept
	{
		return mDrawOrder;
	}

	bool Renderer2D::isDrawEnable() const noexcept
	{
		return mDrawEnable.draw;
	}

	bool Renderer2D::isDrawScreenEnable() const noexcept
	{
		return mDrawEnable.drawScreen;
	}

	void Renderer2D::_internalAwake()
	{
		getActor()->getSceneObject()->addRenderer(Cast<Renderer2D>(self()));
		Component::_internalAwake();
	}

	void Renderer2D::_internalDraw() const
	{
		if (mDrawEnable.draw)
		{
			draw();
		}
	}

	void Renderer2D::_internalDrawScreen() const
	{
		if (mDrawEnable.drawScreen)
		{
			drawScreen();
		}
	}
}
