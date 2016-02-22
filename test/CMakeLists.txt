file(GLOB_RECURSE src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE hdr RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)

add_definitions(-DTEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data")

add_executable(run-tests
    ${src} ${hdr}
    "${PROJECT_SOURCE_DIR}/bb_imganalysis/utility/util.h"
    "${PROJECT_SOURCE_DIR}/bb_imganalysis/utility/util.cpp"
)
 
target_link_libraries(run-tests
    ${CPM_LIBRARIES}
)
