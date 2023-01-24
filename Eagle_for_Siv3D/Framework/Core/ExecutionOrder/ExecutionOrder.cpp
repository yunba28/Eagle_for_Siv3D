#include "ExecutionOrder.hpp"

namespace eagle
{
	HashTable<TypeID, uint64> ExecutionOrder::cOrderList{};

	void ExecutionOrder::Set(const TypeID& type, uint64 order)
	{
		cOrderList[type] = order;
	}

	uint64 ExecutionOrder::Get(const TypeID& type)
	{
		if (cOrderList.contains(type))
		{
			return cOrderList.at(type);
		}

		return 1000ui64;
	}
}
