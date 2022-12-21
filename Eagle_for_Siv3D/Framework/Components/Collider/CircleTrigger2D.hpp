#pragma once

#include <Components/Collider/CircleCollider2D.hpp>

namespace eagle
{
	class CircleTrigger2D : public Collider2D
	{
	public:

		void setupCircle(const Circle& _circle, const P2Filter& _filter = {});

		Circle getCircle()const;

	private:

		bool inRegion()const final override;

	private:

		Circle mCircle{};

		template<class Type>
		friend bool Load(const String& path, Type& collider);

		template<class Type>
		friend bool Save(const String& path, Type& collider);

	};
}
