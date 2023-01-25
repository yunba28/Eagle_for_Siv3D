﻿#pragma once

#include <Framework/Component.hpp>
#include <Common/Companion.hpp>

#include <Siv3D/Vector3D.hpp>
#include <Siv3D/Vector2D.hpp>
#include <Siv3D/Quaternion.hpp>
#include <Siv3D/Mat4x4.hpp>

using s3d::Vec3;
using s3d::Vec2;
using s3d::Quaternion;
using s3d::Mat4x4;

namespace eagle
{
	enum class Space
	{
		Absolute,
		Relative
	};

	class Transform : public Component
	{
	private:

		using Position_t = Vec3;

		using Rotation_t = Quaternion;

		using Scale_t = Vec3;

		using Axis_t = Vec3;

	public:

		Transform();

		virtual ~Transform();

		/*---------------------------------------- 設定関数 -----------------------------------------------*/

		/// @brief Transformの座標・回転・拡縮を設定
		void set(Vec3 _pos, Quaternion _rot, Vec3 _scale, Space _relativeTo = Space::Relative);

		void setPos(double _x, double _y, double _z, Space _relativeTo = Space::Relative);

		void setPos(Vec3 _pos, Space _relativeTo = Space::Relative);

		void setPos(double _x, double _y, Space _relativeTo = Space::Relative);

		void setPos(Vec2 _pos, Space _relativeTo = Space::Relative);

		void setRot(double _roll, double _pitch, double _yaw, Space _relativeTo = Space::Relative);

		void setRot(Vec3 _rollPitchYaw, Space _relativeTo = Space::Relative);

		void setRot(const Mat4x4& _mat, Space _relativeTo = Space::Relative);

		void setRot(Quaternion _rot, Space _relativeTo = Space::Relative);

		void setScale(double _x, double _y, double _z, Space _relativeTo = Space::Relative);

		void setScale(Vec3 _scale, Space _relativeTo = Space::Relative);

		/*---------------------------------------- 取得関数 -----------------------------------------------*/

		[[nodiscard]]
		Position_t getPos(Space _relativeTo = Space::Relative)const;

		[[nodiscard]]
		Vec2 getPos2D(Space _relativeTo = Space::Relative)const;

		[[nodiscard]]
		Rotation_t getRot(Space _relativeTo = Space::Relative)const;

		[[nodiscard]]
		double getRot2D(Space _relativeTo = Space::Relative)const;

		[[nodiscard]]
		Scale_t getScale(Space _relativeTo = Space::Relative)const;

		[[nodiscard]]
		Vec2 getScale2D(Space _relativeTo = Space::Relative)const;

		[[nodiscard]]
		Position_t getWorldPos()const;

		[[nodiscard]]
		Vec2 getWorldPos2D()const;

		[[nodiscard]]
		Rotation_t getWorldRot()const;

		[[nodiscard]]
		double getWorldRot2D()const;

		[[nodiscard]]
		Scale_t getWorldScale()const;

		[[nodiscard]]
		Vec2 getWorldScale2D()const;

		/*---------------------------------------- 汎用関数 -----------------------------------------------*/

		void translate(double _x, double _y, double _z);

		void translate(Vec3 _translation);

		void translate2D(double _x, double _y);

		void translate2D(Vec2 _translation);

		void rotate(double _roll, double _pitch, double _yaw);

		void rotate(Vec3 _rollPithcYaw);

		void rotate(const Mat4x4& _mat);

		void rotate(Quaternion _rotation);

		void rotate(Vec3 _axis, double _angle);

		void rotate2D(double _angle);

		void scaling(double _sx, double _sy, double _sz);

		void scaling(Vec3 _scale);

		void scaling2D(double _sx, double _sy);

		void scaling2D(Vec2 _scale);

		/// @brief トランスフォームのforward(Z)軸を取得する
		[[nodiscard]]
		Axis_t forward()const;

		/// @brief トランスフォームのup(Y)軸を取得
		[[nodiscard]]
		Axis_t up()const;

		/// @brief トランスフォームのright(X)軸を取得
		[[nodiscard]]
		Axis_t right()const;

		/// @brief トランスフォームの回転からオイラー角を取得する
		[[nodiscard]]
		Vec3 eulerAngles(Space _relativeTo = Space::Relative)const;

		void lookAt(const Transform& _target);

		void lookAt(Vec3 _worldPos);

		/*---------------------------------------- 変換関数 -----------------------------------------------*/

		[[nodiscard]]
		Position_t localToWorldPos(Vec3 _localPos)const;

		[[nodiscard]]
		Rotation_t localToWorldRot(Quaternion _localRot)const;

		[[nodiscard]]
		Scale_t localToWorldScale(Vec3 _localScale)const;

		[[nodiscard]]
		Position_t worldToLocalPos(Vec3 _worldPos)const;

		[[nodiscard]]
		Rotation_t worldToLocalRot(Quaternion _worldRot)const;

		[[nodiscard]]
		Scale_t worldToLocalScale(Vec3 _worldScale)const;

		/*---------------------------------------- その他関数 ---------------------------------------------*/

		/// @brief parentの子になる
		void joinCompanion(Transform& _parent);

		/// @brief 親との連携を切る
		void withdrawCompanion();

		/// @brief Transformの親を取得する
		/// @return 見つからなければ空のTransformを取得
		ObjectHandle<Transform> getParent()const noexcept;

		/// @brief Transformの一番上の親を取得する
		/// @return 見つからなければ空のTransformを取得
		ObjectHandle<Transform> getRoot()const noexcept;

		/// @brief transformの更新が行われてから座標・回転・拡縮のいずれかが変更されたか 
		bool isChanged()const noexcept;

	protected:

		virtual void awake()override {}

		virtual void dispose()override {}

		virtual void start()override {}

		virtual void update()override {}

		virtual void lateUpdate()override {}

		virtual void fixedUpdate()override {}

		virtual void onEnable()override {}

		virtual void onDisable()override {}

		virtual void _internalDispose()override;

		virtual void _internalUpdate()override;

	private:

		Position_t mPosition;

		Rotation_t mRotation;

		Scale_t mScale;

		Companion<Transform> mCompanion;

		bool mIsChanged;

	};
}