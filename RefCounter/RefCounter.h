/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Copyright © 2019 chibayuki@foxmail.com

RefCounter
Version 19.10.21.0000

This file is part of RefCounter

RefCounter is released under the GPLv3 license
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

using namespace std;

// 支持引用计数与垃圾回收的指针
template <typename T> class Ref
{
private:
	T* _Ptr;
	size_t* _Count;

	inline void _Increase()
	{
		(*_Count)++;
	}

	inline void _Decrease()
	{
		(*_Count)--;

		if (*_Count == 0)
		{
			if (_Ptr)
			{
				delete _Ptr;
			}

			delete _Count;
		}
	}

public:
	Ref()
	{
		_Ptr = nullptr;
		_Count = new size_t(0);
	}

	Ref(const Ref& ref)
	{
		_Ptr = ref._Ptr;
		_Count = ref._Count;

		_Increase();
	}

	Ref(nullptr_t)
	{
		_Ptr = nullptr;
		_Count = new size_t(1);
	}

	Ref(T* ptr)
	{
		_Ptr = ptr;
		_Count = new size_t(1);
	}

	Ref(const T* ptr)
	{
		_Ptr = const_cast<T*>(ptr);
		_Count = new size_t(1);
	}

	Ref(const T& val)
	{
		_Ptr = new T();
		_Count = new size_t(1);

		*_Ptr = val;
	}

	Ref(T&& val)
	{
		_Ptr = new T();
		_Count = new size_t(1);

		*_Ptr = val;
	}

	Ref& operator=(const Ref& ref)
	{
		if (this == &ref)
		{
			return *this;
		}

		if (_Ptr == ref._Ptr)
		{
			return *this;
		}

		_Decrease();

		_Ptr = ref._Ptr;
		_Count = ref._Count;

		_Increase();

		return *this;
	}

	Ref& operator=(nullptr_t)
	{
		if (_Ptr == nullptr)
		{
			return *this;
		}

		_Decrease();

		_Ptr = nullptr;
		_Count = new size_t(1);

		return *this;
	}

	Ref& operator=(T* ptr)
	{
		if (_Ptr == ptr)
		{
			return *this;
		}

		_Decrease();

		_Ptr = ptr;
		_Count = new size_t(1);

		return *this;
	}

	Ref& operator=(const T* ptr)
	{
		if (_Ptr == ptr)
		{
			return *this;
		}

		_Decrease();

		_Ptr = const_cast<T*>(ptr);
		_Count = new size_t(1);

		return *this;
	}

	Ref& operator=(const T& val)
	{
		_Decrease();

		_Ptr = new T();
		_Count = new size_t(1);

		*_Ptr = val;

		return *this;
	}

	Ref& operator=(T&& val)
	{
		_Decrease();

		_Ptr = new T();
		_Count = new size_t(1);

		*_Ptr = val;

		return *this;
	}

	~Ref()
	{
		_Decrease();
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
};