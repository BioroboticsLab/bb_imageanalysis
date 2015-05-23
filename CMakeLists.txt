cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0012 NEW)

project(bb_imganalysis)

# project dependecies

find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS filesystem serialization system log log_setup thread date_time REQUIRED)
find_package(MPI REQUIRED)
find_package(Threads REQUIRED)

# remove all previously set compiler and linker flags
set(CMAKE_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using Clang or GCC
  # minimal optimization and debug symbols for debug builds
  set(CMAKE_CXX_FLAGS_DEBUG "-O1 -ggdb")
  # enable optimization for release builds
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DQT_NO_DEBUG")
  # enable C++1y
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
  # enable all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Woverloaded-virtual -Wold-style-cast -Wnon-virtual-dtor -Wsign-promo -Wno-missing-braces")
  # warnings are errors
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wno-error=unused-variable -Wno-error=unused-parameter")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  # Force to always compile with W4 and treat warnings as errors
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W3" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
  endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-private-field")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-but-set-variable -Wno-missing-field-initializers")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DNDEBUG -march=native -pipe -fPIC -std=c++1y -ffast-math -funroll-loops -fpeel-loops")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
  endif()

  if ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Ofast -ggdb -DNDEBUG -march=native -pipe -fPIC -std=c++1y -ffast-math -funroll-loops -fpeel-loops")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND
   "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND NOT(${UNIX}))
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-optimize-sibling-calls")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=integer")
endif()

option(ENABLE_LTO "ENABLE_LTO" ON)
if(${ENABLE_LTO})
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -flto")
endif()

set(Boost_USE_MULTITHREADED OFF)

include_directories(
	${PROJECT_SOURCE_DIR}
	SYSTEM ${OpenCV_INCLUDE_DIRS}
	SYSTEM ${Boost_INCLUDE_DIR}
	SYSTEM ${MPI_CXX_INCLUDE_PATH}
)

add_definitions(-DPipelineStandalone)
#add_definitions(-DDEBUG_SHOW_DECODED_GRID)
add_definitions(-DBOOST_LOG_DYN_LINK)

add_subdirectory(src)
add_subdirectory(test)
