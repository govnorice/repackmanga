cmake_minimum_required(VERSION 3.28)
project(repackmanga)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS filesystem)


if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(repackmanga main.cpp
            modules/repack.h
            modules/repack.cpp
            modules/miniz.h
            modules/miniz.c
    )
    target_link_libraries(repackmanga ${Boost_LIBRARIES})
endif ()