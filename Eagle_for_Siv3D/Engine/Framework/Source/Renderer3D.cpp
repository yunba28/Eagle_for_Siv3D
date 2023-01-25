#include <Framework/Renderer3D.hpp>

#include <Framework/Actor.hpp>
#include <Framework/SceneObject.hpp>

namespace eagle
{
	Renderer3D::Renderer3D()
	{
	}

	Renderer3D::~Renderer3D()
	{
	}

	void Renderer3D::_internalAwake()
	{
		getActor()->sceneObject()->addRenderer(Cast<Renderer3D>(self()));
		Component::_internalAwake();
	}
}
