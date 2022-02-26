include(FetchContent)

FetchContent_Declare(googletest
  QUIET
  URL https://github.com/google/googletest/archive/release-1.11.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(glog
  QUIET
  URL https://github.com/google/glog/archive/v0.5.0.tar.gz
)
FetchContent_MakeAvailable(glog)

add_library(
    gbemu_core SHARED
    gameboy.cc
    types/types.cc
    bus/bus.cc
    bus/bus_impl.cc
    interrupt/interrupt_controller.cc
    register/register.cc
    memory/mbc.cc
    memory/ram.cc
    memory/ram_impl.cc
    memory/rom.cc
    cpu/alu.cc
    cpu/cpu.cc
    cpu/opcode.cc
    graphics/ppu.cc
    graphics/dot.cc
    display/display.cc
    display/minifb_display.cc
)
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(
        gbemu_core PRIVATE
        # -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
        -Werror -Wall -Wextra
        --coverage
    )
else()
    target_compile_options(
        gbemu_core PRIVATE
        -Werror -Wall -Wextra -Wno-return-type
        --coverage
    )
endif()
target_link_libraries(
    gbemu_core
    glog
)
target_link_options(gbemu_core PRIVATE --coverage)
target_include_directories(gbemu_core PUBLIC .)

add_executable(
    gbemu
    main.cc
)
target_link_libraries(gbemu gbemu_core minifb)
target_compile_options(gbemu PRIVATE)

add_executable(
    gbemu_test
    types/types_test.cc
    register/register_test.cc
    cpu/alu_test.cc
    cpu/cpu_test.cc
    cpu/opcode_test.cc
    graphics/ppu_test.cc
)
target_link_libraries(
    gbemu_test
    gbemu_core
    gtest_main
    gmock_main
)
target_compile_options(gbemu_test PRIVATE)
add_dependencies(gbemu_test clean_gcda)

include(GoogleTest)
gtest_discover_tests(
    gbemu_test
    PROPERTIES LABELS gbemu
)

add_custom_target(
    clean_gcda
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND find . -name "*.gcda" -depth -delete
)

add_custom_target(
  coverage
  DEPENDS gbemu_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND lcov --rc lcov_branch_coverage=1 -c -i -d src/CMakeFiles -o coverage_base.info > lcov.log 2>&1
  COMMAND ctest -L gbemu
  COMMAND lcov --rc lcov_branch_coverage=1 -d src/CMakeFiles -c -o coverage_test.info > lcov.log 2>&1
  COMMAND lcov --rc lcov_branch_coverage=1 -a coverage_base.info -a coverage_test.info -o coverage_all.info > lcov.log 2>&1
  COMMAND lcov --rc lcov_branch_coverage=1 -r coverage_all.info *_test.cc *gtest* */usr/include/c++/v1/* -o coverage.info >> lcov.log 2>&1
  COMMAND genhtml -o lcovHtml --num-spaces 4 -s --legend --branch-coverage coverage.info >> lcov.log 2>&1
  COMMENT [[create coverage report as lcovHtml/index.html, and log as lcov.log]]
)
