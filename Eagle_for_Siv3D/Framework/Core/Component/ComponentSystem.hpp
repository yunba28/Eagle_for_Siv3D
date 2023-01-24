#pragma once

#include <Core/CoreFwd.hpp>
#include <Utility/SmartObject.hpp>

namespace eagle::Internal
{
	class ComponentArray final
	{
	public:

		ComponentArray();

		void update();

		void lateUpdate();

		void add(const SharedObject<Component>& _component);

		WeakObject<Component> getComponent(const WeakObject<Actor>& _actor)const;

		WeakObject<Component> getComponentByTag(const String& _tag)const;

		Array<WeakObject<Component>> getComponents()const;

		Array<WeakObject<Component>> getComponentsByTag(const String& _tag)const;

		size_t count()const noexcept;

	private:

		Array<SharedObject<Component>> mComponents;

		Array<SharedObject<Component>> mPendingComponents;

		bool mNeedRemove;
	};

	class ComponentSystem final
	{
	public:

		ComponentSystem();

		void update();

		void lateUpdate();

		void add(const SharedObject<Component>& _component, const TypeID& _type);

		WeakObject<Component> getComponent(const WeakObject<Actor>& _actor, const TypeID& _type)const;

		WeakObject<Component> getComponentByTag(const String& _tag, const TypeID& _type)const;

		Array<WeakObject<Component>> getComponents(const TypeID& _type)const;

		Array<WeakObject<Component>> getComponentsByTag(const String& _tag, const TypeID& _type)const;

		size_t count(const TypeID& _type)const;

		size_t count()const;

		bool contains(const TypeID& _type)const;

	private:

		/// @brief すべてのComponentを管理
		HashTable<TypeID, ComponentArray> mComponents;

		/// @brief Componentの実行順
		Array<TypeID> mExecutionOrder;

		Array<TypeID> mPendingOrders;

	};
}
