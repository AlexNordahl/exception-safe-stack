cmake_minimum_required(VERSION 3.20)
project(cpu-matrix)

option(BUILD_TESTS "Build tests" ON)

if (BUILD_TESTS)
    include(CTest)
    enable_testing()
    include(FetchContent)

    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG v1.15.2
    )
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(${PROJECT_NAME}_tests
        tests/stack_tests.cpp
    )
    target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_tests)
endif()

add_executable(main
    src/main.cpp
)

target_compile_features(main PRIVATE cxx_std_17)
target_compile_options(main PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion -Wfloat-conversion -pedantic-errors)