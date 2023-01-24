#pragma once

#include <memory>

namespace eagle
{

	template<class T, class Deleter = std::default_delete<T>>
	class UniqueObject : protected std::unique_ptr<T, Deleter>
	{
	public:

		using Base_t = std::unique_ptr<T, Deleter>;

		using Base_t::unique_ptr;
		using Base_t::operator=;
		using Base_t::swap;
		using Base_t::reset;
		using Base_t::operator*;
		using Base_t::operator->;
		using Base_t::operator bool;

		[[nodiscard]] bool empty()const noexcept;

	};

	template<class T, class Deleter>
	class UniqueObject<T[], Deleter> : protected std::unique_ptr<T[], Deleter>
	{
	public:

		using Base_t = std::unique_ptr<T[], Deleter>;

		using Base_t::unique_ptr;
		using Base_t::operator=;
		using Base_t::swap;
		using Base_t::reset;
		using Base_t::operator[];
		using Base_t::operator bool;

		[[nodiscard]] bool empty()const noexcept;

	};

	template<class T, class Deleter>
	bool UniqueObject<T, Deleter>::empty()const noexcept
	{
		return not this->operator bool();
	}

	template<class T, class ...Args, std::enable_if_t<not std::is_array_v<T>, int> = 0>
	[[nodiscard]] UniqueObject<T> MakeUnique(Args&& ...args)
	{
		return UniqueObject<T>{new T{ std::forward<Args>(args)... }};
	}

	template<class T, std::enable_if_t<std::is_array_v<T> and std::extent_v<T> == 0, int> = 0>
	[[nodiscard]] UniqueObject<T> MakeUnique(const size_t size)
	{
		using Element = std::remove_extent_t<T>;
		return UniqueObject<T>{new Element[size]()};
	}

	template<class T, class ...Args, std::enable_if_t<std::extent_v<T> != 0, int> = 0>
	void MakeUnique(Args&&...) = delete;
}

namespace eagle
{

	template<class T>
	class SharedObject;

	template<class T>
	class WeakObject;

	template<class T>
	class ObjectHandle;

	template<class T>
	class SharedObject final
	{
	public:

		SharedObject()noexcept;

		SharedObject(std::nullptr_t)noexcept;

		template<class U>
		explicit SharedObject(U* _ptr);

		template<class U>
		explicit SharedObject(const SharedObject<U>& _other);

		template<class U, class Deleter>
		SharedObject(U* _ptr, Deleter _deleter);

		template<class U, class Deleter, class Allocator>
		SharedObject(U* _ptr, Deleter _deleter, Allocator _allocator);

		template<class Deleter>
		SharedObject(std::nullptr_t, Deleter _deleter);

		template<class Deleter, class Allocator>
		SharedObject(std::nullptr_t, Deleter _deleter, Allocator _allocator);

		SharedObject(const SharedObject<T>&) = default;

		SharedObject(SharedObject<T>&&)noexcept = default;

		template<class U>
		SharedObject(SharedObject<U>&& _other);

		template<class U>
		SharedObject(const WeakObject<U>& _weak);

		~SharedObject() = default;

		SharedObject<T>& operator=(const SharedObject<T>&) = default;

		template<class U>
		SharedObject<T>& operator=(const SharedObject<U>& _other);

		SharedObject<T>& operator=(SharedObject<T>&&) = default;

		template<class U>
		SharedObject<T>& operator=(SharedObject<U>&& _other);

		SharedObject<T>& operator=(std::nullptr_t)noexcept;

		void swap(SharedObject& _other)noexcept;

		void reset()noexcept;

		template<class U>
		void reset(U* _ptr);

		template<class U, class Deleter>
		void reset(U* _ptr, Deleter _deleter);

		template<class U, class Deleter, class Allocator>
		void reset(U* _ptr, Deleter _deleter, Allocator _allocator);

		[[nodiscard]] WeakObject<T> weak()const noexcept;

		template<class U>
		[[nodiscard]] WeakObject<U> weak()const;

		[[nodiscard]] ObjectHandle<T> lock()const noexcept;

