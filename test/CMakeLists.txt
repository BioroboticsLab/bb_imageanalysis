file(GLOB_RECURSE src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE hdr RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)

include_directories("${CMAKE_SOURCE_DIR}/src/pipeline")

add_definitions(-DTEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data")

add_executable(run-tests
    ${src} ${hdr}
)
 
target_link_libraries(run-tests
    pipeline
)
