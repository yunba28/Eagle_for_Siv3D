#pragma once

#include <unordered_map>
#include <Framework/Component.hpp>
#include <Classes/FixedUpdate.hpp>

namespace eagle::Internal
{
	class ComponentSystem final
	{
	private:

		using Container_t = std::vector<SharedObject<Component>>;

	public:

		ComponentSystem();

		~ComponentSystem();

		void update();

		void add(const SharedObject<Component>& _component, const TypeInfo& _type);

		size_t count()const noexcept;

		size_t count(const TypeInfo& _type)const noexcept;

	private:

		void lateUpdate();

		void fixedUpdate();

		void addOrders(std::vector<TypeInfo>& _orders);

		void removeComponents(std::vector<TypeInfo>& _orders);

		void updateComponents(const Container_t& _container);

	private:

		std::unordered_map<TypeInfo, Container_t> mComponentTable;

		std::vector<TypeInfo> mExecutionOrder, mPendingOrder, mRemoveOrder;

		FixedUpdate mFixedUpdate;
	};
}