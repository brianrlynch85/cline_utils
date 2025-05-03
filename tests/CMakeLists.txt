# ------------------------------------------------------------------------
#
#                            CMakeLists.txt for cline_utils
#                                        V 0.01
#
#                            (c) Brian Lynch February, 2015
#
# ------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.2)

add_executable(ctest_optlonger_config test_optlonger_config.cpp)
target_link_libraries(ctest_optlonger_config bprinter Catch2::Catch2WithMain)
add_test(ctest_optlonger_config ${EXECUTABLE_OUTPUT_PATH}/ctest_optlonger_config -b 4 --longName1=5 -c 3 -d 'hello.txt')

add_executable(ctest_optlonger_missing_arg test_optlonger_missing_arg.cpp)
target_link_libraries(ctest_optlonger_missing_arg bprinter Catch2::Catch2WithMain)
add_test(ctest_optlonger_missing_arg ${EXECUTABLE_OUTPUT_PATH}/ctest_optlonger_missing_arg -b 4 --longName1=5 -c 3 -d)

add_executable(ctest_optlonger_missing_opt test_optlonger_missing_opt.cpp)
target_link_libraries(ctest_optlonger_missing_opt bprinter Catch2::Catch2WithMain)
add_test(ctest_optlonger_missing_opt ${EXECUTABLE_OUTPUT_PATH}/ctest_optlonger_missing_opt -b 4 --longName1=5 -c 3)

add_executable(ctest_optlonger_unrecognized_opt test_optlonger_unrecognized_opt.cpp)
target_link_libraries(ctest_optlonger_unrecognized_opt bprinter Catch2::Catch2WithMain)
add_test(ctest_optlonger_unrecognized_opt ${EXECUTABLE_OUTPUT_PATH}/ctest_optlonger_unrecognized_opt -b 4 --longName1=5 -c 3 -d 'hello.txt' -q)

add_executable(ctest_optlonger_duplicate test_optlonger_duplicate.cpp)
target_link_libraries(ctest_optlonger_duplicate bprinter Catch2::Catch2WithMain)
add_test(ctest_optlonger_duplicate ${EXECUTABLE_OUTPUT_PATH}/ctest_optlonger_duplicate -b 4 --longName1=5 -c 3 -d 'hello.txt')