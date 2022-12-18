#include "Transform_detail.hpp"

#include <Core/ExecutionOrder/ExecutionOrder.hpp>

namespace eagle
{
	float MGet(const Mat4x4& m, int32 y, int32 x)
	{
		if (InRange(y, 0, 3) and InRange(x, 0, 3))
		{
			return m.value.r[y].m128_f32[x];
		}

		throw std::out_of_range("out of range ");
	}

	void MSet(Mat4x4& m, int32 y, int32 x, float val)
	{
		if (InRange(y, 0, 3) and InRange(x, 0, 3))
		{
			m.value.r[y].m128_f32[x] = val;
		}

		throw std::out_of_range("out of range ");
	}

	Quaternion QLookAt(const Mat4x4& m)
	{
		float elem[4]{};
		elem[0] = MGet(m, 0, 0) - MGet(m, 1, 1) - MGet(m, 2, 2) + 1.0f;
		elem[1] = -MGet(m, 0, 0) + MGet(m, 1, 1) - MGet(m, 2, 2) + 1.0f;
		elem[2] = -MGet(m, 0, 0) - MGet(m, 1, 1) + MGet(m, 2, 2) + 1.0f;
		elem[3] = MGet(m, 0, 0) + MGet(m, 1, 1) + MGet(m, 2, 2) + 1.0f;

		int32 biggestIdx = 0;
		for (int32 i = 0; i < std::size(elem); i++)
		{
			if (elem[i] > elem[biggestIdx])
			{
				biggestIdx = i;
			}
		}

		if (elem[biggestIdx] < 0)
		{
			return Quaternion{};
		}

		float q[4]{};
		float v = Sqrt(elem[biggestIdx]) * 0.5f;
		q[biggestIdx] = v;
		float mult = 0.25f / v;

		switch (biggestIdx)
		{
		case 0:
			q[1] = (MGet(m, 1, 0) + MGet(m, 0, 1)) * mult;
			q[2] = (MGet(m, 0, 2) + MGet(m, 2, 0)) * mult;
			q[3] = (MGet(m, 2, 1) - MGet(m, 1, 2)) * mult;
			break;
		case 1:
			q[0] = (MGet(m, 1, 0) + MGet(m, 0, 1)) * mult;
			q[2] = (MGet(m, 2, 1) + MGet(m, 1, 2)) * mult;
			q[3] = (MGet(m, 0, 2) - MGet(m, 2, 0)) * mult;
			break;
		case 2:
			q[0] = (MGet(m, 0, 2) + MGet(m, 2, 0)) * mult;
			q[1] = (MGet(m, 2, 1) + MGet(m, 1, 2)) * mult;
			q[3] = (MGet(m, 1, 0) - MGet(m, 0, 1)) * mult;
			break;
		case 3:
			q[0] = (MGet(m, 2, 1) - MGet(m, 1, 2)) * mult;
			q[1] = (MGet(m, 0, 2) - MGet(m, 2, 0)) * mult;
			q[2] = (MGet(m, 1, 0) - MGet(m, 0, 1)) * mult;
			break;
		}

		return Quaternion(q[0], q[1], q[2], q[3]);
	}
}

namespace eagle
{
	Transform::Transform()
		: mPosition(0, 0, 0)
		, mRotation(Rotation_t::Identity())
		, mScale(1, 1, 1)
		, mCompanion(*this)
		, mIsChanged(false)
	{
		static std::once_flag flag{};
		std::call_once(flag, ExecutionOrder::Set<Transform>, 100ui64);
	}

	Transform::~Transform()
	{

	}

	void Transform::set(Vec3 _pos, Quaternion _rot, Vec3 _scale, Space _relativeTo)
	{
		if (_relativeTo == Space::Relative)
		{
			mPosition = _pos;
			mRotation = _rot;
			mScale = _scale;
		}
		else
		{
			mPosition = worldToLocalPos(_pos);
			mRotation = worldToLocalRot(_rot);
			mScale = worldToLocalScale(_scale);
		}

		mIsChanged = true;
	}

