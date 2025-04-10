#include <gtest/gtest.h>
#include <iostream>
#include "../stl/vector.h"

TEST(TINY_STL, vector) {

    auto v = Vector(3, 1);
    for (int i = 0; i < 3; i++) {
        std::cout << v[i] << " ";
    }

    std::cout << std::endl;
    class test1 {
        public:
        test1(int i) : t(i) {}
        // test1(const test1 &t) = default;
        // test1(const test1 &&t) = delete;
        // test1& operator=(const test1&& _t) {
        //     this->t = _t.t;
        //     return *this;
        // }
        int p() {
            return t;
        }
        private:
        int t;
    };

    auto vv = Vector(3, test1(3));
    for (int i = 0; i < 3; i++) {
        std::cout << vv[i].p() << std::endl;
    }
    std::cout << std::endl;
    std::cout << vv.size() << " " << vv.capacity() << std::endl;
    Vector<test1> vvvv = vv;
    Vector<test1> vvv = std::move(vv);

    vv[0] = test1(1);
    vv[1] = test1(2);

    auto b = vv.begin();
	std::cout << b->p() << std::endl;
    auto n_b = vv.erase(b);
    std::cout << "erase b: " << n_b->p() << std::endl;

}