		template<class U>
		[[nodiscard]] ObjectHandle<U> lock()const;

		[[nodiscard]] T& operator*()noexcept;

		[[nodiscard]] const T& operator*()const noexcept;

		[[nodiscard]] T* operator->()const noexcept;

		[[nodiscard]] bool empty()const noexcept;

		[[nodiscard]] explicit operator bool()const noexcept;

	private:

		SharedObject(std::shared_ptr<T>&& _shared)noexcept;

	private:

		std::shared_ptr<T> mPtr;

		template<class U>
		friend class SharedObject;
		template<class U>
		friend class WeakObject;

		template<class T, class ...Args>
		friend std::enable_if_t<not std::is_array_v<T>, SharedObject<T>> MakeShared(Args&& ...args);

		template<class T, class Allocator, class ...Args>
		friend std::enable_if_t<not std::is_array_v<T>, SharedObject<T>> AllocateShared(const Allocator& allocator, Args&& ...args);

		template<class To, class From>
		friend SharedObject<To> Cast(const SharedObject<From>&);

		template<class T>
		friend bool operator==(const WeakObject<T>& a, const SharedObject<T>& b);

		template<class T>
		friend bool operator==(const ObjectHandle<T>& a, const SharedObject<T>& b);

		template<class T>
		friend bool operator==(const SharedObject<T>& a, const SharedObject<T>& b);

		template<class T>
		friend bool operator==(const SharedObject<T>& a, const WeakObject<T>& b);

		template<class T>
		friend bool operator==(const SharedObject<T>& a, const ObjectHandle<T>& b);

		template<class T>
		friend bool operator==(const SharedObject<T>& a, std::nullptr_t);

		template<class T>
		friend bool operator!=(const WeakObject<T>& a, const SharedObject<T>& b);

		template<class T>
		friend bool operator!=(const ObjectHandle<T>& a, const SharedObject<T>& b);

		template<class T>
		friend bool operator!=(const SharedObject<T>& a, const SharedObject<T>& b);

		template<class T>
		friend bool operator!=(const SharedObject<T>& a, const WeakObject<T>& b);

		template<class T>
		friend bool operator!=(const SharedObject<T>& a, const ObjectHandle<T>& b);

		template<class T>
		friend bool operator!=(const SharedObject<T>& a, std::nullptr_t);
	};

	template<class T>
	class WeakObject final
	{
	public:

		WeakObject()noexcept;

		WeakObject(std::nullptr_t)noexcept;

		WeakObject(const WeakObject<T>&)noexcept = default;

		template<class U>
		WeakObject(const WeakObject<U>& _other)noexcept;

		WeakObject(WeakObject<T>&&)noexcept = default;

		template<class U>
		WeakObject(WeakObject<U>&& _other)noexcept;

		template<class U>
		WeakObject(const SharedObject<U>& _shared);

		WeakObject& operator=(const WeakObject&)noexcept = default;

		template<class U>
		WeakObject& operator=(const WeakObject<U>& _other)noexcept;

		WeakObject& operator=(WeakObject&&)noexcept = default;

		template<class U>
		WeakObject& operator=(WeakObject<U>&& _other)noexcept;

		void reset()noexcept;

		void swap(WeakObject<T>& _other)noexcept;

		[[nodiscard]] ObjectHandle<T> lock()const noexcept;

		template<class U>
		[[nodiscard]] ObjectHandle<U> lock()const noexcept;

		[[nodiscard]] bool expired() const noexcept;

		[[nodiscard]] bool empty()const noexcept;

		[[nodiscard]] explicit operator bool()const noexcept;

	private:

		std::weak_ptr<T> mWeak;

		template<class U>
		friend class SharedObject;
		template<class U>
		friend class WeakObject;
		template<class U>
		friend class ObjectHandle;

		template<class To, class From>
		friend WeakObject<To> Cast(const WeakObject<From>&);

		template<class T>
		friend bool operator==(const SharedObject<T>& a, const WeakObject<T>& b);

		template<class T>
		friend bool operator==(const ObjectHandle<T>& a, const WeakObject<T>& b);

