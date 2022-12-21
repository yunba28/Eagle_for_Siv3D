#pragma once

#ifdef _DEBUG
#include <Core/RenderSystem/DrawableComponent.hpp>
#else
#include <Core/Component/Component.hpp>
#endif

#include <Components/Collider/Collision.hpp>

namespace eagle
{
	enum class ShapeType
	{
		Circe,
		CircleTrigger,
		Rect,
		Line,
		Polygon
	};

	class Collider2D :
#ifdef _DEBUG
		public DrawableComponent2D
#else
		public Component
#endif
	{
	public:

		Collider2D();

		virtual ~Collider2D();

		void setPos(double _x, double _y, bool _applyTransform = false);

		void setPos(Vec2 _pos, bool _applyTransform = false);

		Vec2 getPos()const;

		void setVelocity(double _x, double _y);

		void setVelocity(Vec2 _velocity);

		Vec2 getVelocity()const;

		void setVelocityRange(RectF _range);

		void setVelocityRange(Vec2 _range);

		RectF getVelocityRange()const;

		void addForce(double _x, double _y);

		void addForce(Vec2 _force);

		void addForceAtPosition(double _x, double _y, Vec2 _worldPosition);

		void addForceAtPosition(Vec2 _force, Vec2 _worldPosition);

		void addImpulse(double _x, double _y);

		void addImpulse(Vec2 _impulse);

		void addImpulseAtPosition(double _x, double _y, Vec2 _worldPosition);

		void addImpulseAtPosition(Vec2 _impulse, Vec2 _worldPosition);

		void setAngle(double _angle, bool _applyTransform = false);

		double getAngle()const;

		void setAngularVelocity(double _velocity);

		double getAngularVelocity()const;

		void setAngularDamping(double _damping);

		double getAngularDamping()const;

		void addAngularImpulse(double _impulse);

		void setBodyType(P2BodyType _bodyType);

		P2BodyType getBodyType()const;

		void setDamping(double _damping);

		double getDamping()const;

		void setFixedRotation(bool _fixed);

		bool isFixedRotation()const;

		void setGravityScale(double _scale);

		double getGravityScale()const;

		void setAwake(bool _awake);

		bool isAwake()const;

		void setSleepEnable(bool _enable);

		void setBulletMode(bool _flag);

		bool isBullet()const;

		double getMass()const;

		void setFilter(P2Filter _filter);

		void setDensity(double _density);

		void setFriction(double _friction);

		void setRestitution(double _restitution);

		void setRestitutionThreshold(double _threshold);

		void translate(double _x,double _y);

		void translate(Vec2 _translate);

		void rotate(double _radianAngle);

		void onCollision(const Collision& _collision);

		P2BodyID id()const noexcept;

	protected:

		// 物理演算が適応できる領域内にいるか
		virtual bool inRegion()const = 0;

		virtual void awake()override;

	private:

		void dispose() final override;

		void fixedUpdate()override;

#ifdef _DEBUG
		void draw()const final override;
#endif

		void collisionUpdate();

		void callCollisionEnter();

		void callCollisionExit();

		void callCollision();

	protected:

		static bool LoadMaterial(const INI& ini, P2Material& material);

		static bool LoadFilter(const INI& ini, P2Filter& filter);

		bool loadProperties(const INI& _ini);

		static bool LoadProperties(const INI& ini, Collider2D& collider);

		static void SaveMaterial(INI& ini, const P2Shape& shape);

		static void SaveFilter(INI& ini, const P2Shape& shape);

		static void SaveProperties(INI& ini, Collider2D& collider);

	private:

		WeakObject<P2World> mP2World;

	protected:

		WeakObject<Camera2D> mCamera2D;

		P2Body mP2Body;

	private:

		RectF mVelocityRange;

		Array<Collision> mPrevCollisions;

		Array<Collision> mCurrentCollisions;
	};
}
