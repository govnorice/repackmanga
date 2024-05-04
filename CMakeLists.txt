cmake_minimum_required(VERSION 3.28)
project(repackmanga)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk+-3.0)

include_directories(${GTK_INCLUDE_DIRS})

link_libraries(${GTK_LIBRARIES})

# CSS
set(STYLE_HEX_FILE "${CMAKE_CURRENT_SOURCE_DIR}/style.hex")

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(repackmanga main.cpp
            modules/Repack.h
            modules/Repack.cpp
            modules/Miniz.h
            modules/Miniz.c
            modules/CssManager.cpp
            modules/CssManager.h
            widgets/ChooserDialog.cpp
            widgets/ChooserDialog.h
            "${STYLE_HEX_FILE}"
    )
    target_link_libraries(repackmanga ${Boost_LIBRARIES})
endif ()