#ifndef _UNIQUE_PTR_H
#define _UNIQUE_PTR_H

#include <utility>

/**
 * 
 *  传入的指针类型，在析构的时候可能需要调用非delete的函数
 *  比如传入的是文件指针，析构指针的时候直接调用delete是不合适的
 *  可能需要调用fclose，需要有自定义的
 *
 *  自定义的deleter以仿函数的类型传入，重载()操作符
 *  unique_ptr只提供最基本的delete，或者delete[]操作(模板的偏特化)
 */

template<class T>
struct DefaultDeleter
{
	void operator()(T* ptr) const
	{
		delete ptr;
	}
};

template<class T>
struct DefaultDeleter<T[]>
{
	// 数组类型什么时候退化为指针？
	// 1. 作为函数传参进去的时候； 2. 作为返回值；3.作为赋值操作的右值
	void operator() (T* ptr) const
	{
		delete[] ptr;
	}
};


template<class T, class Deleter = DefaultDeleter<T>>
class Unique_ptr
{
public:
	Unique_ptr() : _ptr(nullptr) {}
	Unique_ptr(T* ptr) : _ptr(ptr) {};
	Unique_ptr(const Unique_ptr&) = delete;
	Unique_ptr& operator=(const Unique_ptr&) = delete;

	Unique_ptr(const Unique_ptr&&);
	Unique_ptr&& operator=(const Unique_ptr&&);
		
	~Unique_ptr();

	T* get() const
	{
		return _ptr;
	}

	// 运算符重载
	T& operator*() const
	{
		return *_ptr;
	}

	T* operator->() const
	{
		return _ptr;
	}

	T* release()
	{
		T* ptr = _ptr;
		_ptr = nullptr;
		return ptr;
	}


	// 实现多态（拥有子类的unique_ptr允许自动的转换为拥有父类的unique_ptr）
	// 类似构造？
	template<class U>
	Unique_ptr(Unique_ptr<U> &&that)
	{
		this->_ptr = that.release();
	}

private:
	T* _ptr;

};


template<class T, class Deleter>
class Unique_ptr<T[], Deleter> : Unique_ptr<T, Deleter>
{
public:
	Unique_ptr(T *ptr) : Unique_ptr<T, Deleter>(ptr) {}
};

template<class T, class Deleter>
Unique_ptr<T, Deleter>::Unique_ptr(const Unique_ptr&& other)
{
	// 避免自己移动自己
	if (other._ptr == this->_ptr)
	{
		return;
	}
	Deleter{}(this->_ptr);
	this->_ptr = other._ptr;
	other._ptr = nullptr;
}


template <class T, class Deleter>
Unique_ptr<T, Deleter>&& Unique_ptr<T, Deleter>::operator=(const Unique_ptr&& other)
{
	// 避免自己移动自己
	if (other._ptr == this->_ptr)
	{
		return this;
	}
	Deleter{}(this->_ptr);
	this->_ptr = other._ptr;
	other._ptr = nullptr;
	return this;
}


template<class T, class Deleter>
Unique_ptr<T, Deleter>::~Unique_ptr()
{
	if (this->_ptr == nullptr) return;
	Deleter{}(this->_ptr);
}



// make_unique
template<class T, class ...Args>
Unique_ptr<T> Make_unique(Args&& ...arg)
{
	return Unique_ptr(new T(std::forward<Args>(arg)...));
}


#endif //
