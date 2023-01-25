#include <Framework/Detail/UniqueTag.hpp>

namespace eagle::backend
{
	class UniqueTagAssetDetail final : Uncopyable
	{
	public:

		UniqueTagAssetDetail();

		~UniqueTagAssetDetail();

		size_t secured(const String& _tag);

		void release(const UniqueTag& _tag);

		const String& get(size_t _hash)const;

		size_t count(const UniqueTag& _tag)const;

	private:

		struct Info
		{
			String name{ U"" };
			size_t base{ 0 };
			int32_t count{ 0 };
			int32_t maxCount{ 0 };
		};

		HashTable<size_t, Info> mAssets;

	};

	UniqueTagAssetDetail::UniqueTagAssetDetail()
		: mAssets()
	{

	}

	UniqueTagAssetDetail::~UniqueTagAssetDetail()
	{

	}

	size_t UniqueTagAssetDetail::secured(const String& _tag)
	{
		size_t hash = _tag.hash();

		Info newInfo{};

		// 既に同じ名前が存在する
		if (mAssets.contains(hash))
		{
			Info& info = mAssets[hash];

			newInfo.name = info.name + U"({})"_fmt(info.maxCount);
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
			newInfo.name = _tag;
			newInfo.base = hash;
			newInfo.count = 1;
			newInfo.maxCount = 1;
		}

		// 生成された名前を登録
		mAssets.emplace(hash, newInfo);

		return hash;
	}

	void UniqueTagAssetDetail::release(const UniqueTag& _tag)
	{
		size_t hash = _tag.code();

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

	const String& UniqueTagAssetDetail::get(size_t _hash)const
	{
		return mAssets.at(_hash).name;
	}

	size_t UniqueTagAssetDetail::count(const UniqueTag& _tag)const
	{
		return mAssets.at(_tag.code()).count;
	}
}

namespace eagle
{
	UniqueTag::UniqueTag()
		: mHashCode(0)
		, mAssetWeak()
	{

	}

	UniqueTag::UniqueTag(UniqueTag&& _other)noexcept
		: mHashCode(std::forward<size_t>(_other.mHashCode))
		, mAssetWeak(std::forward<WeakObject<backend::UniqueTagAssetDetail>>(_other.mAssetWeak))
	{
		_other.mHashCode = 0;
		_other.mAssetWeak.reset();
	}

	UniqueTag::~UniqueTag()
	{
		if (auto asset = mAssetWeak.lock(); asset)
		{
			asset->release(*this);
		}
	}

	UniqueTag& UniqueTag::operator=(UniqueTag&& _other)noexcept
	{
		mHashCode = std::forward<size_t>(_other.mHashCode);
		mAssetWeak = std::forward<WeakObject<backend::UniqueTagAssetDetail>>(_other.mAssetWeak);
		return *this;
	}

	void UniqueTag::set(const String& _tag)
	{
		if (auto asset = mAssetWeak.lock(); asset)
		{
			asset->release(*this);
			mHashCode = asset->secured(_tag);
		}
	}

	const String& UniqueTag::get()const
	{
		if (auto asset = mAssetWeak.lock(); asset)
		{
			return asset->get(mHashCode);
		}

		throw Error{U"null reference."};
	}

	size_t UniqueTag::code()const
	{
		return mHashCode;
	}

	size_t UniqueTag::count()const
	{
		if (auto asset = mAssetWeak.lock(); asset)
		{
			return asset->count(*this);
		}

		throw Error{ U"null reference." };
	}

	UniqueTag::operator bool()const noexcept
	{
		return mAssetWeak.operator bool();
	}

	const String& UniqueTag::operator*()const
	{
		return get();
	}

	bool UniqueTag::operator==(const UniqueTag& _tag)const noexcept
	{
		return mHashCode == _tag.mHashCode;
	}

	bool UniqueTag::operator==(const String& _tag)const noexcept
	{
		return mHashCode == std::hash<String>{}(_tag);
	}

	bool UniqueTag::operator!=(const UniqueTag& _tag)const noexcept
	{
		return mHashCode != _tag.mHashCode;
	}

	bool UniqueTag::operator!=(const String& _tag)const noexcept
	{
		return mHashCode != std::hash<String>{}(_tag);
	}

	UniqueTagAsset::UniqueTagAsset()
		: mAssetDetail(MakeShared<backend::UniqueTagAssetDetail>())
	{

	}

	UniqueTagAsset::~UniqueTagAsset()
	{

	}

	void UniqueTagAsset::secured(const String& _tag, UniqueTag& _out)
	{
		_out.mHashCode = mAssetDetail->secured(_tag);
		_out.mAssetWeak = mAssetDetail.weak();
	}

	void UniqueTagAsset::release(const UniqueTag& _tag)
	{
		mAssetDetail->release(_tag);
	}

	const String& UniqueTagAsset::get(size_t _hash)const
	{
		return mAssetDetail->get(_hash);
	}

	size_t UniqueTagAsset::count(const UniqueTag& _tag)const
	{
		return mAssetDetail->count(_tag);
	}
}
