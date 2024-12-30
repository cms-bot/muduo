set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/log LOG)
aux_source_directory(. SRC_LIST)
add_executable(test ${SRC_LIST} ${LOG})