		template<class T>
		friend bool operator==(const WeakObject<T>& a, const SharedObject<T>& b);

		template<class T>
		friend bool operator==(const WeakObject<T>& a, const WeakObject<T>& b);

		template<class T>
		friend bool operator==(const WeakObject<T>& a, const ObjectHandle<T>& b);

		template<class T>
		friend bool operator==(const WeakObject<T>& a, std::nullptr_t);

		template<class T>
		friend bool operator!=(const SharedObject<T>& a, const WeakObject<T>& b);

		template<class T>
		friend bool operator!=(const ObjectHandle<T>& a, const WeakObject<T>& b);

		template<class T>
		friend bool operator!=(const WeakObject<T>& a, const SharedObject<T>& b);

		template<class T>
		friend bool operator!=(const WeakObject<T>& a, const WeakObject<T>& b);

		template<class T>
		friend bool operator!=(const WeakObject<T>& a, const ObjectHandle<T>& b);

		template<class T>
		friend bool operator!=(const WeakObject<T>& a, std::nullptr_t);
	};

	template<class T>
	class ObjectHandle final
	{
	public:

		ObjectHandle() = delete;

		ObjectHandle(const ObjectHandle&) = delete;

		ObjectHandle(ObjectHandle&&) = default;

		ObjectHandle& operator=(const ObjectHandle&) = delete;

		ObjectHandle& operator=(ObjectHandle&&) = default;

		~ObjectHandle()noexcept = default;

		[[nodiscard]] T& operator*()noexcept;

		[[nodiscard]] const T& operator*()const noexcept;

		[[nodiscard]] T* operator->()const noexcept;

		[[nodiscard]] bool empty()const noexcept;

		[[nodiscard]] explicit operator bool()const noexcept;

	private:

		std::shared_ptr<T> mShared;

		template<class U>
		friend class SharedObject;
		template<class U>
		friend class WeakObject;
		template<class U>
		friend class ObjectHandle;

		ObjectHandle(const std::shared_ptr<T>& _shared)noexcept;

		template<class U>
		ObjectHandle(const std::shared_ptr<U>& _shared)noexcept;

		template<class T>
		friend bool operator==(const SharedObject<T>& a, const ObjectHandle<T>& b);

		template<class T>
		friend bool operator==(const WeakObject<T>& a, const ObjectHandle<T>& b);

		template<class T>
		friend bool operator==(const ObjectHandle<T>& a, const SharedObject<T>& b);

		template<class T>
		friend bool operator==(const ObjectHandle<T>& a, const WeakObject<T>& b);

		template<class T>
		friend bool operator==(const ObjectHandle<T>& a, const ObjectHandle<T>& b);

		template<class T>
		friend bool operator==(const ObjectHandle<T>& a, std::nullptr_t);

		template<class T>
		friend bool operator!=(const SharedObject<T>& a, const ObjectHandle<T>& b);

		template<class T>
		friend bool operator!=(const WeakObject<T>& a, const ObjectHandle<T>& b);

		template<class T>
		friend bool operator!=(const ObjectHandle<T>& a, const SharedObject<T>& b);

		template<class T>
		friend bool operator!=(const ObjectHandle<T>& a, const WeakObject<T>& b);

		template<class T>
		friend bool operator!=(const ObjectHandle<T>& a, const ObjectHandle<T>& b);

		template<class T>
		friend bool operator!=(const ObjectHandle<T>& a, std::nullptr_t);
	};

	template<class T, class ...Args>
	std::enable_if_t<not std::is_array_v<T>, SharedObject<T>> MakeShared(Args&& ...args);

	template<class T, class Allocator, class ...Args>
	std::enable_if_t<not std::is_array_v<T>, SharedObject<T>> AllocateShared(const Allocator& allocator, Args&& ...args);

	template<class To, class From>
	SharedObject<To> Cast(const SharedObject<From>& from);

	template<class To, class From>
	WeakObject<To> Cast(const WeakObject<From>& from);

	template<class T>
	bool operator==(const SharedObject<T>& a, const SharedObject<T>& b);

	template<class T>
	bool operator==(const SharedObject<T>& a, const WeakObject<T>& b);