	void Transform::setPos(double _x, double _y, double _z, Space _relativeTo)
	{
		if (_relativeTo == Space::Relative)
		{
			mPosition.set(_x, _y, _z);
		}
		else
		{
			mPosition = worldToLocalPos(Vec3{ _x,_y,_z });
		}

		mIsChanged = true;
	}

	void Transform::setPos(Vec3 _pos, Space _relativeTo)
	{
		if (_relativeTo == Space::Relative)
		{
			mPosition = _pos;
		}
		else
		{
			mPosition = worldToLocalPos(_pos);
		}

		mIsChanged = true;
	}

	void Transform::setPos(double _x, double _y, Space _relativeTo)
	{
		if (_relativeTo == Space::Relative)
		{
			mPosition.x = _x;
			mPosition.y = _y;
		}
		else
		{
			Vec3 local = worldToLocalPos(Vec3{ _x,_y,0 });
			mPosition.x = local.x;
			mPosition.y = local.y;
		}

		mIsChanged = true;
	}

	void Transform::setPos(Vec2 _pos, Space _relativeTo)
	{
		if (_relativeTo == Space::Relative)
		{
			mPosition.x = _pos.x;
			mPosition.y = _pos.y;
		}
		else
		{
			Vec3 local = worldToLocalPos(_pos.xy0());
			mPosition.x = local.x;
			mPosition.y = local.y;
		}

		mIsChanged = true;
	}

	void Transform::setRot(double _roll, double _pitch, double _yaw, Space _relativeTo)
	{
		if (_relativeTo == Space::Relative)
		{
			mRotation = Rotation_t::RollPitchYaw(_roll, _pitch, _yaw);
		}
		else
		{
			mRotation = worldToLocalRot(Rotation_t::RollPitchYaw(_roll, _pitch, _yaw));
		}

		mIsChanged = true;
	}

	void Transform::setRot(Vec3 _rollPitchYaw, Space _relativeTo)
	{
		if (_relativeTo == Space::Relative)
		{
			mRotation = Rotation_t::RollPitchYaw(_rollPitchYaw.x, _rollPitchYaw.y, _rollPitchYaw.z);
		}
		else
		{
			mRotation = worldToLocalRot(Rotation_t::RollPitchYaw(_rollPitchYaw.x, _rollPitchYaw.y, _rollPitchYaw.z));
		}

		mIsChanged = true;
	}

	void Transform::setRot(const Mat4x4& _mat, Space _relativeTo)
	{
		if (_relativeTo == Space::Relative)
		{
			mRotation = Rotation_t{ _mat };
		}
		else
		{
			mRotation = worldToLocalRot(Rotation_t{ _mat });
		}

		mIsChanged = true;
	}

	void Transform::setRot(Quaternion _rot, Space _relativeTo)
	{
		if (_relativeTo == Space::Relative)
		{
			mRotation = _rot;
		}
		else
		{
			mRotation = worldToLocalRot(_rot);
		}

		mIsChanged = true;
	}

	void Transform::setScale(double _x, double _y, double _z, Space _relativeTo)
	{
		if (_relativeTo == Space::Relative)
		{
			mScale.set(_x, _y, _z);
		}
		else
		{
			mScale = worldToLocalScale(Vec3{ _x,_y,_z });
		}

		mIsChanged = true;
	}

	void Transform::setScale(Vec3 _scale, Space _relativeTo)
	{
		if (_relativeTo == Space::Relative)
		{
			mScale = _scale;
		}
		else
		{
			mScale = worldToLocalScale(_scale);
		}

		mIsChanged = true;
	}

	Transform::Position_t Transform::getPos(Space _relativeTo) const
	{
		if (_relativeTo == Space::Relative)
		{
			return mPosition;
		}

		return getWorldPos();
	}

	Vec2 Transform::getPos2D(Space _relativeTo) const
	{
		return getPos(_relativeTo).xy();
	}

	Transform::Rotation_t Transform::getRot(Space _relativeTo) const
	{
		if (_relativeTo == Space::Relative)
		{
			return mRotation;
		}

		return getWorldRot();
	}

	double Transform::getRot2D(Space _relativeTo) const
	{
		return eulerAngles(_relativeTo).z;
	}

	Transform::Scale_t Transform::getScale(Space _relativeTo) const
	{
		if (_relativeTo == Space::Relative)
		{
			return mScale;
		}

		return getWorldScale();
	}

