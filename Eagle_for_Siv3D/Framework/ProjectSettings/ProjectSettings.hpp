#pragma once

#include <Core/CoreFwd.hpp>
#include <ProjectSettings/InputAxis.hpp>

namespace eagle
{
	class ProjectSettings
	{
	public:

		static void SetFixedTimestep(double _timestep)noexcept;

		static double GetFixedTimestep()noexcept;

		static void SetExecutionOrder(const TypeID& type, uint64 order);

		static uint64 GetExecutionOrder(const TypeID& type);

		template<class T>
		static void SetExecutionOrder(uint64 order)requires(std::derived_from<T, Component>)
		{
			SetExecutionOrder(typeid(T), order);
		}

		template<class T>
		static uint64 GetExecutionOrder()requires(std::derived_from<T, Component>)
		{
			return GetExecutionOrder(typeid(T));
		}

	private:

	};
}
