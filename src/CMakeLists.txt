find_package(GTest REQUIRED)
include_directories(/usr/local/include)

add_library(
    gbemu_core
    bus.cc
    types.cc
    memory/mbc.cc
    memory/rom.cc
    cpu/cpu.cc
    cpu/register.cc
    cpu/opcode.cc
    cpu/alu.cc
)
target_compile_options(
    gbemu_core PUBLIC
    -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic
    -Wno-padded -Wno-poison-system-directories
    -g
    --coverage
)
target_link_options(
    gbemu_core PUBLIC
    --coverage
)
target_include_directories(
    gbemu_core PUBLIC
    .
)


add_executable(
    gbemu
    main.cc
)
target_link_libraries(gbemu gbemu_core)
target_compile_options(
    gbemu PRIVATE
)

add_executable(
    gbemu_test
    types_test.cc
    cpu/register_test.cc
    cpu/opcode_test.cc
    cpu/alu_test.cc
)
target_link_libraries(gbemu_test gbemu_core GTest::GTest GTest::Main)
target_compile_options(
    gbemu_test PRIVATE
    -Wno-global-constructors
    -g
)
target_link_options(
    gbemu_test PRIVATE
)
add_dependencies(gbemu_test clean_gcda)
gtest_discover_tests(gbemu_test)

add_custom_target(
    clean_gcda
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND find . -name "*.gcda" -print0 | xargs -0 rm
)

add_custom_target(
  coverage
  DEPENDS gbemu_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND lcov --rc lcov_branch_coverage=1 -c -i -d src/CMakeFiles/gbemu_core.dir/ -o coverage_base.info > lcov.log 2>&1
  COMMAND ctest
  COMMAND lcov --rc lcov_branch_coverage=1 -d src/CMakeFiles -c -o coverage_test.info > lcov.log 2>&1
  COMMAND lcov --rc lcov_branch_coverage=1 -a coverage_base.info -a coverage_test.info -o coverage_all.info > lcov.log 2>&1
  COMMAND lcov --rc lcov_branch_coverage=1 -r coverage_all.info *_test.cc *gtest* */usr/include/* -o coverage.info >> lcov.log 2>&1
  COMMAND genhtml -o lcovHtml --num-spaces 4 -s --legend --branch-coverage coverage.info >> lcov.log 2>&1
  COMMENT [[create coverage report as lcovHtml/index.html, and log as lcov.log]]
)
