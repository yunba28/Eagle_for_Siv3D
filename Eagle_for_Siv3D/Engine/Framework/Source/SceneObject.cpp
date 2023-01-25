#include <Framework/SceneObject.hpp>

namespace eagle
{
	SceneObject::SceneObject()
		: mSceneObject(MakeShared<backend::SceneObjectDetail>())
	{
		mSceneObject->mSelf = mSceneObject.weak();
	}

	void SceneObject::setBackgroundColor(const Color& _background)
	{
		mSceneObject->setBackgroundColor(_background);
	}

	WeakObject<Camera2D> SceneObject::getCamera2D() const noexcept
	{
		return mSceneObject->getCamera2D();
	}

	WeakObject<DebugCamera3D> SceneObject::getCamera3D() const noexcept
	{
		return mSceneObject->getCamera3D();
	}

	const Color& SceneObject::getBackgroundColor() const noexcept
	{
		return mSceneObject->getBackgroundColor();
	}

	ObjectHandle<backend::SceneObjectDetail> SceneObject::getDetail() const noexcept
	{
		return mSceneObject.lock();
	}
}
