  cmake_minimum_required(VERSION 3.8)

  # 设置C++标准版本为C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


   include(FetchContent)
   FetchContent_Declare(
     googletest
     URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
   )

   set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
   FetchContent_MakeAvailable(googletest)

   include(GoogleTest)

   file(GLOB TEST_SOURCES *.cpp)

   # 为每一个test设立单独的测试
   foreach (single_test_source ${TEST_SOURCES})
       get_filename_component(test_name ${single_test_source} NAME_WE)
       add_executable(${test_name} ${single_test_source})
       target_link_libraries(${test_name} PRIVATE gtest gtest_main)
       add_test(NAME ${test_name} COMMAND ${test_name})
   endforeach()