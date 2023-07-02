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


add_executable(main test/main.cpp)
add_dependencies(main mylib)
target_link_libraries(main mylib)

add_executable(sender src/svshm_sender.c src/binary_sems.c)
add_dependencies(sender mylib)
target_link_libraries(sender mylib)

add_executable(receiver src/svshm_receiver.c src/binary_sems.c)
add_dependencies(receiver mylib)
target_link_libraries(receiver mylib)

add_executable(fser src/svmsg_sender.c)
add_dependencies(fser mylib)
target_link_libraries(fser mylib)

add_executable(fcli src/svmsg_receiver.c)
add_dependencies(fcli mylib)
target_link_libraries(fcli mylib)
