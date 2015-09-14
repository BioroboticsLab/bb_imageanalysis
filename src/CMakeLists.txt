file(GLOB src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB hdr RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)

file(GLOB src-util RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} utility/*.cpp)
file(GLOB hdr-util RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} utility/*.h)

add_definitions(-DUSE_DEEPLOCALIZER)

add_subdirectory(pipeline)

link_directories(${DEEPLOCALIZER_CLASSIFIER_LIBRARY_DIR})

add_executable(bb_imganalysis
    ${src} ${hdr}
    ${src-util} ${hdr-util}
)

get_target_property(PIPELINE_INCLUDES Pipeline PIPELINE_INCLUDES)

include_directories(${PIPELINE_INCLUDES})

message(${DEEPLOCALIZER_CLASSIFIER_LIBRARY_DIR})

target_link_libraries(bb_imganalysis
    "deeplocalizer-classifier"
    Pipeline
    ${CMAKE_THREAD_LIBS_INIT}
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    ${MPI_CXX_LIBRARIES}
    ${Caffe_LIBRARIES}
    ${LMDB_LIBRARIES}
    ${CUDA_LIBRARIES}
)

# add files to source groups to preserve folder structure in visual studio
# adapted from: http://goo.gl/Xde41i

set(ALL_FILES ${src} ${hdr})

foreach(FILE ${ALL_FILES}) 
  get_filename_component(PARENT_DIR "${FILE}" PATH)

  # skip src or include and changes /'s to \\'s
  string(REGEX REPLACE "(\\./)?(src|include)/?" "" GROUP "${PARENT_DIR}")
  string(REPLACE "/" "\\" GROUP "${GROUP}")

  # group into categories
  if ("${FILE}" MATCHES ".*\\.cpp")
    set(GROUP "Source Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.h")
    set(GROUP "Header Files\\${GROUP}")
  endif()

  source_group("${GROUP}" FILES "${FILE}")
endforeach()

