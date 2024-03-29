﻿/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Copyright © 2019 chibayuki@foxmail.com

RefCounter
Version 19.12.5.0000

This file is part of RefCounter

RefCounter is released under the GPLv3 license
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#define DEBUG (1)

#include <map>

using std::map;

// 引用计数器
class RefCounter
{
private:
	static map<void*, size_t> _Obj;

	using pair = map<void*, size_t>::value_type;
	using iterator = map<void*, size_t>::iterator;

	__declspec(allocator) static void* __cdecl operator new(size_t _Size);
	__declspec(allocator) static void* __cdecl operator new(size_t _Size, const std::nothrow_t& _Tag) noexcept;
	__declspec(allocator) static void* __cdecl operator new[](size_t _Size);
	__declspec(allocator) static void* __cdecl operator new[](size_t _Size, const std::nothrow_t& _Tag) noexcept;

	static void __cdecl operator delete(void* _Block) noexcept;
	static void __cdecl operator delete(void* _Block, const std::nothrow_t& _Tag) noexcept;
	static void __cdecl operator delete[](void* _Block) noexcept;
	static void __cdecl operator delete[](void* _Block, const std::nothrow_t& _Tag) noexcept;
	static void __cdecl operator delete(void* _Block, size_t _Size) noexcept;
	static void __cdecl operator delete[](void* _Block, size_t _Size) noexcept;

#ifdef __cpp_aligned_new
	__declspec(allocator) static void* __cdecl operator new(size_t _Size, std::align_val_t _Al);
	__declspec(allocator) static void* __cdecl operator new(size_t _Size, std::align_val_t _Al, const std::nothrow_t& _Tag) noexcept;
	__declspec(allocator) static void* __cdecl operator new[](size_t _Size, std::align_val_t _Al);
	__declspec(allocator) static void* __cdecl operator new[](size_t _Size, std::align_val_t _Al, const std::nothrow_t& _Tag) noexcept;

	static void __cdecl operator delete(void* _Block, std::align_val_t _Al) noexcept;
	static void __cdecl operator delete(void* _Block, std::align_val_t _Al, const std::nothrow_t& _Tag) noexcept;
	static void __cdecl operator delete[](void* _Block, std::align_val_t _Al) noexcept;
	static void __cdecl operator delete[](void* _Block, std::align_val_t _Al, const std::nothrow_t& _Tag) noexcept;
	static void __cdecl operator delete(void* _Block, size_t _Size, std::align_val_t _Al) noexcept;
	static void __cdecl operator delete[](void* _Block, size_t _Size, std::align_val_t _Al) noexcept;
#endif

protected:
	RefCounter();
	RefCounter(const RefCounter&);
	RefCounter& operator=(const RefCounter&);
	virtual ~RefCounter();

	size_t Increase(void* ptr);
	size_t Decrease(void* ptr);

	inline const RefCounter* operator&() const
	{
		return this;
	}

#if DEBUG
public:
	static size_t GetCount(void* ptr);
#endif
};

// 支持引用计数与垃圾回收的指针
template <typename T> class Ref : private RefCounter
{
private:
	T* _Ptr;

	inline void _Attach()
	{
		Increase(_Ptr);
	}

	inline void _Detach()
	{
		if (Decrease(_Ptr) == 0 && _Ptr)
		{
			delete _Ptr;
		}
	}

public:
	Ref() :
		_Ptr(nullptr)
	{
		_Attach();
	}

	Ref(const Ref& ref) :
		_Ptr(ref._Ptr)
	{
		_Attach();
	}

	Ref(Ref&& ref) :
		_Ptr(ref._Ptr)
	{
		ref._Ptr = nullptr;
	}

	Ref(nullptr_t) :
		_Ptr(nullptr)
	{
		_Attach();
	}

	Ref(T* ptr) :
		_Ptr(ptr)
	{
		_Attach();
	}

	Ref(const T* ptr) :
		_Ptr(ptr)
	{
		_Attach();
	}

	Ref(const T& val)
	{
		_Ptr = new T();

		_Attach();

		*_Ptr = val;
	}

	Ref(T&& val)
	{
		_Ptr = new T();

		_Attach();

		*_Ptr = val;
	}

	Ref& operator=(const Ref& ref)
	{
		if (this == &ref)
		{
			return *this;
		}

		if (_Ptr != ref._Ptr)
		{
			_Detach();

			_Ptr = ref._Ptr;

			_Attach();
		}

		return *this;
	}

	Ref& operator=(Ref&& ref)
	{
		if (this == &ref)
		{
			return *this;
		}

		_Detach();

		_Ptr = ref._Ptr;
		ref._Ptr = nullptr;

		_Attach();

		return *this;
	}

	Ref& operator=(nullptr_t)
	{
		if (_Ptr != nullptr)
		{
			_Detach();

			_Ptr = nullptr;

			_Attach();
		}

		return *this;
	}

	Ref& operator=(T* ptr)
	{
		if (_Ptr != ptr)
		{
			_Detach();

			_Ptr = ptr;

			_Attach();
		}

		return *this;
	}

	Ref& operator=(const T* ptr)
	{
		if (_Ptr != ptr)
		{
			_Detach();

			_Ptr = const_cast<T*>(ptr);

			_Attach();
		}

		return *this;
	}

	Ref& operator=(const T& val)
	{
		if (_Ptr == nullptr)
		{
			_Detach();

			_Ptr = new T();

			_Attach();
		}

		*_Ptr = val;

		return *this;
	}

	Ref& operator=(T&& val)
	{
		if (_Ptr == nullptr)
		{
			_Detach();

			_Ptr = new T();

			_Attach();
		}

		*_Ptr = val;

		return *this;
	}

	virtual ~Ref()
	{
		_Detach();
	}

	inline bool operator==(const Ref& ref) const
	{
		return (_Ptr == ref._Ptr);
	}

	inline bool operator==(nullptr_t) const
	{
		return (_Ptr == nullptr);
	}

	inline bool operator==(T* ptr) const
	{
		return (_Ptr == ptr);
	}

	inline bool operator==(const T* ptr) const
	{
		return (_Ptr == ptr);
	}

	inline bool operator!=(const Ref& ref) const
	{
		return (_Ptr != ref._Ptr);
	}

	inline bool operator!=(nullptr_t) const
	{
		return (_Ptr != nullptr);
	}

	inline bool operator!=(T* ptr) const
	{
		return (_Ptr != ptr);
	}

	inline bool operator!=(const T* ptr) const
	{
		return (_Ptr != ptr);
	}

	inline T& operator*()
	{
		return *_Ptr;
	}

	inline const T& operator*() const
	{
		return *_Ptr;
	}

	inline T* operator->()
	{
		return _Ptr;
	}

	inline const T* operator->() const
	{
		return _Ptr;
	}

	inline operator T() const
	{
		return *_Ptr;
	}

	inline explicit operator bool() const
	{
		return (_Ptr != nullptr);
	}

	inline bool operator !() const
	{
		return (_Ptr == nullptr);
	}

	inline Ref RefCopy() const
	{
		return Ref(_Ptr);
	}

	inline Ref ShallowCopy() const
	{
		return Ref(*_Ptr);
	}
};
