// tinystl.cpp: 定义应用程序的入口点。
//

#include "tinystl.h"
#include <memory>
//#include <cstdio>
//using namespace std;
//
///**
// * C其实是没有函数重载这一说法的，因为在函数编译的过程当中不会对函数名字进行符号修饰
// * 那么C要怎么实现多态？
// *
// * 函数指针
// */
//
//typedef void(*func)(void* args);
//
//void test_nums(void* num)
//{
//	printf("test_num: %d\n", *static_cast<int*>(num));
//}
//
//void test_string(void* string)
//{
//	printf("test_string: hello, world\n");
//}
//
//void multiple_state_single_arg (func f, void* args)
//{
//	f(args);
//}
//
//// 传入多个参数怎么办？
//// struct
//
//
//
//struct Args
//{
//	int a;
//	int b;
//};
//
//
//void test_many_nums(void* args)
//{
//	Args* _args = static_cast<Args*>(args);
//	printf("num1: %d\n", _args->a);
//	printf("num2: %d\n", _args->b);
//}
//
//
//struct Args_functor
//{
//	void operator() () const 
//	{
//		printf("num1: %d\n", _a);
//		printf("num2: %d\n", _b);
//	}
//	int _a;
//	int _b;
//};
//
//template <typename T>
//void test_functor(const T &fn)
//{
//	fn();
//}

class test
{
public:
	test(int num) : t(new int(num)) {}
	void out()
	{
		std::cout << "test unique_ptr" << std::endl;
	}
private:
	int* t;

};

class test2 : test
{
	test2(int num): test(num)
	{
	}
};

int main()
{
	//multiple_state_single_arg(test_nums, &a);
	//multiple_state_single_arg(test_string, nullptr);

	//multiple_state_single_arg(test_many_nums, new Args{ 1, 2 });

	//Args_functor args{ 1, 2 };
	//test_functor(args);

	
	struct deleter
	{
		void operator()(test* t)
		{
			std::cout << "custom deleter..." << std::endl;
			delete t;
		}
	};
	// 只能用这种方式声明，不能使用make_unique，不提供自定义deleter的实现
	// deleter实际上就是一个仿函数
	std::unique_ptr<test, deleter> t(new test(2), deleter());
	t->out();
	return 0;
}
