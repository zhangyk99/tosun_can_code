cmake_minimum_required(VERSION 3.14)
project(TosunCanCode)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    include_directories("lib/windows")
    link_directories("lib/windows/x86")
else()
    include_directories("lib/linux")
    link_directories("lib/linux")
endif()

add_subdirectory(tests)

add_executable(test test.cpp)

if(WIN32)
    target_link_libraries(test libTSCAN)
else()
    target_link_libraries(test TSCANApiOnLinux TSH dl)
endif()