	template<class T>
	bool operator==(const SharedObject<T>& a, const ObjectHandle<T>& b);

	template<class T>
	bool operator==(const WeakObject<T>& a, const SharedObject<T>& b);

	template<class T>
	bool operator==(const WeakObject<T>& a, const WeakObject<T>& b);

	template<class T>
	bool operator==(const WeakObject<T>& a, const ObjectHandle<T>& b);

	template<class T>
	bool operator==(const ObjectHandle<T>& a, const SharedObject<T>& b);

	template<class T>
	bool operator==(const ObjectHandle<T>& a, const WeakObject<T>& b);

	template<class T>
	bool operator==(const ObjectHandle<T>& a, const ObjectHandle<T>& b);

	template<class T>
	bool operator==(const SharedObject<T>& a, std::nullptr_t);

	template<class T>
	bool operator==(const WeakObject<T>& a, std::nullptr_t);

	template<class T>
	bool operator==(const ObjectHandle<T>& a, std::nullptr_t);

	template<class T>
	bool operator!=(const SharedObject<T>& a, const SharedObject<T>& b);

	template<class T>
	bool operator!=(const SharedObject<T>& a, const WeakObject<T>& b);

	template<class T>
	bool operator!=(const SharedObject<T>& a, const ObjectHandle<T>& b);

	template<class T>
	bool operator!=(const WeakObject<T>& a, const SharedObject<T>& b);

	template<class T>
	bool operator!=(const WeakObject<T>& a, const WeakObject<T>& b);

	template<class T>
	bool operator!=(const WeakObject<T>& a, const ObjectHandle<T>& b);

	template<class T>
	bool operator!=(const ObjectHandle<T>& a, const SharedObject<T>& b);

	template<class T>
	bool operator!=(const ObjectHandle<T>& a, const WeakObject<T>& b);

	template<class T>
	bool operator!=(const ObjectHandle<T>& a, const ObjectHandle<T>& b);

	template<class T>
	bool operator!=(const SharedObject<T>& a, std::nullptr_t);

	template<class T>
	bool operator!=(const WeakObject<T>& a, std::nullptr_t);

	template<class T>
	bool operator!=(const ObjectHandle<T>& a, std::nullptr_t);
}

namespace eagle
{

	template<class T>
	inline SharedObject<T>::SharedObject()noexcept
		: mPtr()
	{

	}

	template<class T>
	inline SharedObject<T>::SharedObject(std::nullptr_t)noexcept
		: mPtr()
	{

	}

	template<class T>
	template<class U>
	inline SharedObject<T>::SharedObject(U* _ptr)
		: mPtr(_ptr)
	{

	}

	template<class T>
	template<class U, class Deleter>
	inline SharedObject<T>::SharedObject(U* _ptr, Deleter _deleter)
		: mPtr(_ptr, _deleter)
	{

	}

	template<class T>
	template<class U, class Deleter, class Allocator>
	inline SharedObject<T>::SharedObject(U* _ptr, Deleter _deleter, Allocator _allocator)
		: mPtr(_ptr, _deleter, _allocator)
	{

	}

	template<class T>
	template<class Deleter>
	inline SharedObject<T>::SharedObject(std::nullptr_t, Deleter _deleter)
		: mPtr(std::nullptr_t, _deleter)
	{

	}

	template<class T>
	template<class Deleter, class Allocator>
	inline SharedObject<T>::SharedObject(std::nullptr_t, Deleter _deleter, Allocator _allocator)
		: mPtr(std::nullptr_t, _deleter, _allocator)
	{

	}

	template<class T>
	template<class U>
	inline SharedObject<T>::SharedObject(const SharedObject<U>& _other)
		: mPtr(_other.mPtr)
	{

	}

	template<class T>
	template<class U>
	inline SharedObject<T>::SharedObject(SharedObject<U>&& _other)
		: mPtr(std::forward<decltype(_other.mPtr)>(_other.mPtr))
	{

	}

	template<class T>
	template<class U>
	inline SharedObject<T>::SharedObject(const WeakObject<U>& _weak)
		: mPtr(_weak.mWeak)
	{

	}

