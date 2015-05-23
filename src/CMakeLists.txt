file(GLOB_RECURSE pipeline-src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE pipeline-hdr RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
file(GLOB src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB hdr RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)

list(REMOVE_ITEM pipeline-src ${src})
list(REMOVE_ITEM pipeline-hdr ${hdr})

add_library(pipeline STATIC
    ${pipeline-src} ${pipeline-hdr}
)

add_executable(bb_imganalysis
    ${src} ${hdr}
)
 
target_link_libraries(bb_imganalysis
    pipeline
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    ${MPI_CXX_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
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

