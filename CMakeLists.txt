cmake_minimum_required(VERSION 2.6)

cmake_policy (SET CMP0020 NEW)

project(bb_imganalysis)

# project dependecies
find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS filesystem serialization system REQUIRED) 

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# using Clang or GCC
	# minimal optimization and debug symbols for debug builds
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
	# enable optimization and warnings-are-errors for release builds
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DQT_NO_DEBUG -Werror -pedantic -Woverloaded-virtual -Wold-style-cast -Wnon-virtual-dtor -Wsign-promo")
	# enable C++1y
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
        # enable all warnings
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# using Visual Studio C++
    # Force to always compile with W3
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W3" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    endif()
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND
    "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=integer")
endif()

include_directories(
	${PROJECT_SOURCE_DIR}
	SYSTEM ${OpenCV_INCLUDE_DIRS}
	SYSTEM ${Boost_INCLUDE_DIR}
)

add_subdirectory(src)
