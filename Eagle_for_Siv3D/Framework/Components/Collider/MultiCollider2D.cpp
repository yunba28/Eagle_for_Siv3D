#include "MultiCollider2D.hpp"

namespace eagle
{
	void MultiCollider2D::addCircle(const Circle& _circle, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addCircle(_circle, _material, _filter);
		mShapeTypes << ShapeType::Circe;
	}

	void MultiCollider2D::addCircleTrigger(const Circle& _circle, const P2Filter& _filter)
	{
		mP2Body.addCircleSensor(_circle, _filter);
		mShapeTypes << ShapeType::CircleTrigger;
	}

	void MultiCollider2D::addRect(const RectF& _rect, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addRect(_rect, _material, _filter);
		mShapeTypes << ShapeType::Rect;
	}

	void MultiCollider2D::addLine(const Line& _line, OneSided _onesided, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addLine(_line, _onesided, _material, _filter);
		mShapeTypes << ShapeType::Line;
	}

	void MultiCollider2D::addPolygon(const Polygon& _polygon, const P2Material& _material, const P2Filter& _filter)
	{
		mP2Body.addPolygon(_polygon, _material, _filter);
		mShapeTypes << ShapeType::Polygon;
	}

	void MultiCollider2D::setFilter(P2Filter _filter, size_t _index)
	{
		mP2Body.shape(_index).setFilter(_filter);
	}

	void MultiCollider2D::setDensity(double _density, size_t _index)
	{
		mP2Body.shape(_index).setDensity(_density);
	}

	void MultiCollider2D::setFriction(double _friction, size_t _index)
	{
		mP2Body.shape(_index).setFriction(_friction);
	}

	void MultiCollider2D::setRestitution(double _restitution, size_t _index)
	{
		mP2Body.shape(_index).setRestitution(_restitution);
	}

	void MultiCollider2D::setRestitutionThreshold(double _threshold, size_t _index)
	{
		mP2Body.shape(_index).setRestitutionThreshold(_threshold);
	}

	size_t MultiCollider2D::getShapeCount() const
	{
		return mP2Body.num_shapes();
	}

	bool MultiCollider2D::inRegion() const
	{
		return true;
	}
}
