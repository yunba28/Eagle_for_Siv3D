#include <Framework/SceneObject.hpp>

namespace eagle
{
	SceneObject::SceneObject()
		: mSceneObject(MakeShared<backend::SceneObjectDetail>())
	{

	}

	ObjectHandle<backend::SceneObjectDetail> SceneObject::detail() const noexcept
	{
		return mSceneObject.lock();
	}

	SharedObject<SceneObject> SceneObject::Create()
	{
		auto object = MakeShared<SceneObject>();
		object->mSceneObject->mSelf = object->mSceneObject.weak();
		return object;
	}
}
