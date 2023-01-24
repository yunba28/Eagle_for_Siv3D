#include "ProjectSettings.hpp"

#include <Core/FixedSystem/FixedSystem.hpp>
#include <Core/ExecutionOrder/ExecutionOrder.hpp>

#include <Utility/Input.hpp>

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

	void ProjectSettings::AddInputAxis(const String& tag, const Input& input, double scale)
	{
		InputAxis.add(tag, input, scale);
	}

	void ProjectSettings::AddInputAxis(const String& tag, const Array<std::pair<Input, double>>& axisList)
	{
		for (const auto& [input,scale] : axisList)
		{
			InputAxis.add(tag, input, scale);
		}
	}

	void ProjectSettings::RemoveInputAxis(const String& tag)
	{
		InputAxis.remove(tag);
	}

	void ProjectSettings::AddInputAction(const String& tag, const InputGroup& action)
	{
		InputAction.add(tag, action);
	}

	void ProjectSettings::AddInputAction(const String& tag, const Array<InputGroup>& actionList)
	{
		for (const auto& action : actionList)
		{
			InputAction.add(tag, action);
		}
	}

	void ProjectSettings::RemoveInputAction(const String& tag)
	{
		InputAction.remove(tag);
	}
}
