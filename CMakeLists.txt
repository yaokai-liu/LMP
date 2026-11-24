cmake_minimum_required(VERSION 4.3)
project(LMP C)

set(CMAKE_C_STANDARD 23)
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)
endif ()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)
endif ()
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)
endif ()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(TARGET_PREFIX "")
else ()
    set(TARGET_PREFIX "LMP_")
endif ()

#add_compile_options("-O3" "-Os")
add_compile_options("-flto" "-fno-strict-aliasing")
add_compile_options("-Wall" "-Wextra" "-Werror")

if (ENABLE_TEST_COV)
    add_compile_options("-fprofile-arcs" "-ftest-coverage" "--coverage")
    add_link_options("-fprofile-arcs" "-ftest-coverage" "--coverage")
endif ()
if (ENABLE_GPROF)
    add_compile_options("-pg")
    add_link_options("-pg")
endif ()

add_subdirectory(extstd)
include_directories(extstd/include)
link_libraries(extstd)

include_directories(include/LMP)

file(GLOB_RECURSE LMP_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.c src/*.h)
add_library(LMP SHARED ${LMP_SRC})
target_link_libraries(LMP PRIVATE m)
target_compile_definitions(LMP PRIVATE "_SHARED_LIB_" $<$<BOOL:${BUILD_TESTING}>:_TEST_LOCAL_API_>)

if (BUILD_TESTING)
    enable_testing()
    set(TARGET_PREFIX "LMP_")
    target_link_libraries(LMP PRIVATE gcov)
    target_compile_options(LMP PRIVATE "--coverage")
    target_link_options(LMP PRIVATE "--coverage")
    AUX_SOURCE_DIRECTORY(test/testcases ${TARGET_PREFIX}TESTCASE_SRC)
    file(GLOB_RECURSE ${TARGET_PREFIX}TESTCASE_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/testcases/*.c test/testcases/*.h)
    add_executable(${TARGET_PREFIX}test test/test.c ${${TARGET_PREFIX}TESTCASE_SRC})
    target_compile_definitions(${TARGET_PREFIX}test PRIVATE "_SHARED_LIB_" $<$<BOOL:${BUILD_TESTING}>:_TEST_LOCAL_API_>)
    target_include_directories(${TARGET_PREFIX}test PRIVATE test/testcases)
    target_link_libraries(${TARGET_PREFIX}test PRIVATE check gcov LMP)
    if (GITHUB_CI)
        target_link_libraries(${TARGET_PREFIX}test PRIVATE subunit)
    endif ()
    target_compile_options(${TARGET_PREFIX}test PRIVATE "--coverage")
    target_link_options(${TARGET_PREFIX}test PRIVATE "--coverage")
    add_test(NAME MyUnitTests COMMAND ${TARGET_PREFIX}test)

    find_program(LCOV_PATH lcov)
    find_program(GENHTML_PATH genhtml)
    if(LCOV_PATH AND GENHTML_PATH)
        add_custom_target(coverage
            COMMAND ${LCOV_PATH} --directory . --zerocounters
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${LCOV_PATH} --directory . --capture --output-file coverage.info
            COMMAND ${LCOV_PATH} --remove coverage.info test/* /usr/* --ignore-errors unused --output-file coverage_filtered.info
            COMMAND ${GENHTML_PATH} coverage_filtered.info --output-directory coverage_html
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating coverage report with lcov..."
            VERBATIM
        )
        add_dependencies(coverage ${TARGET_PREFIX}test)
    else()
        message(WARNING "lcov / genhtml not found, coverage target disabled")
    endif()
endif ()

add_executable(${TARGET_PREFIX}debug test/debug.c)
target_link_libraries(${TARGET_PREFIX}debug PRIVATE LMP)
target_compile_definitions(${TARGET_PREFIX}debug PRIVATE "_TEST_LOCAL_API_")