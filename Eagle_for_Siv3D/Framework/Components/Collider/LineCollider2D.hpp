#pragma once

#include <Components/Collider/Collider2D.hpp>

namespace eagle
{
	class LineCollider2D final : public Collider2D
	{
	public:

		void setupLine(const Line& _line, OneSided _oneSided = OneSided::No, const P2Material& _material = {}, const P2Filter& _filter = {});

		Line getLine()const;

	private:

		bool inRegion()const final override;

	private:

		Line mLine{};

	};
}
