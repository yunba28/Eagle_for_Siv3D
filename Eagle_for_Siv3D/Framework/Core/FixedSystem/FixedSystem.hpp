#pragma once

#include <Core/CoreFwd.hpp>
#include <Utility/SmartObject.hpp>

namespace eagle::Internal
{
	class FixedSystem final
	{
	public:

		FixedSystem();

		void update();

		void add(const WeakObject<Component>& _component);

		static void SetTimestep(double _timestep)noexcept;

		static double GetTimestep()noexcept;

	private:

		Array<WeakObject<Component>> mComponents;

		Array<WeakObject<Component>> mPendingComponents;

		bool mNeedRemove;

		double mAccumulation;

		static double sTimestep;
	};
}
