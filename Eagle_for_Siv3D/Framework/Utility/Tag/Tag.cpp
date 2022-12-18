#include "Tag.hpp"

namespace eagle
{
	Tag::Tag(const Tag& _other)
	{
		Tag tmp = _other.mAssetPtr->secured(*_other);
		mHashCode = tmp.mHashCode;
		mAssetPtr = tmp.mAssetPtr;
	}

	Tag& Tag::operator=(const Tag& _other)
	{
		if (mAssetPtr)
		{
			mAssetPtr->release(*this);
		}
		Tag tmp = _other.mAssetPtr->secured(*_other);
		mHashCode = tmp.mHashCode;
		mAssetPtr = tmp.mAssetPtr;
		return *this;
	}

	Tag::~Tag()
	{
		if (mAssetPtr)
		{
			mAssetPtr->release(*this);
		}
	}

	void Tag::set(const s3d::String& _name)
	{
		mAssetPtr->release(*this);
		Tag tmp = mAssetPtr->secured(_name);
		mHashCode = tmp.mHashCode;
	}

	const s3d::String& Tag::get() const
	{
		return mAssetPtr->get(mHashCode);
	}

	const s3d::String& Tag::operator*() const
	{
		return mAssetPtr->get(mHashCode);
	}

	const size_t Tag::code() const noexcept
	{
		return mHashCode;
	}

	const size_t Tag::count() const
	{
		return mAssetPtr->count(*this);
	}

	bool Tag::operator==(const Tag& _name) const noexcept
	{
		return mHashCode == _name.mHashCode;
	}

	bool Tag::operator==(const s3d::String& _name) const noexcept
	{
		return mHashCode == _name.hash();
	}

	bool Tag::operator!=(const Tag& _name) const noexcept
	{
		return mHashCode != _name.mHashCode;
	}

	bool Tag::operator!=(const s3d::String& _name) const noexcept
	{
		return mHashCode != _name.hash();
	}

	Tag::operator bool() const noexcept
	{
		return mAssetPtr != nullptr;
	}

	Tag::Tag(size_t _hash, TagAsset* _ptr) noexcept
		: mHashCode(_hash)
		, mAssetPtr(_ptr)
	{
	}

	Tag TagAsset::secured(const s3d::String& _name)
	{
		size_t hash = _name.hash();

		Info newInfo{};

		// 既に同じ名前が存在する
		if (mAssets.contains(hash))
		{
			newInfo = mAssets[hash];
			newInfo.count += 1;
			newInfo.maxCount += 1;
		}
		// 初めて生成される名前
		else
		{
			newInfo.name = _name;
			newInfo.count = 1;
			newInfo.maxCount = 1;
		}

		// 生成された名前を登録
		mAssets[hash] = newInfo;

		return Tag(hash, this);
	}

	void TagAsset::release(const Tag& _name)
	{
		size_t hash = _name.code();

		// Asset内に名前が存在しない
		if (not mAssets.contains(hash))
			return;

		Info& info = mAssets[hash];
		// 該当する名前のカウントを減らす
		info.count = Max(0, info.count - 1);
		// カウントが0になったので最大生成数もリセット
		if (info.count == 0)
		{
			info.maxCount = 0;
		}
	}

	const s3d::String& TagAsset::get(size_t _hash) const
	{
		return mAssets.at(_hash).name;
	}

	size_t TagAsset::count(const Tag& _name) const
	{
		if (mAssets.contains(_name.code()))
		{
			return mAssets.at(_name.code()).count;
		}

		return 0;
	}
}
