#ifndef _ARRAY_H
#define _ARRAY_H

template<class T, int N>
class Array {
public:
	// 支持下标
	T& operator[] (int i) {
		return _array[i];
	}
	
private:
	T _array[N];

};







#endif