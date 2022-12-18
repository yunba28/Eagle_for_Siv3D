#include "ExecutionOrder.hpp"

namespace eagle
{
	HashTable<std::type_index, uint64> ExecutionOrder::cOrderList{};

	void ExecutionOrder::Set(const std::type_index& type, uint64 order)
	{
		cOrderList[type] = order;
	}

	uint64 ExecutionOrder::Get(const std::type_index& type)
	{
		if (cOrderList.contains(type))
		{
			return cOrderList.at(type);
		}

		return 1000ui64;
	}
}