	template<class T>
	template<class U>
	inline SharedObject<T>& SharedObject<T>::operator=(const SharedObject<U>& _other)
	{
		mPtr = _other.mPtr;
		return *this;
	}

	template<class T>
	template<class U>
	inline SharedObject<T>& SharedObject<T>::operator=(SharedObject<U>&& _other)
	{
		mPtr = std::move(_other.mPtr);
		return *this;
	}

	template<class T>
	inline SharedObject<T>& SharedObject<T>::operator=(std::nullptr_t)noexcept
	{
		mPtr.reset();
		return *this;
	}

	template<class T>
	inline void SharedObject<T>::swap(SharedObject& _other)noexcept
	{
		mPtr.swap(_other.mPtr);
	}

	template<class T>
	inline void SharedObject<T>::reset()noexcept
	{
		mPtr.rest();
	}

	template<class T>
	template<class U>
	inline void SharedObject<T>::reset(U* _ptr)
	{
		mPtr.reset(_ptr);
	}

	template<class T>
	template<class U, class Deleter>
	inline void SharedObject<T>::reset(U* _ptr, Deleter _deleter)
	{
		mPtr.reset(_ptr, _deleter);
	}

	template<class T>
	template<class U, class Deleter, class Allocator>
	inline void SharedObject<T>::reset(U* _ptr, Deleter _deleter, Allocator _allocator)
	{
		mPtr.reset(_ptr, _deleter, _allocator);
	}

	template<class T>
	inline WeakObject<T> SharedObject<T>::weak()const noexcept
	{
		return WeakObject<T>{*this};
	}

	template<class T>
	template<class U>
	inline WeakObject<U> SharedObject<T>::weak()const
	{
		return WeakObject<U>{Cast<U>(*this)};
	}

	template<class T>
	inline ObjectHandle<T> SharedObject<T>::lock()const noexcept
	{
		return ObjectHandle<T>{mPtr};
	}

	template<class T>
	template<class U>
	inline ObjectHandle<U> SharedObject<T>::lock()const
	{
		return ObjectHandle<U>{Cast<U>(*this).mPtr};
	}

	template<class T>
	inline T& SharedObject<T>::operator*()noexcept
	{
		return *mPtr;
	}

	template<class T>
	inline const T& SharedObject<T>::operator*()const noexcept
	{
		return *mPtr;
	}

	template<class T>
	inline T* SharedObject<T>::operator->()const noexcept
	{
		return mPtr.get();
	}

	template<class T>
	inline bool SharedObject<T>::empty()const noexcept
	{
		return not mPtr.operator bool();
	}

	template<class T>
	inline SharedObject<T>::operator bool()const noexcept
	{
		return mPtr.operator bool();
	}

	template<class T>
	inline SharedObject<T>::SharedObject(std::shared_ptr<T>&& _shared)noexcept
		: mPtr(std::forward<std::shared_ptr<T>>(_shared))
	{

	}

	template<class T>
	inline WeakObject<T>::WeakObject()noexcept
		: mWeak()
	{

	}

	template<class T>
	inline WeakObject<T>::WeakObject(std::nullptr_t)noexcept
		: mWeak()
	{

	}

	template<class T>
	template<class U>
	inline WeakObject<T>::WeakObject(const WeakObject<U>& _other)noexcept
		: mWeak(_other.mWeak)
	{

	}

	template<class T>
	template<class U>
	inline WeakObject<T>::WeakObject(WeakObject<U>&& _other)noexcept
		: mWeak(std::forward<decltype(_other.mWeak)>(_other.mWeak))
	{

	}

	template<class T>
	template<class U>
	inline WeakObject<T>::WeakObject(const SharedObject<U>& _other)
		: mWeak(_other.mPtr)
	{

	}

	template<class T>
	template<class U>
	inline WeakObject<T>& WeakObject<T>::operator=(const WeakObject<U>& _other) noexcept
	{
		mWeak = _other.mWeak;
		return *this;
	}

