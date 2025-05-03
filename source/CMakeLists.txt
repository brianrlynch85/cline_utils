# ------------------------------------------------------------------------
#
#                         CMakeLists.txt for cline_utils
#                                        V 0.01
#
#                            (c) Brian Lynch February, 2015
#
# ------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.2)

add_executable(example_main example_main.cpp)
target_link_libraries(example_main bprinter)