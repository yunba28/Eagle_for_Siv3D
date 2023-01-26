#include <Components/Camera/FollowingCamera2D.hpp>

#include <Framework/Actor.hpp>
#include <Framework/BasicScene.hpp>

namespace eagle
{
	FollowingCamera2D::FollowingCamera2D()
		: mCamera()
		, mTarget()
		, mOffset(0, 0)
		, mSweepRate(0.5)
	{
	}

	FollowingCamera2D::~FollowingCamera2D()
	{
	}

	void FollowingCamera2D::setTarget(const WeakObject<Transform>& _target)
	{
		// フリーの状態からターゲットを設定したら処理を有効にする
		if (mTarget.empty() and _target)
		{
			setEnable(true);
		}
		// 空のターゲットを指定した場合は以降の処理を無効にする
		else if (_target.empty())
		{
			setEnable(false);
		}

		mTarget = _target;
	}

	void FollowingCamera2D::setRegion(const s3d::Optional<s3d::RectF>& _region)
	{
		mRegion = _region;
	}

	void FollowingCamera2D::setOffset(s3d::Vec2 _offset)
	{
		mOffset = _offset;
	}

	void FollowingCamera2D::setSweepRate(double _rate)
	{
		mSweepRate = _rate;
	}

	ObjectHandle<Transform> FollowingCamera2D::getTarget() const noexcept
	{
		return mTarget.lock();
	}

	s3d::Vec2 FollowingCamera2D::getOffset() const noexcept
	{
		return mOffset;
	}

	double FollowingCamera2D::getSweepRate() const noexcept
	{
		return mSweepRate;
	}

	void FollowingCamera2D::awake()
	{
		// 2Dカメラを取得
		mCamera = getActor()->getSceneObject()->getCamera2D();

		// 不必要な関数を無効にする
		setEnable({ false,true,false,false });
	}

	void FollowingCamera2D::lateUpdate()
	{
		auto camera = mCamera.lock();

		auto target = mTarget.lock();

		auto targetPos = target->getPos2D();

		Vec2 cameraPos = targetPos + mOffset;

		Vec2 currentPos = camera->getCenter();

		Vec2 sweepedPos = currentPos.lerp(cameraPos, mSweepRate);

		if (mRegion)
		{
			sweepedPos.clamp(*mRegion);
		}

		camera->setCenter(sweepedPos);

		getTransform()->setPos(sweepedPos, Space::Absolute);
	}
}
