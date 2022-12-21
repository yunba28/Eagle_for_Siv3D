﻿#pragma once

#include <Components/Collider/Collider2D.hpp>

namespace eagle
{
	class RectCollider2D : public Collider2D
	{
	public:

		void setupRect(const RectF& _rect, const P2Material& _material = {}, const P2Filter& _filter = {});

		RectF getRect()const;

	private:

		bool inRegion()const final override;

	private:

		RectF mRect{};

		template<class Type>
		friend bool Load(const String& path, Type& collider);

		template<class Type>
		friend bool Save(const String& path, Type& collider);
	};
}
