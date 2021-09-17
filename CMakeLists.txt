cmake_minimum_required(VERSION 3.19)
project(QuadTree)

set(CMAKE_CXX_STANDARD 20)


if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wshadow -W -g")
endif ()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCES
	src/QuadTree.hpp
	src/FreeList.hpp
	)

include_directories(src)

add_library(quadtree STATIC ${SOURCES})
set_target_properties(quadtree PROPERTIES LINKER_LANGUAGE CXX)

add_executable(quadtreetestbin EXCLUDE_FROM_ALL
	tests/Point.cpp
	tests/Point.hpp
	tests/Rect.cpp
	tests/Rect.hpp
	tests/main.cpp
	)

if (WIN32)
	## If you want to link SFML statically
	set(SFML_STATIC_LIBRARIES TRUE)

	## In most cases better set in the CMake cache
	set(SFML_DIR C:/Users/clebi/libraries/SFML-2.5.1/lib/cmake/SFML)
endif()

find_package(SFML 2.5 COMPONENTS graphics system window QUIET)

target_link_libraries(quadtreetestbin PUBLIC sfml-window sfml-graphics sfml-system)


add_executable(unit_tests EXCLUDE_FROM_ALL
	${SOURCES}
	tests/BasicTests.cpp
	tests/Catch2Main.cpp
	tests/UtilsFunctionTests.cpp
	tests/CollisionsTests.cpp
	tests/Point.cpp
	tests/Point.hpp
	tests/Collisions.hpp
	tests/FreeListTests.cpp
	tests/Rect.cpp
	tests/Rect.hpp
	tests/QuadTreeRectTests.cpp
	tests/TestsUtils.hpp
	)

find_package(Catch2 QUIET)
if (NOT Catch2_FOUND)
	set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/lib/catch2)
	find_package(Catch2 REQUIRED)
endif ()
target_link_libraries(unit_tests PRIVATE Catch2::Catch2)