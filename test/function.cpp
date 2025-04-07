#include "../stl/function.h"
#include <gtest/gtest.h>


TEST(TINYSTL_TEST, Function)
{
	int a = 1;
	Function<void(int)> tmp = [&](int t)
		{
			a = t;
		};

	tmp(10);
	EXPECT_EQ(a, 10);

}