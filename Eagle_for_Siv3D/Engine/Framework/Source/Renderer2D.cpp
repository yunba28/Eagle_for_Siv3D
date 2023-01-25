#include <Framework/Renderer2D.hpp>

#include <Framework/Actor.hpp>
#include <Framework/SceneObject.hpp>

namespace eagle
{
	Renderer2D::Renderer2D()
		: mDrawOrder(1000)
	{
	}

	Renderer2D::~Renderer2D()
	{
	}

	void Renderer2D::setDrawOrder(size_t _order)
	{
		mDrawOrder = _order;
		getActor()->sceneObject()->notifyChangeDrawOrder();
	}

	size_t Renderer2D::getDrawOrder() const noexcept
	{
		return mDrawOrder;
	}

	void Renderer2D::_internalAwake()
	{
		getActor()->sceneObject()->addRenderer(Cast<Renderer2D>(self()));
		Component::_internalAwake();
	}
}
