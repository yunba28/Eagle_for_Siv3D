#pragma once

#include <stdexcept>

namespace eagle
{
	/// @brief オブジェクトに親子関係を持たせる
	/// @tparam T 管理するオブジェクト
	template<class T>
	class Companion
	{
	public:

		Companion(T& _self);

		Companion(const Companion&) = delete;

		Companion(Companion&&)noexcept = default;

		~Companion();

		explicit operator bool()const noexcept;

		Companion& operator=(const Companion&) = delete;

		Companion& operator=(Companion&&)noexcept = default;

		/// @brief 自身の参照を取得
		T& self()noexcept;

		/// @brief 自身の参照を取得
		const T& self()const noexcept;

		/// @brief parentのCompanionに参加する
		/// @param _parent 
		void join(Companion& _parent);

		/// @brief parentのCompanionから抜ける
		void withdraw();

		/// @brief 親オブジェクトを取得
		/// @return 取得できなかったらnullptr
		Companion* getParent()const noexcept;

		/// @brief Companionの最上位オブジェクトを取得
		/// @return 取得できなかったらnullptr
		Companion* getRoot()const noexcept;

		/// @brief 指定したインデックスの子を取得
		/// @param _index 
		/// @return 指定する子のインデックス
		Companion* getChildren(size_t _index = 0ui64)const;

		/// @brief 親オブジェクトを取得
		/// @return 取得出来たらtrue
		bool tryGetParent(Companion*& _out)const noexcept;

		/// @brief Companionの最上位オブジェクトを取得
		/// @return 取得出来たらtrue
		bool tryGetRoot(Companion*& _out)const noexcept;

		/// @brief 指定したインデックスの子を取得
		/// @param out 取得した子を格納するCompanionのポインタ
		/// @param _index 指定する子のインデックス
		bool tryGetChildren(Companion*& _out, size_t _index = 0ui64)const noexcept;

		size_t childCount()const noexcept;

	private:

		T& mSelf;

		Companion* mParent{ nullptr };
		Companion* mPrev{ nullptr };
		Companion* mNext{ nullptr };
		Companion* mChildren{ nullptr };

		size_t mChildCount;

	private:

		void uncheckedGetChildren(Companion*& _out, size_t _index)const noexcept;

	};
}

namespace eagle
{
	template<class T>
	Companion<T>::Companion(T& _self)
		: mSelf(_self)
	{
	}

	template<class T>
	Companion<T>::~Companion()
	{
		//親から外れる
		withdraw();

		Companion* pChild = mChildren;

		//全ての親子関係を断ち切る
		while (pChild)
		{
			pChild->withdraw();
			pChild = pChild->mNext;
		}
	}

	template<class T>
	Companion<T>::operator bool() const noexcept
	{
		return (mParent != nullptr) or (mPrev != nullptr) or (mNext != nullptr) or (mChildren != nullptr);
	}

	template<class T>
	T& Companion<T>::self() noexcept
	{
		return mSelf;
	}

	template<class T>
	const T& Companion<T>::self() const noexcept
	{
		return mSelf;
	}

	template<class T>
	void Companion<T>::join(Companion& _parent)
	{
		//既に同じ親に所属している
		if (mParent == &_parent)
			return;

		//以前の親から離脱する
		withdraw();

		//既にいる親の子の手前に割り込む
		if (_parent.mChildren)
		{
			_parent.mChildren->mNext = this;
			mPrev = _parent.mChildren;
		}
		//自身が最初の子
		else
		{
			_parent.mChildren = this;
		}

		_parent.mChildCount += 1;

		mParent = &_parent;
	}

	template<class T>
	void Companion<T>::withdraw()
	{
		//どこにも所属していない
		if (not mParent)
			return;

		//自身が最後の子
		if ((not mPrev) and (not mNext))
		{
			mParent->mChildren = nullptr;
		}

		//先に所属した兄弟がいる
		if (mPrev)
		{
			mPrev->mNext = mNext;
		}

		//後に所属した兄弟がいる
		if (mNext)
		{
			mNext->mPrev = mPrev;
		}

		if (mParent->mChildCount > 0)
		{
			mParent->mChildCount -= 1;
		}

		mParent = nullptr;
		mNext = nullptr;
		mPrev = nullptr;
	}

	template<class T>
	Companion<T>* Companion<T>::getParent() const noexcept
	{
		return mParent;
	}

	template<class T>
	Companion<T>* Companion<T>::getRoot() const noexcept
	{
		//自分が最上位のオブジェクト
		if (not mParent)
		{
			return nullptr;
		}

		Companion* pParent = mParent;

		//親が最上位オブジェクト
		if (not pParent->mParent)
		{
			return pParent;
		}

		while (pParent->mParent)
		{
			pParent = pParent->mParent;
		}

		return pParent;
	}

	template<class T>
	Companion<T>* Companion<T>::getChildren(size_t _index) const
	{
		//範囲外指定
		if (_index < *mChildCount)
		{
			Companion* pChild = nullptr;
			uncheckedGetChildren(pChild, _index);
			return pChild;
		}

		throw std::out_of_range{ "out of range" };
	}

	template<class T>
	bool Companion<T>::tryGetParent(Companion*& _out) const noexcept
	{
		if (mParent)
		{
			_out = mParent;
			return true;
		}

		return false;
	}

	template<class T>
	bool Companion<T>::tryGetRoot(Companion*& _out) const noexcept
	{
		//自分が最上位のオブジェクト
		if (not mParent)
		{
			return false;
		}

		Companion* pParent = mParent;

		//親が最上位オブジェクト
		if (not pParent->mParent)
		{
			_out = pParent;
			return true;
		}

		while (pParent->mParent)
		{
			pParent = pParent->mParent;
		}

		_out = pParent;

		return true;
	}

	template<class T>
	bool Companion<T>::tryGetChildren(Companion*& _out, size_t _index)const noexcept
	{
		//範囲外指定
		if (not (_index < *mChildCount))
		{
			return false;
		}

		uncheckedGetChildren(_out, _index);

		return true;
	}

	template<class T>
	size_t Companion<T>::childCount() const noexcept
	{
		return mChildCount;
	}

	template<class T>
	void Companion<T>::uncheckedGetChildren(Companion*& _out, size_t _index) const noexcept
	{
		Companion* pChild = mChildren;

		size_t index = 0ui64;

		while (pChild)
		{
			if (index == _index)
			{
				_out = pChild;
				break;
			}

			pChild = pChild->mNext;
			index += 1;
		}
	}
}
