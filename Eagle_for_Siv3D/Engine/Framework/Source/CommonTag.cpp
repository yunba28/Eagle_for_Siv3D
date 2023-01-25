#include <Framework/Detail/CommonTag.hpp>

namespace eagle::backend
{
	class CommonTagAssetDetail final : Uncopyable
	{
	public:

		CommonTagAssetDetail();

		~CommonTagAssetDetail();

		size_t secured(const String& _tag);

		void release(const CommonTag& _tag);

		const String& get(size_t _hash)const;

		size_t count(const CommonTag& _tag)const;

	private:

		struct Info
		{
			String name{ U"" };
			int32_t count{ 0 };
		};

		HashTable<size_t, Info> mAssets;

	};

	CommonTagAssetDetail::CommonTagAssetDetail()
		: mAssets()
	{

	}

	CommonTagAssetDetail::~CommonTagAssetDetail()
	{

	}

	size_t CommonTagAssetDetail::secured(const String& _tag)
	{
		size_t hash = _tag.hash();

		// 既に同じ名前が存在する
		if (mAssets.contains(hash))
		{
			Info& info = mAssets[hash];
			info.count += 1;
		}
		// 初めて生成される名前
		else
		{
			Info newInfo{};
			newInfo.name = _tag;
			newInfo.count = 1;
			// 生成された名前を登録
			mAssets.emplace(hash, newInfo);
		}

		return hash;
	}

	void CommonTagAssetDetail::release(const CommonTag& _tag)
	{
		size_t hash = _tag.code();

		// Asset内に名前が存在しない
		if (not mAssets.contains(hash))
			return;

		Info& info = mAssets[hash];
		// 該当する名前のカウントを減らす
		info.count = std::max(0, info.count - 1);
	}

	const String& CommonTagAssetDetail::get(size_t _hash)const
	{
		return mAssets.at(_hash).name;
	}

	size_t CommonTagAssetDetail::count(const CommonTag& _tag)const
	{
		return mAssets.at(_tag.code()).count;
	}
}

namespace eagle
{
	CommonTag::CommonTag()
		: mHashCode(0)
		, mAssetWeak()
	{

	}

	CommonTag::CommonTag(CommonTag&& _other)noexcept
		: mHashCode(std::forward<size_t>(_other.mHashCode))
		, mAssetWeak(std::forward<WeakObject<backend::CommonTagAssetDetail>>(_other.mAssetWeak))
	{
		_other.mHashCode = 0;
		_other.mAssetWeak.reset();
	}

	CommonTag::~CommonTag()
	{
		if (auto asset = mAssetWeak.lock(); asset)
		{
			asset->release(*this);
		}
	}

	CommonTag& CommonTag::operator=(CommonTag&& _other)noexcept
	{
		mHashCode = std::forward<size_t>(_other.mHashCode);
		mAssetWeak = std::forward<WeakObject<backend::CommonTagAssetDetail>>(_other.mAssetWeak);
		return *this;
	}

	void CommonTag::set(const String& _tag)
	{
		if (auto asset = mAssetWeak.lock(); asset)
		{
			asset->release(*this);
			mHashCode = asset->secured(_tag);
		}
	}

	const String& CommonTag::get()const
	{
		if (auto asset = mAssetWeak.lock(); asset)
		{
			return asset->get(mHashCode);
		}

		throw Error{ U"null reference." };
	}

	size_t CommonTag::code()const
	{
		return mHashCode;
	}

	size_t CommonTag::count()const
	{
		if (auto asset = mAssetWeak.lock(); asset)
		{
			return asset->count(*this);
		}

		throw Error{ U"null reference." };
	}

	CommonTag::operator bool()const noexcept
	{
		return mAssetWeak.operator bool();
	}

	const String& CommonTag::operator*()const
	{
		return get();
	}

	bool CommonTag::operator==(const CommonTag& _tag)const noexcept
	{
		return mHashCode == _tag.mHashCode;
	}

	bool CommonTag::operator==(const String& _tag)const noexcept
	{
		return mHashCode == std::hash<String>{}(_tag);
	}

	bool CommonTag::operator!=(const CommonTag& _tag)const noexcept
	{
		return mHashCode != _tag.mHashCode;
	}

	bool CommonTag::operator!=(const String& _tag)const noexcept
	{
		return mHashCode != std::hash<String>{}(_tag);
	}

	CommonTagAsset::CommonTagAsset()
		: mAssetDetail(MakeShared<backend::CommonTagAssetDetail>())
	{

	}

	CommonTagAsset::~CommonTagAsset()
	{

	}

	void CommonTagAsset::secured(const String& _tag, CommonTag& _out)
	{
		_out.mHashCode = mAssetDetail->secured(_tag);
		_out.mAssetWeak = mAssetDetail.weak();
	}

	void CommonTagAsset::release(const CommonTag& _tag)
	{
		mAssetDetail->release(_tag);
	}

	const String& CommonTagAsset::get(size_t _hash)const
	{
		return mAssetDetail->get(_hash);
	}

	size_t CommonTagAsset::count(const CommonTag& _tag)const
	{
		return mAssetDetail->count(_tag);
	}
}
