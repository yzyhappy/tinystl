#ifndef _ARRAY_H
#define _ARRAY_H

template<class T, int N>
class Array {
public:
	// ֧���±�
	T& operator[] (int i) {
		return _array[i];
	}
	
private:
	T _array[N];

};







#endif