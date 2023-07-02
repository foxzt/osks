cmake_minimum_required(VERSION 3.22)
project(osks)

set(CMAKE_CXX_STANDARD 17)

set(MY_LIB_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
file(MAKE_DIRECTORY ${MY_LIB_OUTPUT_DIRECTORY})
set(LIBRARY_OUTPUT_PATH ${MY_LIB_OUTPUT_DIRECTORY})

set(MY_BIN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${MY_BIN_OUTPUT_DIRECTORY})
set(EXECUTABLE_OUTPUT_PATH ${MY_BIN_OUTPUT_DIRECTORY})

set(LIB_SOURCES
        src/lib/get_num.c
        src/lib/error_functions.c
        )
add_library(mylib STATIC ${LIB_SOURCES})


add_executable(test test/main.cpp)
add_dependencies(test mylib)
target_link_libraries(test mylib)

add_executable(svshm_sender src/svshm_sender.c src/binary_sems.c)
add_dependencies(svshm_sender mylib)
target_link_libraries(svshm_sender mylib)

add_executable(svshm_receiver src/svshm_receiver.c src/binary_sems.c)
add_dependencies(svshm_receiver mylib)
target_link_libraries(svshm_receiver mylib)

add_executable(svmsg_sender src/svmsg_sender.c)
add_dependencies(svmsg_sender mylib)
target_link_libraries(svmsg_sender mylib)

add_executable(svmsg_receiver src/svmsg_receiver.c)
add_dependencies(svmsg_receiver mylib)
target_link_libraries(svmsg_receiver mylib)
