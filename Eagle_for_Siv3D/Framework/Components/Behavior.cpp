#include "Behavior.hpp"

#include <Framework/Actor.hpp>

namespace eagle
{
	Behavior::Behavior()
		: mTransform()
	{
	}

	ObjectHandle<Transform> Behavior::getTransform() const noexcept
	{
		return mTransform.lock();
	}

	void Behavior::_internalAwake()
	{
		mTransform = getActor()->getComponent<Transform>();
	}
}
