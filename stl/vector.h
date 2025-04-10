#ifndef _VECTOR_H
#define _VECTOR_H

#include <type_traits>
#include <cstdlib>

/**
 * 仿照cpp reference做
 * 
 * 
 */

template<class T>
class Vector {
	
public:
    Vector() = default;
    Vector(size_t size, const T& t) {
        _size = size;
        _capacity = size * 2;
        _place = static_cast<T*>(malloc(size * 2 * sizeof(t)));
        for (int i = 0; i < size; i++) {
            // 拷贝构造
            new (_place+i) T(t);
        } 
    }
    // 初始化列表

    Vector(const Vector& _t) {
        _size = _t.size();
        _capacity = _size * 2;
        _place = static_cast<T*>(malloc(_t.size() * sizeof(T) * 2));
        std::cout << "using the copy" << std::endl;
        for (int i = 0; i < _t.size(); i++) {
            new (_place+i) T(_t[i]);
        }
    }

    // 移动的真谛在于资源的转移，不会再进行多分配
    // 这样写实际上进行了额外的分配，在新的地方又调用了构造函数，这实际上是一种深拷贝
    // 对move的理解还是不到位
    Vector(const Vector&& _t) {
        // _size = _t.size();
        // _capacity = _t.size() * 2;
        // _place = static_cast<T*>(malloc(_t.size() * sizeof(T) * 2));
        // std::cout << "using the move" << std::endl;
        // for (int i = 0; i < _t.size(); i++) {
        //     new (_place+i) T(std::move(_t[i]));
        // }
    }



    T& operator[](size_t i) {
        return _place[i];
    }

    T& operator[] (size_t i) const {
        return _place[i];
    }

    const size_t size() const {
        return _size;
    }

    const size_t capacity() const {
        return _capacity;
    }


    //
    T& front() {
        return _place[0];
    }

    T& front() const {
        return _place[0];
    }

    T& back() {
        return _place[_size-1];
    }

    T& back() const {
        return _place[_size-1];
    }

    T* data() {
        return _place;
    }

    T* begin() {
        return &_place[0];
    }

    T* end() {
        return _place + _size;
    }


    void reserve(size_t size) {
        if (size <= _capacity) {
            return ;
        }
        _capacity = size;
        // 扩容然后复制过去
        // 深拷贝
        auto _n_place = static_cast<T*>(malloc(size * sizeof(T)));
        for (int i = 0; i < _size; i++) {
            new (_n_place+i) T(std::move(_place[i]));
            _place[i].~T();
        }

        _place = nullptr;
        _place = _n_place;
        return ;        
    }

    void shrink_to_fit() {
        // 类似reserve的方式，直接开辟新空间拷贝过去，这就是为啥尽量避免扩缩容操作，因为都消耗时间
    }


    void push_back(const T& m) {
        // if (_size >= _capacity) {

        //     _capacity *= 2;
        //     auto _n_place = static_cast<T*>(malloc(capacity * sizeof(T)));
        //     for (int i = 0; i <_size; i++) {
        //         new (_n_place + i) T(_place[i]);
        //         _place[i].~T();
        //     }
        //     _place = _n_place;
        // }
        reserve(_size+1);
        new (_place+_size) T(m);
        _size++;
    }

    template<class... Args>
    void emplace_back(Args... args) {
        reserve(_size+1);
        new (_place + _size) T(std::forward<Args>(args)...);
    }

    void pop_back()
	{
        _size--;
		_place[_size].~T();
    }

    T* erase(T *_p)
    {
        auto _nptr = _p;
        for (; _p < _place + _size; _p++)
        {
			*_p = std::move(*(_p + 1));
        }
		_size--;
		_place[_size].~T();
		return _nptr;    
    }

	~Vector()
    {
	    // 析构函数自定义去处理了指针类型的话，一定要再多写拷贝构造/拷贝运算符
        // 否则就是浅拷贝，析构两个对象会造成double free

    }
    

private:
    T *_place = nullptr;

    size_t _capacity = 0;
    size_t _size = 0;



};












#endif