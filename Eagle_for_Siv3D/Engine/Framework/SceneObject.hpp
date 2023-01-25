#pragma once

#include <Framework/Detail/SceneObjectDetail.hpp>

namespace eagle
{
	class SceneObject final : Uncopyable
	{
	public:

		SceneObject();

		template<class T = Actor>
		WeakObject<T> createActor(const String& _name = U"MyActor")
		{
			return mSceneObject->createActor<T>(_name);
		}

		ObjectHandle<backend::SceneObjectDetail> detail()const noexcept;

	public:

		static SharedObject<SceneObject> Create();

	private:

		SharedObject<backend::SceneObjectDetail> mSceneObject;

	};
}