	Vec2 Transform::getScale2D(Space _relativeTo) const
	{
		return getScale(_relativeTo).xy();
	}

	Transform::Position_t Transform::getWorldPos() const
	{
		return localToWorldPos(mPosition);
	}

	Vec2 Transform::getWorldPos2D() const
	{
		return localToWorldPos(mPosition).xy();
	}

	Transform::Rotation_t Transform::getWorldRot() const
	{
		return localToWorldRot(mRotation);
	}

	double Transform::getWorldRot2D() const
	{
		return eulerAngles(Space::Absolute).z;
	}

	Transform::Axis_t Transform::getWorldScale() const
	{
		return localToWorldScale(mScale);
	}

	Vec2 Transform::getWorldScale2D() const
	{
		return localToWorldScale(mScale).xy();
	}

	void Transform::translate(double _x, double _y, double _z)
	{
		mPosition.moveBy(_x, _y, _z);

		mIsChanged = true;
	}

	void Transform::translate(Vec3 _translation)
	{
		mPosition.moveBy(_translation);

		mIsChanged = true;
	}

	void Transform::rotate(double _roll, double _pitch, double _yaw)
	{
		mRotation *= Rotation_t::RollPitchYaw(_roll, _pitch, _yaw);

		mIsChanged = true;
	}

	void Transform::rotate(Vec3 _rollPithcYaw)
	{
		mRotation *= Rotation_t::RollPitchYaw(_rollPithcYaw.x, _rollPithcYaw.y, _rollPithcYaw.z);

		mIsChanged = true;
	}

	void Transform::rotate(const Mat4x4& _mat)
	{
		mRotation *= Rotation_t{ _mat };

		mIsChanged = true;
	}

	void Transform::rotate(Quaternion _rotation)
	{
		mRotation *= _rotation;

		mIsChanged = true;
	}

	void Transform::rotate(Vec3 _axis, double _angle)
	{
		mRotation *= Quaternion::RotationAxis(_axis, _angle);

		mIsChanged = true;
	}

	void Transform::rotate2D(double _angle)
	{
		rotate(Mat4x4::RotateZ(_angle));
	}

	void Transform::scaling(double _sx, double _sy, double _sz)
	{
		mScale *= Vec3{ _sx,_sy,_sz };

		mIsChanged = true;
	}

	void Transform::scaling(Vec3 _scale)
	{
		mScale *= _scale;

		mIsChanged = true;
	}

	Transform::Axis_t Transform::forward() const
	{
		return getWorldRot() * Vec3::Forward();
	}

	Transform::Axis_t Transform::up() const
	{
		return getWorldRot() * Vec3::Up();
	}

	Transform::Axis_t Transform::right() const
	{
		return getWorldRot() * Vec3::Right();
	}

	Vec3 Transform::eulerAngles(Space _relativeTo) const
	{
		Mat4x4 m{ (_relativeTo == Space::Absolute) ? getWorldRot() : mRotation };
		Vec3 euler{ 0,0,0 };
		euler.x = Atan2(MGet(m, 2, 1), MGet(m, 2, 2));
		euler.y = Asin(MGet(m, 2, 0));
		euler.z = -Atan2(MGet(m, 1, 0), MGet(m, 0, 0));
		return euler;
	}

	void Transform::lookAt(const Transform& _target)
	{
		lookAt(_target.getWorldPos());
	}

	void Transform::lookAt(Vec3 _worldPos)
	{
		Float3 z = (_worldPos - getWorldPos()).normalized();
		Float3 x = Vec3::Up().cross(z).normalized();
		Float3 y = z.cross(x).normalized();

		Mat4x4 m = Mat4x4::Set(
			x.x, y.x, z.x, 0.f,
			x.y, y.y, z.y, 0.f,
			x.z, y.z, z.z, 0.f,
			0.f, 0.f, 0.f, 1.f
		);

		mRotation = QLookAt(m);
	}

