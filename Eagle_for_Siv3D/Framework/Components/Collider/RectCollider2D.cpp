#include "RectCollider2D.hpp"
#include <Components/Transform.hpp>
#include <Utility/Load.hpp>

namespace eagle
{
	void RectCollider2D::setupRect(const RectF& _rect, const P2Material& _material, const P2Filter& _filter)
	{
		if (mP2Body.num_shapes() < 1)
		{
			mP2Body.addRect(_rect, _material, _filter);
		}
	}

	RectF RectCollider2D::getRect() const
	{
		return mRect.movedBy(getTransform()->getWorldPos2D());
	}

	bool RectCollider2D::inRegion() const
	{
		auto camera = mCamera2D.lock();

		auto area = camera->getRegion().scaled(1.2);

		return area.intersects(getRect());
	}

	template<>
	bool Load<RectCollider2D>(const String& path, RectCollider2D& collider)
	{
		if (FileSystem::Extension(path) != U"col")
			return false;

		const INI ini{ path };

		if (ini.isEmpty())
			return false;

		if (not ini.hasSection(U"Rect"))
			return false;

		RectF rect{};
		P2Material material{};
		P2Filter filter{};

		try
		{
			if (ini.hasValue(U"Rect", U"LocalPos"))
			{
				rect.pos = Parse<Vec2>(ini[U"Rect.LocalPos"]);
			}

			if (ini.hasValue(U"Rect", U"Size"))
			{
				rect.size = Parse<Vec2>(ini[U"Rect.Size"]);
			}

			Collider2D::LoadMaterial(ini, material);
			Collider2D::LoadFilter(ini, filter);
		}
		catch (ParseError&)
		{
			return false;
		}

		collider.setupRect(rect, material, filter);

		return Collider2D::LoadProperties(ini, collider);
	}

	template<>
	bool Save<RectCollider2D>(const String& path, RectCollider2D& collider)
	{
		String out{ path };

		if (FileSystem::Extension(out) != U"col")
		{
			auto it = std::find(out.begin(), out.end(), U'.');
			out.erase(it + 1, out.end());
			out += U"col";
		}

		INI ini{};

		ini.addSection(U"Rect");
		ini.write(U"Rect", U"LocalPos", collider.mRect.pos);
		ini.write(U"Rect", U"Size", collider.mRect.size);

		Collider2D::SaveMaterial(ini, collider.mP2Body.shape(0));
		Collider2D::SaveFilter(ini, collider.mP2Body.shape(0));
		Collider2D::SaveProperties(ini, collider);

		return ini.save(out);
	}
}