	template<class T>
	template<class U>
	inline WeakObject<T>& WeakObject<T>::operator=(WeakObject<U>&& _other) noexcept
	{
		mWeak = std::forward<decltype(_other.mWeak)>(_other.mWeak);
		return *this;
	}

	template<class T>
	inline void WeakObject<T>::reset()noexcept
	{
		mWeak.reset();
	}

	template<class T>
	inline void WeakObject<T>::swap(WeakObject<T>& _other)noexcept
	{
		mWeak.swap(_other.mWeak);
	}

	template<class T>
	inline ObjectHandle<T> WeakObject<T>::lock()const noexcept
	{
		return ObjectHandle<T>{mWeak.lock()};
	}

	template<class T>
	template<class U>
	inline ObjectHandle<U> WeakObject<T>::lock()const noexcept
	{
		return ObjectHandle<U>{Cast<U>(*this).mWeak.lock()};
	}

	template<class T>
	inline bool WeakObject<T>::expired()const noexcept
	{
		return mWeak.expired();
	}

	template<class T>
	inline bool WeakObject<T>::empty()const noexcept
	{
		return mWeak.expired();
	}

	template<class T>
	inline WeakObject<T>::operator bool()const noexcept
	{
		return not mWeak.expired();
	}

	template<class T>
	inline T& ObjectHandle<T>::operator*()noexcept
	{
		return *mShared;
	}

	template<class T>
	inline const T& ObjectHandle<T>::operator*()const noexcept
	{
		return *mShared;
	}

	template<class T>
	inline T* ObjectHandle<T>::operator->()const noexcept
	{
		return mShared.get();
	}

	template<class T>
	inline bool ObjectHandle<T>::empty()const noexcept
	{
		return not mShared.operator bool();
	}

	template<class T>
	inline ObjectHandle<T>::operator bool()const noexcept
	{
		return mShared.operator bool();
	}

	template<class T>
	inline ObjectHandle<T>::ObjectHandle(const std::shared_ptr<T>& _shared)noexcept
		: mShared(_shared)
	{

	}

	template<class T>
	template<class U>
	inline ObjectHandle<T>::ObjectHandle(const std::shared_ptr<U>& _shared)noexcept
		: mShared(_shared)
	{

	}
}

namespace eagle
{

	template<class T, class ...Args>
	std::enable_if_t<not std::is_array_v<T>, SharedObject<T>> MakeShared(Args&& ...args)
	{
		return SharedObject<T>{std::make_shared<T>(std::forward<Args>(args)...)};
	}

	template<class T, class Allocator, class ...Args>
	std::enable_if_t<not std::is_array_v<T>, SharedObject<T>> AllocateShared(const Allocator& allocator, Args&& ...args)
	{
		return SharedObject<T>{std::allocate_shared<T>(allocator, std::forward<Args>(args)...)};
	}

	template<class To, class From>
	SharedObject<To> Cast(const SharedObject<From>& from)
	{
		if constexpr (std::is_same_v<To, From>)
		{
			return from;
		}
		else if constexpr (std::is_base_of_v<To, From>)
		{
			return SharedObject<To>{std::static_pointer_cast<To>(from.mPtr)};
		}
		else if constexpr (std::is_base_of_v<From, To>)
		{
			return SharedObject<To>{std::dynamic_pointer_cast<To>(from.mPtr)};
		}
		else if constexpr (std::is_convertible_v<To*, From*>)
		{
			return SharedObject<To>{std::reinterpret_pointer_cast<To>(from.mPtr)};
		}
		else
		{
			throw std::bad_cast{ "bad-cast" };
		}
	}

	template<class To, class From>
	WeakObject<To> Cast(const WeakObject<From>& from)
	{
		if constexpr (std::is_same_v<To, From>)
		{
			return from;
		}

		WeakObject<To> to{};
		auto sp = from.mWeak.lock();

		if constexpr (std::is_base_of_v<To, From>)
		{
			to.mWeak = std::static_pointer_cast<To>(sp);
			return to;
		}
		else if constexpr (std::is_base_of_v<From, To>)
		{
			to.mWeak = std::dynamic_pointer_cast<To>(sp);
			return to;
		}
		else if constexpr (std::is_convertible_v<To*, From*>)
		{
			to.mWeak = std::reinterpret_pointer_cast<To>(sp);
			return to;
		}
		else
		{
			throw std::bad_cast{ "bad-cast" };
		}
	}
}

