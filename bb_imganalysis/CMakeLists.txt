file(GLOB src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB hdr RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)

file(GLOB src-util RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} utility/*.cpp)
file(GLOB hdr-util RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} utility/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${CPM_BIN_TARGET_NAME}
    ${src} ${hdr}
    ${src-util} ${hdr-util}
)

target_link_libraries(${CPM_BIN_TARGET_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBS}
    ${MPI_CXX_LIBRARIES}
    ${CPM_LIBRARIES}
)

# add files to source groups to preserve folder structure in visual studio
# adapted from: http://goo.gl/Xde41i

set(ALL_FILES ${src} ${hdr} ${src-util} ${hdr-util})

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

