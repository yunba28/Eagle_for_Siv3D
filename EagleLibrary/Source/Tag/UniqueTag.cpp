#include <Classes/Tag.hpp>

namespace eagle::Internal
{
	std::pair<int32_t, bool> DigitNumber(int32_t num, int32_t digit)
	{
		int32_t d1 = static_cast<int32_t>(std::pow(10, digit + 1));
		int32_t d2 = static_cast<int32_t>(std::pow(10, digit));

		// 整数の最大桁数を超えている
		if (d2 > num)
		{
			return { 0,false };
		}

		// 一の桁
		if (d2 == 1)
		{
			return 
			{
				num % d1,
				true
			};
		}
		// 十の桁以降
		else
		{
			return
			{
				((num % d1) - (num % d2)) / d2,
				true
			};
		}
	}

	std::u32string ConcatNumber(const std::u32string& src, int32_t num)
	{
		std::u32string number{};
		const int32_t codePointBase{ 48 };
		for (int32_t digit = 0;; ++digit)
		{
			auto [n, ret] = DigitNumber(num, digit);
			if (not ret)
				break;
			number.push_back(static_cast<char32_t>(codePointBase + n));
		}
		std::reverse(number.begin(), number.end());
		return src + (U"(" + number + U")");
	}

	class UniqueTagAssetDetail final : Uncopyable
	{
	public:

		UniqueTagAssetDetail();

		~UniqueTagAssetDetail();

		size_t secured(const std::u32string& _tag, UniqueTag& _out);

		void release(const UniqueTag& _tag);

		const std::u32string& get(size_t _hash)const;

		size_t count(const UniqueTag& _tag)const;

	private:

		struct Info
		{
			std::u32string name{ U"" };
			size_t base{ 0 };
			int32_t count{ 0 };
			int32_t maxCount{ 0 };
		};

		std::unordered_map<size_t, Info> mAssets;

	};

	UniqueTagAssetDetail::UniqueTagAssetDetail()
		: mAssets()
	{

	}

	UniqueTagAssetDetail::~UniqueTagAssetDetail()
	{

	}

	size_t UniqueTagAssetDetail::secured(const std::u32string& _tag, UniqueTag& _out)
	{
		size_t hash = std::hash<std::u32string>{}(_tag);

		Info newInfo{};

		// 既に同じ名前が存在する
		if (mAssets.contains(hash))
		{
			Info& info = mAssets[hash];

			newInfo.name = ConcatNumber(info.name, info.maxCount);
			newInfo.base = info.base;
			newInfo.count += 1;
			newInfo.maxCount += 1;

			info.count += 1;
			info.maxCount += 1;

			hash = std::hash<std::u32string>{}(newInfo.name);
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
		info.count = std::max(0, info.count - 1);
		// カウントが0になったので最大生成数もリセット
		if (info.count == 0)
		{
			info.maxCount = 0;
		}
		// ベースになった名前のカウントも減らす
		if (info.base != hash)
		{
			Info& baseInfo = mAssets[info.base];
			baseInfo.count = std::max(0, baseInfo.count - 1);
			if (baseInfo.count == 0)
			{
				baseInfo.maxCount = 0;
			}
		}
	}

	const std::u32string& UniqueTagAssetDetail::get(size_t _hash)const
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
		, mAssetWeak(std::forward<WeakObject<Internal::UniqueTagAssetDetail>>(_other.mAssetWeak))
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
		mAssetWeak = std::forward<WeakObject<Internal::UniqueTagAssetDetail>>(_other.mAssetWeak);
		return *this;
	}

	void UniqueTag::set(const std::u32string& _tag)
	{
		if (auto asset = mAssetWeak.lock(); asset)
		{
			asset->release(*this);
			asset->secured(_tag, *this);
		}
	}

	const std::u32string& UniqueTag::get()const
	{
		if (auto asset = mAssetWeak.lock(); asset)
		{
			return asset->get(mHashCode);
		}

		throw std::exception{"no reference."};
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

		throw std::exception{ "no reference." };
	}

	UniqueTag::operator bool()const noexcept
	{
		return mAssetWeak.operator bool();
	}

	const std::u32string& UniqueTag::operator*()const
	{
		return get();
	}

	bool UniqueTag::operator==(const UniqueTag& _tag)const noexcept
	{
		return mHashCode == _tag.mHashCode;
	}

	bool UniqueTag::operator==(const std::u32string& _tag)const noexcept
	{
		return mHashCode == std::hash<std::u32string>{}(_tag);
	}

	bool UniqueTag::operator!=(const UniqueTag& _tag)const noexcept
	{
		return mHashCode != _tag.mHashCode;
	}

	bool UniqueTag::operator!=(const std::u32string& _tag)const noexcept
	{
		return mHashCode != std::hash<std::u32string>{}(_tag);
	}

	UniqueTagAsset::UniqueTagAsset()
		: mAssetDetail(MakeShared<Internal::UniqueTagAssetDetail>())
	{

	}

	UniqueTagAsset::~UniqueTagAsset()
	{

	}

	void UniqueTagAsset::secured(const std::u32string& _tag, UniqueTag& _out)
	{
		_out.mHashCode = mAssetDetail->secured(_tag, _out);
		_out.mAssetWeak = mAssetDetail.weak();
	}

	void UniqueTagAsset::release(const UniqueTag& _tag)
	{
		mAssetDetail->release(_tag);
	}

	const std::u32string& UniqueTagAsset::get(size_t _hash)const
	{
		return mAssetDetail->get(_hash);
	}

	size_t UniqueTagAsset::count(const UniqueTag& _tag)const
	{
		return mAssetDetail->count(_tag);
	}
}