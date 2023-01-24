#pragma once

#include <Eagle.hpp>

#include "../Fwd.hpp"

namespace eagle
{
	template<class State, class Data>
	class BasicScene : s3d::SceneManager<State,Data>::Scene
	{
	public:

		explicit BasicScene(const InitData& _init)
			: mSceneObject(MakeShared<SceneObject>())
		{

		}

		virtual ~BasicScene() = default;

		virtual void update()
		{
			mSceneObject->update();
		}

	protected:

		ObjectHandle<SceneObject> sceneObject()const noexcept
		{
			return mSceneObject.lock();
		}

	private:

		SharedObject<SceneObject> mSceneObject;

	};
}