	void Transform::lookAtDirection(Vec3 _direction)
	{
		Float3 z = _direction.normalized();
		Float3 x = Vec3::Up().cross(z).normalized();
		Float3 y = z.cross(x).normalized();

		Mat4x4 m = Mat4x4::Set(
			x.x, y.x, z.x, 0.f,
			x.y, y.y, z.y, 0.f,
			x.z, y.z, z.z, 0.f,
			0.f, 0.f, 0.f, 1.f
		);

		mRotation = QLookAt(m);
	}

	Transform::Position_t Transform::localToWorldPos(Vec3 _localPos) const
	{
		Companion<Transform>* parent = mCompanion.getParent();

		if (not parent)
		{
			return _localPos;
		}

		Quaternion rot{ mRotation };

		Mat4x4 m{ rot };

		_localPos = m.transformPoint(_localPos);

		while (parent)
		{
			m = Mat4x4{ parent->self().mRotation };

			_localPos = m.transformPoint(_localPos);

			_localPos += parent->self().mPosition;

			parent = parent->getParent();
		}

		return _localPos;
	}

	Transform::Rotation_t Transform::localToWorldRot(Quaternion _localRot) const
	{
		Companion<Transform>* parent = mCompanion.getParent();

		if (not parent)
		{
			return _localRot;
		}

		do
		{
			//親のローカル回転を加算
			_localRot *= parent->self().mRotation;

			parent = parent->getParent();

		} while (parent);

		return _localRot;
	}

	Transform::Scale_t Transform::localToWorldScale(Vec3 _localScale) const
	{
		Companion<Transform>* parent = mCompanion.getParent();

		if (not parent)
		{
			return _localScale;
		}

		do
		{
			//親のローカルスケールを乗算
			_localScale *= parent->self().mScale;

			parent = parent->getParent();

		} while (parent);

		return _localScale;
	}

	Transform::Position_t Transform::worldToLocalPos(Vec3 _worldPos) const
	{
		Companion<Transform>* parent = mCompanion.getParent();

		if (not parent)
		{
			return _worldPos;
		}

		//親のワールド座標
		Vec3 parentWorldPos = parent->self().getWorldPos();

		return _worldPos - parentWorldPos;
	}

	Transform::Rotation_t Transform::worldToLocalRot(Quaternion _worldRot) const
	{
		Companion<Transform>* parent = mCompanion.getParent();

		if (not parent)
		{
			return _worldRot;
		}

		//親のワールド回転
		Quaternion parentWorldRot = parent->self().getWorldRot();

		return _worldRot * parentWorldRot.conjugate();
	}

	Transform::Scale_t Transform::worldToLocalScale(Vec3 _worldScale) const
	{
		Companion<Transform>* parent = mCompanion.getParent();

		if (not parent)
		{
			return _worldScale;
		}

		//親のワールド軸スケール
		Vec3 parentWorldScale = parent->self().getWorldScale();

		//ゼロ除算の防止
		if (parentWorldScale.x == 0.0 or
			parentWorldScale.y == 0.0 or
			parentWorldScale.z == 0.0)
		{
			return Vec3{ 0,0,0 };
		}

		return _worldScale / parentWorldScale;
	}

	void Transform::joinCompanion(Transform& _parent)
	{
		mCompanion.join(_parent.mCompanion);

		mPosition = worldToLocalPos(mPosition);
		mRotation = worldToLocalRot(mRotation);
		mScale = worldToLocalScale(mScale);
	}

	void Transform::withdrawCompanion()
	{
		mPosition = localToWorldPos(mPosition);
		mRotation = localToWorldRot(mRotation);
		mScale = localToWorldScale(mScale);

		mCompanion.withdraw();
	}

	Transform::Transform_handle Transform::getParent() const noexcept
	{
		if (auto pParent = mCompanion.getParent(); pParent)
		{
			return pParent->self().getTransform();
		}
		return WeakObject<Transform>().lock();
	}

	Transform::Transform_handle Transform::getRoot() const noexcept
	{
		if (auto pRoot = mCompanion.getRoot(); pRoot)
		{
			return pRoot->self().getTransform();
		}
		return WeakObject<Transform>().lock();
	}

	bool Transform::isChanged() const noexcept
	{
		return mIsChanged;
	}

	void Transform::dispose()
	{
		mCompanion.withdraw();
	}

	void Transform::update()
	{
		mIsChanged = false;
	}
}
