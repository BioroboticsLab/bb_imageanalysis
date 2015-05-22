file(GLOB_RECURSE src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE hdr RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
file(GLOB_RECURSE pipeline-hdr RELATIVE ${CMAKE_SOURCE_DIR} *.h)

add_definitions(-DTEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data")

add_executable(run-tests
    ${src} ${hdr} ${pipeline-hdr}
)
 
target_link_libraries(run-tests
    pipeline
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    ${MPI_CXX_LIBRARIES}
)
