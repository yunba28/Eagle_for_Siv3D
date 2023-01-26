#pragma once

#include <Framework/Behavior.hpp>

#include <Siv3D/Camera2D.hpp>
#include <Siv3D/Optional.hpp>
#include <Siv3D/RectF.hpp>

namespace eagle
{
	class FollowingCamera2D : public Behavior
	{
	public:

		FollowingCamera2D();

		virtual ~FollowingCamera2D();

		/// @brief カメラが追従するターゲットを設定する
		void setTarget(const WeakObject<Transform>& _target);

		/// @brief カメラが移動できる範囲を設定する
		void setRegion(const s3d::Optional<s3d::RectF>& _region);

		/// @brief カメラの中心がターゲットからどれだけ離れているかを設定する
		void setOffset(s3d::Vec2 _offset);

		/// @brief カメラの追従に余韻をつける
		void setSweepRate(double _rate);

		ObjectHandle<Transform> getTarget()const noexcept;

		s3d::Vec2 getOffset()const noexcept;

		double getSweepRate()const noexcept;

	private:

		virtual void awake()override;

		virtual void update()override {}

		virtual void lateUpdate()override;

	private:

		WeakObject<s3d::Camera2D> mCamera;

		WeakObject<Transform> mTarget;

		s3d::Optional<s3d::RectF> mRegion;

		s3d::Vec2 mOffset;

		double mSweepRate;

	};
}
