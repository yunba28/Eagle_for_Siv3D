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
	private:

		using List_t = ComponentArray;

	public:

		ComponentSystem();

		void update();

		void add(const SharedObject<Component>& _component, const std::type_index& _type);

		WeakObject<Component> getComponent(const WeakObject<Actor>& _actor, const std::type_index& _type)const;

		WeakObject<Component> getComponentByTag(const String& _tag, const std::type_index& _type)const;

		Array<WeakObject<Component>> getComponents(const std::type_index& _type)const;

		Array<WeakObject<Component>> getComponentsByTag(const String& _tag, const std::type_index& _type)const;

		size_t count(const std::type_index& _type)const;

		size_t count()const;

		bool contains(const std::type_index& _type)const;

	private:

		/// @brief すべてのComponentを管理
		HashTable<std::type_index, List_t> mComponents;

		/// @brief Componentの実行順
		Array<std::type_index> mExecutionOrder;

		Array<std::type_index> mPendingOrders;

	};
}
