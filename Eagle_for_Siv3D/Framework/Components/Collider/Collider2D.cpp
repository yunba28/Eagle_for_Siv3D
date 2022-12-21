#include "Collider2D.hpp"

#include <Core/Scene/ScenePimpl.hpp>
#include <Core/RenderSystem/RenderSystem.hpp>
#include <Core/FixedSystem/FixedSystem.hpp>
#include <Components/Transform.hpp>

namespace eagle
{
	Collider2D::Collider2D()
		: mP2World()
		, mP2Body()
		, mVelocityRange(-1000, -1000, 2000, 2000)
		, mPrevCollisions()
		, mCurrentCollisions()
	{

	}

	Collider2D::~Collider2D()
	{
	}

	void Collider2D::setPos(double _x, double _y, bool _applyTransform)
	{
		mP2Body.setPos(_x, _y);

		// 即座にTrasformへ適応
		if (_applyTransform)
		{
			getTransform()->setPos(_x, _y, Space::Absolute);
		}
	}

	void Collider2D::setPos(Vec2 _pos, bool _applyTransform)
	{
		mP2Body.setPos(_pos);

		// 即座にTrasformへ適応
		if (_applyTransform)
		{
			getTransform()->setPos(_pos, Space::Absolute);
		}
	}

	Vec2 Collider2D::getPos() const
	{
		return mP2Body.getPos();
	}

	void Collider2D::setVelocity(double _x, double _y)
	{
		mP2Body.setVelocity(Vec2{ _x, _y });
	}

	void Collider2D::setVelocity(Vec2 _velocity)
	{
		mP2Body.setVelocity(_velocity);
	}

	Vec2 Collider2D::getVelocity() const
	{
		return mP2Body.getVelocity();
	}

	void Collider2D::setVelocityRange(RectF _range)
	{
		mVelocityRange = _range;
	}

	void Collider2D::setVelocityRange(Vec2 _range)
	{
		mVelocityRange.set(Arg::center = Vec2{ 0,0 }, _range);
	}

	RectF Collider2D::getVelocityRange() const
	{
		return mVelocityRange;
	}

	void Collider2D::addForce(double _x, double _y)
	{
		mP2Body.applyForce(Vec2{ _x,_y });
	}

	void Collider2D::addForce(Vec2 _force)
	{
		mP2Body.applyForce(_force);
	}

	void Collider2D::addForceAtPosition(double _x, double _y, Vec2 _worldPosition)
	{
		mP2Body.applyForceAt(Vec2{ _x,_y }, _worldPosition);
	}

	void Collider2D::addForceAtPosition(Vec2 _force, Vec2 _worldPosition)
	{
		mP2Body.applyForceAt(_force, _worldPosition);
	}

	void Collider2D::addImpulse(double _x, double _y)
	{
		mP2Body.applyLinearImpulse(Vec2{ _x,_y });
	}

	void Collider2D::addImpulse(Vec2 _impulse)
	{
		mP2Body.applyLinearImpulse(_impulse);
	}

	void Collider2D::addImpulseAtPosition(double _x, double _y, Vec2 _worldPosition)
	{
		mP2Body.applyLinearImpulseAt(Vec2{ _x,_y }, _worldPosition);
	}

	void Collider2D::addImpulseAtPosition(Vec2 _impulse, Vec2 _worldPosition)
	{
		mP2Body.applyLinearImpulseAt(_impulse, _worldPosition);
	}

	void Collider2D::setAngle(double _angle, bool _applyTransform)
	{
		mP2Body.setAngle(_angle);

		// 即座にTrasformへ適応
		if (_applyTransform)
		{
			getTransform()->setRot(Mat4x4::RotateZ(_angle));
		}
	}

	double Collider2D::getAngle() const
	{
		return mP2Body.getAngle();
	}

	void Collider2D::setAngularVelocity(double _velocity)
	{
		mP2Body.setAngularVelocity(_velocity);
	}

	double Collider2D::getAngularVelocity() const
	{
		return mP2Body.getAngularVelocity();
	}

	void Collider2D::setAngularDamping(double _damping)
	{
		mP2Body.setAngularDamping(_damping);
	}

	double Collider2D::getAngularDamping() const
	{
		return mP2Body.getAngularDamping();
	}

	void Collider2D::addAngularImpulse(double _impulse)
	{
		mP2Body.applyAngularImpulse(_impulse);
	}

	void Collider2D::setBodyType(P2BodyType _bodyType)
	{
		mP2Body.setBodyType(_bodyType);
	}

