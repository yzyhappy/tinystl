#ifndef _UNIQUE_PTR_H
#define _UNIQUE_PTR_H

#include <utility>

/**
 * 
 *  �����ָ�����ͣ���������ʱ�������Ҫ���÷�delete�ĺ���
 *  ���紫������ļ�ָ�룬����ָ���ʱ��ֱ�ӵ���delete�ǲ����ʵ�
 *  ������Ҫ����fclose����Ҫ���Զ����
 *
 *  �Զ����deleter�Էº��������ʹ��룬����()������
 *  unique_ptrֻ�ṩ�������delete������delete[]����(ģ���ƫ�ػ�)
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
	// ��������ʲôʱ���˻�Ϊָ�룿
	// 1. ��Ϊ�������ν�ȥ��ʱ�� 2. ��Ϊ����ֵ��3.��Ϊ��ֵ��������ֵ
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

	// ���������
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


	// ʵ�ֶ�̬��ӵ�������unique_ptr�����Զ���ת��Ϊӵ�и����unique_ptr��
	// ���ƹ��죿
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
	// �����Լ��ƶ��Լ�
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
	// �����Լ��ƶ��Լ�
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
