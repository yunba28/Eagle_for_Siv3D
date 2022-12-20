#include "Name.hpp"

namespace eagle
{
	Name::~Name()
	{
		if (mAssetPtr)
		{
			mAssetPtr->release(*this);
		}
	}

	void Name::set(const s3d::String& _name)
	{
		mAssetPtr->release(*this);
		mAssetPtr->secured(_name,*this);
	}

	const s3d::String& Name::get() const
	{
		return mAssetPtr->get(mHashCode);
	}

	const s3d::String& Name::operator*() const
	{
		return mAssetPtr->get(mHashCode);
	}

	const size_t Name::code() const noexcept
	{
		return mHashCode;
	}

	const size_t Name::count() const
	{
		return mAssetPtr->count(*this);
	}

	bool Name::operator==(const Name& _name) const noexcept
	{
		return mHashCode == _name.mHashCode;
	}

	bool Name::operator==(const s3d::String& _name) const noexcept
	{
		return mHashCode == _name.hash();
	}

	bool Name::operator!=(const Name& _name) const noexcept
	{
		return mHashCode != _name.mHashCode;
	}

	bool Name::operator!=(const s3d::String& _name) const noexcept
	{
		return mHashCode != _name.hash();
	}

	Name::operator bool() const noexcept
	{
		return mAssetPtr != nullptr;
	}

	Name::Name(size_t _hash, NameAsset* _ptr) noexcept
		: mHashCode(_hash)
		, mAssetPtr(_ptr)
	{
	}

	void NameAsset::secured(const s3d::String& _name, Name& _outName)
	{
		size_t hash = _name.hash();

		Info newInfo{};

		// 既に同じ名前が存在する
		if (mAssets.contains(hash))
		{
			Info& info = mAssets[hash];

			newInfo.name = info.name + U" #{}"_fmt(info.maxCount);
			newInfo.base = info.base;
			newInfo.count += 1;
			newInfo.maxCount += 1;

			info.count += 1;
			info.maxCount += 1;

			hash = newInfo.name.hash();
		}
		// 初めて生成される名前
		else
		{
			newInfo.name = _name;
			newInfo.base = hash;
			newInfo.count = 1;
			newInfo.maxCount = 1;
		}

		// 生成された名前を登録
		mAssets.emplace(hash, newInfo);

		_outName.mHashCode = hash;
		_outName.mAssetPtr = this;
	}

	void NameAsset::release(const Name& _name)
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
		// ベースになった名前のカウントも減らす
		if (info.base != hash)
		{
			Info& baseInfo = mAssets[info.base];
			baseInfo.count = Max(0, baseInfo.count - 1);
			if (baseInfo.count == 0)
			{
				baseInfo.maxCount = 0;
			}
		}
	}

	const s3d::String& NameAsset::get(size_t _hash) const
	{
		return mAssets.at(_hash).name;
	}

	size_t NameAsset::count(const Name& _name) const
	{
		if (mAssets.contains(_name.code()))
		{
			return mAssets.at(_name.code()).count;
		}

		return 0;
	}
}