	P2BodyType Collider2D::getBodyType() const
	{
		return mP2Body.getBodyType();
	}

	void Collider2D::setDamping(double _damping)
	{
		mP2Body.setDamping(_damping);
	}

	double Collider2D::getDamping() const
	{
		return mP2Body.getDamping();
	}

	void Collider2D::setFixedRotation(bool _fixed)
	{
		mP2Body.setFixedRotation(_fixed);
	}

	bool Collider2D::isFixedRotation() const
	{
		return mP2Body.isFixedRotation();
	}

	void Collider2D::setGravityScale(double _scale)
	{
		mP2Body.setGravityScale(_scale);
	}

	double Collider2D::getGravityScale() const
	{
		return mP2Body.getGravityScale();
	}

	void Collider2D::setAwake(bool _awake)
	{
		mP2Body.setAwake(_awake);
	}

	bool Collider2D::isAwake() const
	{
		return mP2Body.isAwake();
	}

	void Collider2D::setSleepEnable(bool _enable)
	{
		mP2Body.setSleepEnabled(_enable);
	}

	void Collider2D::setBulletMode(bool _flag)
	{
		mP2Body.setBullet(_flag);
	}

	bool Collider2D::isBullet() const
	{
		return mP2Body.isBullet();
	}

	double Collider2D::getMass() const
	{
		return mP2Body.getMass();
	}

	void Collider2D::setFilter(P2Filter _filter)
	{
		mP2Body.shape(0).setFilter(_filter);
	}

	void Collider2D::setDensity(double _density)
	{
		mP2Body.shape(0).setDensity(_density);
	}

	void Collider2D::setFriction(double _friction)
	{
		mP2Body.shape(0).setFriction(_friction);
	}

	void Collider2D::setRestitution(double _restitution)
	{
		mP2Body.shape(0).setRestitution(_restitution);
	}

	void Collider2D::setRestitutionThreshold(double _threshold)
	{
		mP2Body.shape(0).setRestitutionThreshold(_threshold);
	}

	void Collider2D::translate(double _x, double _y)
	{
		setPos(mP2Body.getPos().movedBy(_x, _y), true);
	}

	void Collider2D::translate(Vec2 _translate)
	{
		setPos(mP2Body.getPos().movedBy(_translate), true);
	}

	void Collider2D::rotate(double _radianAngle)
	{
		setAngle(mP2Body.getAngle() + _radianAngle, true);
	}

	void Collider2D::onCollision(const Collision& _collision)
	{
		mCurrentCollisions << _collision;
	}

	P2BodyID Collider2D::id() const noexcept
	{
		return mP2Body.id();
	}

	void Collider2D::awake()
	{
		mP2World = Internal::FixedSystem::GetP2World();
		mP2Body = mP2World.lock()->createPlaceholder(P2BodyType::Dynamic, Vec2{ 0,0 });
		mCamera2D = getActor()->getScene()->_getRenderSystem()->getCamera2D();
		joinFixedSystem(Cast<Collider2D>(mThis));
#ifdef _DEBUG
		setFunctionEnable(false, false, true, true);
#else
		setFunctionEnable(false, false, true);
#endif
	}

	void Collider2D::dispose()
	{
		auto system = getActor()->getScene()->_getFixedSystem();
		system->removeCollider(mP2Body.id());
	}

	void Collider2D::fixedUpdate()
	{
		auto tf = getTransform();
		tf->setPos(mP2Body.getPos(), Space::Absolute);
		tf->setRot(Mat4x4::RotateZ(mP2Body.getAngle()), Space::Absolute);

		mP2Body.setVelocity(mP2Body.getVelocity().clamped(mVelocityRange));

		if (mCurrentCollisions.isEmpty())
		{
			callCollisionExit();
		}
		else
		{
			collisionUpdate();
		}

		// 物体が領域内にあるか
		if (inRegion())
		{
			mP2Body.setAwake(true);
		}
		else
		{
			mP2Body.setAwake(false);
			callCollisionExit();
			mPrevCollisions.clear();
		}
	}

#ifdef _DEBUG
	void Collider2D::draw()const
	{
		mP2Body.drawWireframe(2.0, ColorF{ 0.0,1.0,0.7 });
	}
#endif

	void Collider2D::collisionUpdate()
	{
		if (mPrevCollisions.isEmpty())
		{
			callCollisionEnter();
		}
		else
		{
			callCollision();
		}

		mPrevCollisions.swap(mCurrentCollisions);
		mCurrentCollisions.clear();
	}

