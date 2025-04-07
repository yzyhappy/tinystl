#include "../stl/unique_ptr.h"
#include <gtest/gtest.h>
#include <vector>

#include "src/gtest-internal-inl.h"


class Animal
{
public:
    virtual void speak() = 0;
};

class Dog : public Animal
{
public:
	void speak() override
	{
		printf("wang wang\n");
	}
};

class Cat : public Animal
{
public:
	void speak() override
	{
		printf("miao miao\n");
	}
};


TEST(TINYSTL_TEST, uniqueptr)
{
    auto arr = Unique_ptr<int[]>(new int[10]);
	std::vector<Unique_ptr<Animal>> zoo;
	// 需要自己实现强制转换
	zoo.push_back(Make_unique<Cat>());
	zoo.push_back(Make_unique<Dog>());
	for (const auto& animal : zoo)
	{
		animal->speak();
	}
}
