#pragma once

#include <Framework/FrameworkFwd.hpp>
#include <Common/SmartObject.hpp>
#include <Common/FixedUpdate.hpp>

namespace eagle::backend
{
	class ComponentSystem final
	{
	private:

		using Container_t = Array<SharedObject<Component>>;

	public:

		ComponentSystem();

		~ComponentSystem();

		void update();

		void add(const SharedObject<Component>& _component, const TypeInfo& _type);

	private:

		void lateUpdate();

		void fixedUpdate();

		void addOrders(Array<TypeInfo>& _orders);

		void removeComponents(Array<TypeInfo>& _orders);

		void updateComponents(const Container_t& _container);

	private:

		HashTable<TypeInfo, Container_t> mComponentTable;

		Array<TypeInfo> mExecutionOrder, mPendingOrder, mRemoveOrder;

		FixedUpdate mFixedUpdate;
	};
}
