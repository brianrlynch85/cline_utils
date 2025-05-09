# ------------------------------------------------------------------------
#
#                         CMakeLists.txt for cline_utils
#                                        V 0.01
#
#                            (c) Brian Lynch February, 2015
#
# ------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.2)

project(
   cline_utils
   VERSION 1.0.1
   LANGUAGES CXX
       )

enable_testing()

Include(FetchContent)
include(CMakePrintHelpers)

cmake_path(GET PROJECT_SOURCE_DIR PARENT_PATH PARENT_DIR)
#cmake_print_variables(PROJECT_SOURCE_DIR)
#cmake_print_variables(PARENT_DIR)

# Grab the bprinter library
set(FETCHCONTENT_BASE_DIR ${PARENT_DIR}/bprinter)
FetchContent_Declare(
    bprinter
    GIT_REPOSITORY https://github.com/brianrlynch85/bprinter.git
    GIT_TAG        v1.0.2 # or a later release
)
FetchContent_MakeAvailable(bprinter)

# Grab the Catch2 test framework
set(FETCHCONTENT_BASE_DIR ${PARENT_DIR}/catch2)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/brianrlynch85/Catch2
  GIT_TAG        v3.8.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)

#add_executable(tests test_parsing.cpp)
#target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

#cmake_print_variables(bprinter_SOURCE_DIR)
include_directories(${PROJECT_SOURCE_DIR}/include ${bprinter_SOURCE_DIR}/include)
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "include directory='${dir}'")
#endforeach()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
#cmake_print_variables(EXECUTABLE_OUTPUT_PATH)

add_subdirectory(source)
add_subdirectory(tests)