	void Collider2D::callCollisionEnter()
	{
		for (const auto& ref : getActor()->getAllComponents())
		{
			for (const auto& col : mCurrentCollisions)
			{
				ref.lock()->onCollisionEnter(col);
			}
		}
	}

	void Collider2D::callCollisionExit()
	{
		for (const auto& col : mPrevCollisions)
		{
			for (const auto& ref : getActor()->getAllComponents())
			{
				ref.lock()->onCollisionExit(col);
			}
		}
	}

	void Collider2D::callCollision()
	{
		const auto& refs = getActor()->getAllComponents();

		Array<Collision> CurrentCollisions = mCurrentCollisions;

		// 以前のコリジョンを比較
		for (const auto& pcol : mPrevCollisions)
		{
			bool isStay = false;
			auto it = CurrentCollisions.begin();
			auto itEnd = CurrentCollisions.end();
			for (it;it!=itEnd;++it)
			{
				// 以前も当たっていて今回も当たっている
				if (pcol.p2.pair == (*it).p2.pair)
				{
					isStay = true;
					for (const auto& ref : refs)
					{
						ref.lock()->onCollisionStay(*it);
					}
					CurrentCollisions.erase(it);
					break;
				}
			}
			// 以前は当たっていたが今回は当たっていない
			if (not isStay)
			{
				for (const auto& ref : refs)
				{
					ref.lock()->onCollisionExit(pcol);
				}
			}
		}
		// 初めて当たる
		for (const auto& ccol : CurrentCollisions)
		{
			for (const auto& ref : refs)
			{
				ref.lock()->onCollisionEnter(ccol);
			}
		}
	}

	bool Collider2D::LoadMaterial(const INI& _ini, P2Material& _material)
	{
		if (_ini.hasValue(U"Material", U"Density"))
		{
			_material.density = ParseFloat<double>(_ini[U"Material.Density"]);
		}

		if (_ini.hasValue(U"Material", U"Restitution"))
		{
			_material.restitution = ParseFloat<double>(_ini[U"Material.Restitution"]);
		}

		if (_ini.hasValue(U"Material", U"Friction"))
		{
			_material.friction = ParseFloat<double>(_ini[U"Material.Friction"]);
		}

		if (_ini.hasValue(U"Material", U"RestitutionThreshold"))
		{
			_material.restitutionThreshold = ParseFloat<double>(_ini[U"Material.RestitutionThreshold"]);
		}

		return true;
	}

	bool Collider2D::LoadFilter(const INI& _ini, P2Filter& _filter)
	{
		if (_ini.hasValue(U"Filter", U"Category"))
		{
			_filter.categoryBits = ParseInt<uint16>(_ini[U"Filter.Category"]);
		}

		if (_ini.hasValue(U"Filter", U"Mask"))
		{
			_filter.categoryBits = ParseInt<uint16>(_ini[U"Filter.Mask"]);
		}

		return true;
	}

	bool Collider2D::loadProperties(const INI& _ini)
	{
		const String section = U"Properties";

		if (not _ini.hasSection(section))
			return false;

		try
		{
			if (_ini.hasValue(section,U"Position"))
			{
				setPos(Parse<Vec2>(_ini[section + U".Position"]), true);
			}

			if (_ini.hasValue(section,U"VelocityRange"))
			{
				setVelocityRange(Parse<RectF>(_ini[section + U".VelocityRange"]));
			}

			if (_ini.hasValue(section, U"Angle"))
			{
				setAngle(Parse<double>(_ini[section + U".Angle"]), true);
			}

			if (_ini.hasValue(section,U"BodyType"))
			{
				setBodyType(static_cast<P2BodyType>(Parse<uint8>(_ini[section + U".BodyType"])));
			}

			if (_ini.hasValue(section, U"Damping"))
			{
				setDamping(Parse<double>(_ini[section + U".Damping"]));
			}

			if (_ini.hasValue(section, U"FixedRotation"))
			{
				setFixedRotation(Parse<bool>(_ini[section + U".FixedRotation"]));
			}

			if (_ini.hasValue(section, U"GravityScale"))
			{
				setGravityScale(Parse<double>(_ini[section + U".GravityScale"]));
			}

			if (_ini.hasValue(section, U"Awake"))
			{
				setAwake(Parse<bool>(_ini[section + U".Awake"]));
			}

			if (_ini.hasValue(section, U"Bullte"))
			{
				setBulletMode(Parse<bool>(_ini[section + U".Bullte"]));
			}
		}
		catch (ParseError&)
		{
			return false;
		}

		return true;
	}
}
