#include <Framework/Detail/ComponentSystem.hpp>

#include <Framework/Component.hpp>
#include <Framework/ExecutionOrder.hpp>

namespace eagle::backend
{
	ComponentSystem::ComponentSystem()
		: mComponentTable()
		, mExecutionOrder()
		, mPendingOrder()
		, mRemoveOrder()
		, mFixedUpdate([&]() {this->fixedUpdate(); })
	{
	}

	ComponentSystem::~ComponentSystem()
	{
	}

	void ComponentSystem::update()
	{
		// 追加する型リストがあれば追加する
		addOrders(mPendingOrder);

		// 削除するコンポーネントがあれば削除する
		removeComponents(mRemoveOrder);

		// 通常の更新処理
		for (const auto& type : mExecutionOrder)
		{
			updateComponents(mComponentTable[type]);
		}

		// 一定間隔で呼ばれる更新処理
		mFixedUpdate.update();

		// 通常の更新処理の後に行われる更新処理
		lateUpdate();

	}

	void ComponentSystem::add(const SharedObject<Component>& _component, const TypeInfo& _type)
	{
		if (not mComponentTable.contains(_type))
		{
			mComponentTable.emplace(_type, Container_t{});
			mPendingOrder.push_back(_type);
		}
		mComponentTable[_type].push_back(_component);
	}

	void ComponentSystem::lateUpdate()
	{
		for (const auto& type : mExecutionOrder)
		{
			for (const auto& component : mComponentTable[type])
			{
				if (component->isEnable())
				{
					component->_internalLateUpdate();
				}
			}
		}
	}

	void ComponentSystem::fixedUpdate()
	{
		for (const auto& type : mExecutionOrder)
		{
			for (const auto& component : mComponentTable[type])
			{
				if (component->isEnable())
				{
					component->_internalFixedUpdate();
				}
			}
		}
	}

	void ComponentSystem::addOrders(Array<TypeInfo>& _orders)
	{
		// 空の場合は処理を中断する
		if (_orders.empty())
			return;

		mExecutionOrder.insert(
			mExecutionOrder.end(),
			_orders.begin(),
			_orders.end());

		_orders.clear();

		auto compare = [](const TypeInfo& a, const TypeInfo& b)->bool
		{
			return ExecutionOrder::Get(a) < ExecutionOrder::Get(b);
		};

		mExecutionOrder.stable_sort_by(compare);
	}

	void ComponentSystem::removeComponents(Array<TypeInfo>& _orders)
	{
		// 空の場合は処理を中断する
		if (_orders.empty())
			return;

		auto compare = [](const SharedObject<Component>& component)
		{
			return component->isPendingKill();
		};

		for (const auto& order : _orders)
		{
			auto& container = mComponentTable[order];
			container.remove_if(compare);
		}

		_orders.clear();
	}

	void ComponentSystem::updateComponents(const Container_t& _container)
	{
		// 空の場合は更新しない
		if (_container.empty())
			return;

		bool removed{ false };

		for (const auto& component : _container)
		{
			// 破棄命令が出ている
			if (component->isPendingKill())
			{
				removed = true;
				continue;
			}

			if (component->isEnable())
			{
				component->_internalUpdate();
			}
		}

		// 破棄命令が出ていることを告げる
		if (removed)
		{
			mRemoveOrder.push_back(_container[0]->getTypeInfo());
		}
	}
}