namespace eagle
{
	template<class T>
	bool operator==(const SharedObject<T>& a, const SharedObject<T>& b)
	{
		return a.mPtr == b.mPtr;
	}

	template<class T>
	bool operator==(const SharedObject<T>& a, const WeakObject<T>& b)
	{
		return a.mPtr == b.mWeak.lock();
	}

	template<class T>
	bool operator==(const SharedObject<T>& a, const ObjectHandle<T>& b)
	{
		return a.mPtr == b.mShared;
	}

	template<class T>
	bool operator==(const WeakObject<T>& a, const SharedObject<T>& b)
	{
		return a.mWeak.lock() == b.mPtr;
	}

	template<class T>
	bool operator==(const WeakObject<T>& a, const WeakObject<T>& b)
	{
		return a.mWeak.lock() == b.mWeak.lock();
	}

	template<class T>
	bool operator==(const WeakObject<T>& a, const ObjectHandle<T>& b)
	{
		return a.mWeak.lock() == b.mShared;
	}

	template<class T>
	bool operator==(const ObjectHandle<T>& a, const SharedObject<T>& b)
	{
		return a.mShared == b.mPtr;
	}

	template<class T>
	bool operator==(const ObjectHandle<T>& a, const WeakObject<T>& b)
	{
		return a.mShared == b.mWeak.lock();
	}

	template<class T>
	bool operator==(const ObjectHandle<T>& a, const ObjectHandle<T>& b)
	{
		return a.mShared == b.mShared;
	}

	template<class T>
	bool operator==(const SharedObject<T>& a, std::nullptr_t)
	{
		return a.mPtr == nullptr;
	}

	template<class T>
	bool operator==(const WeakObject<T>& a, std::nullptr_t)
	{
		return a.expired();
	}

	template<class T>
	bool operator==(const ObjectHandle<T>& a, std::nullptr_t)
	{
		return a.mShared == nullptr;
	}

	template<class T>
	bool operator!=(const SharedObject<T>& a, const SharedObject<T>& b)
	{
		return a.mPtr != b.mPtr;
	}

	template<class T>
	bool operator!=(const SharedObject<T>& a, const WeakObject<T>& b)
	{
		return a.mPtr != b.mWeak.lock();
	}

	template<class T>
	bool operator!=(const SharedObject<T>& a, const ObjectHandle<T>& b)
	{
		return a.mPtr != b.mShared;
	}

	template<class T>
	bool operator!=(const WeakObject<T>& a, const SharedObject<T>& b)
	{
		return a.mWeak.lock() != b.mPtr;
	}

	template<class T>
	bool operator!=(const WeakObject<T>& a, const WeakObject<T>& b)
	{
		return a.mWeak.lock() != b.mWeak.lock();
	}

	template<class T>
	bool operator!=(const WeakObject<T>& a, const ObjectHandle<T>& b)
	{
		return a.mWeak.lock() != b.mShared;
	}

	template<class T>
	bool operator!=(const ObjectHandle<T>& a, const SharedObject<T>& b)
	{
		return a.mShared != b.mPtr;
	}

	template<class T>
	bool operator!=(const ObjectHandle<T>& a, const WeakObject<T>& b)
	{
		return a.mShared != b.mWeak.lock();
	}

	template<class T>
	bool operator!=(const ObjectHandle<T>& a, const ObjectHandle<T>& b)
	{
		return a.mShared != b.mShared;
	}

	template<class T>
	bool operator!=(const SharedObject<T>& a, std::nullptr_t)
	{
		return a.mPtr != nullptr;
	}

	template<class T>
	bool operator!=(const WeakObject<T>& a, std::nullptr_t)
	{
		return not a.expired();
	}

	template<class T>
	bool operator!=(const ObjectHandle<T>& a, std::nullptr_t)
	{
		return a.mShared != nullptr;
	}
}