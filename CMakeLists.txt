cmake_minimum_required(VERSION 4.3)
project(LMP VERSION 0.0.1 LANGUAGES C)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/ProjectBasicConfig.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/CompilerOptions.cmake)
add_compile_options("-fno-strict-aliasing")

find_package(extstd REQUIRED)
include_directories(${extstd_INCLUDE_DIRS})

file(GLOB_RECURSE LMP_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.c src/*.h)
add_library(LMP SHARED ${LMP_SRC})
target_link_libraries(LMP PRIVATE extstd::extstd)
target_compile_definitions(LMP PRIVATE "_SHARED_LIB_" $<$<BOOL:${ENABLE_TEST_COV}>:_TEST_LOCAL_API_>)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/InstallCLibrary.cmake)
setup_c_library_install(${PROJECT_NAME})

if(ENABLE_TEST_COV)
    enable_testing()
endif()
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/SetupCTesting.cmake)
setup_c_testing(${PROJECT_NAME} PREFIX ${TARGET_PREFIX} DEPENDENCIES extstd::extstd)
