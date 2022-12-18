#include "ProjectSettings.hpp"

#include <Core/FixedSystem/FixedSystem.hpp>
#include <Core/ExecutionOrder/ExecutionOrder.hpp>

namespace eagle
{
	void ProjectSettings::SetFixedTimestep(double _timestep) noexcept
	{
		Internal::FixedSystem::SetTimestep(_timestep);
	}

	double ProjectSettings::GetFixedTimestep() noexcept
	{
		return Internal::FixedSystem::GetTimestep();
	}

	void ProjectSettings::SetExecutionOrder(const TypeID& type, uint64 order)
	{
		ExecutionOrder::Set(type, order);
	}

	uint64 ProjectSettings::GetExecutionOrder(const TypeID& type)
	{
		return ExecutionOrder::Get(type);
	}
}
