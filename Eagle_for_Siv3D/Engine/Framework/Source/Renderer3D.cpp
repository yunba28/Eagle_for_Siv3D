#include <Framework/Renderer3D.hpp>

#include <Framework/Actor.hpp>
#include <Framework/SceneObject.hpp>

namespace eagle
{
	Renderer3D::Renderer3D()
		: mDrawEnable(true)
	{
	}

	Renderer3D::~Renderer3D()
	{
	}

	void Renderer3D::setEnable(Enable _enable, DrawEnable _drawEnable)
	{
		Component::setEnable(_enable);
		mDrawEnable = _drawEnable;
	}

	bool Renderer3D::isDrawEnable() const noexcept
	{
		return mDrawEnable.draw;
	}

	void Renderer3D::_internalAwake()
	{
		getActor()->getSceneObject()->addRenderer(Cast<Renderer3D>(self()));
		Component::_internalAwake();
	}

	void Renderer3D::_internalDraw()
	{
		if (mDrawEnable.draw)
		{
			draw();
		}
	}
}
