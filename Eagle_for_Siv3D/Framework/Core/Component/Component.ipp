#pragma once

namespace eagle
{
	template<class T>
	ObjectHandle<T> Component::getActor()const noexcept requires(std::derived_from<T, Actor>)
	{
		return Cast<T>(mActor).lock();
	}

	template<class T>
	SharedObject<T> Component::Create(const Actor_ref& actor)requires(std::derived_from<T, Component>)
	{
		SharedObject<T> ptr{ MakeShared<T>() };
		Create_impl(actor, Cast<Component>(ptr), typeid(T));
		return ptr;
	}
}
