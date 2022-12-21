#include "Animation2D.hpp"

namespace eagle
{
	Animation2D::Animation2D()
		: mAssetTag()
		, mColor(255)
		, mPivot(0,0)
		, mOffset(0,0)
		, mParam()
		, mTimer(1.0s)
	{
	}

	Animation2D::~Animation2D()
	{
	}

	void Animation2D::update()
	{
		const auto progress = mTimer.progress0_1();

		for (const auto& keyEvent : mKeyEvents)
		{
			if (InRange(progress, keyEvent.beginSec, keyEvent.endSec))
			{
				if (keyEvent.callOnce and keyEvent.isCalled)
					continue;

				keyEvent.func();
			}
		}

		if (isComplated() and mParam.hasLoop)
		{
			restart();
		}
	}

	void Animation2D::draw(Vec2 _pos) const
	{
		auto tex = slisedTexture();
		const auto offset = -tex.region().getRelativePoint(mPivot.x, mPivot.y) + mOffset;
		tex.draw(_pos + offset, mColor);
	}

	void Animation2D::start()
	{
		mTimer.start();
	}

	void Animation2D::restart()
	{
		mTimer.restart();
	}

	void Animation2D::pause()
	{
		mTimer.pause();
	}

	void Animation2D::resume()
	{
		mTimer.resume();
	}

	void Animation2D::setTextureAssetTag(const AssetTag& _asseTag)
	{
		mAssetTag = _asseTag;
	}

	void Animation2D::addKeyEvent(const KeyEvent& _event)
	{
		mKeyEvents << _event;
	}

	void Animation2D::setDiffuseColor(const Color& _color)
	{
		mColor = _color;
	}

	void Animation2D::setPivot(Vec2 _pivot)
	{
		mPivot = _pivot;
	}

	void Animation2D::setOffset(Vec2 _offset)
	{
		mOffset = _offset;
	}

	void Animation2D::setParam(const Param& _param)
	{
		mParam = _param;
	}

	void Animation2D::setDuration(const Duration& _duration)
	{
		mTimer.setRemaining(_duration);
	}

	const AssetTag& Animation2D::getTextureAssetTag() const
	{
		return mAssetTag;
	}

	const Color& Animation2D::getDiffuseColor() const
	{
		return mColor;
	}

	Vec2 Animation2D::getPivot() const
	{
		return mPivot;
	}

	Vec2 Animation2D::getOffset() const
	{
		return mOffset;
	}

	const Animation2D::Param& Animation2D::getParam() const
	{
		return mParam;
	}

	Duration Animation2D::getDuration() const
	{
		return mTimer.duration();
	}

	bool Animation2D::isStarted() const
	{
		return mTimer.isStarted();
	}

	bool Animation2D::isRunning() const
	{
		return mTimer.isRunning();
	}

	bool Animation2D::isComplated() const
	{
		return mTimer.progress0_1() == 1.0;
	}

	bool Animation2D::isPaused() const
	{
		return mTimer.isPaused();
	}

	TextureRegion Animation2D::slisedTexture() const
	{
		const auto texture = TextureAsset(mAssetTag);

		const auto progress = mTimer.progress0_1();

		const auto size = texture.size() / mParam.division;

		const auto length = mParam.range.y - mParam.range.x;

		const auto index = mParam.range.x + static_cast<int32>(length * progress);

		const Point index2D{ index % mParam.division.x ,index / mParam.division.x };

		return texture(index2D * size, size);
	}
}
