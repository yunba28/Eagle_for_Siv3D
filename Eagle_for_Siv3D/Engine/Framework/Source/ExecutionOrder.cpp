#include <Framework/ExecutionOrder.hpp>

namespace eagle
{
	HashTable<TypeInfo, size_t> ExecutionOrder::sOrderList{};

	void ExecutionOrder::Set(const TypeInfo& info, size_t order)
	{
		sOrderList[info] = order;
	}

	size_t ExecutionOrder::Get(const TypeInfo& info)
	{
		if (sOrderList.contains(info))
		{
			return sOrderList[info];
		}
		return 1000ui64;
	}
}
