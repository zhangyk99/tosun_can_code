cmake_minimum_required(VERSION 3.14)
project(tests)

find_package(GTest)

add_executable(tests tests.cpp)

if(WIN32)
    target_link_libraries(tests GTest::gtest GTest::gtest_main libTSCAN)
else()
    target_link_libraries(tests GTest::gtest GTest::gtest_main TSCANApiOnLinux TSH dl)
endif()