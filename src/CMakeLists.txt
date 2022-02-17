find_package(GTest REQUIRED)
include_directories(/usr/local/include)

add_library(
    gbemu_core
    mbc.cc
    rom.cc
    cpu.cc
    bus.cc
    register.cc
    types.cc
    opcode.cc
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
    -g
    --coverage
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
    register_test.cc
    types_test.cc
    opcode_test.cc
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
gtest_discover_tests(gbemu_test)

add_custom_target(
  coverage
  DEPENDS gbemu_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND lcov --rc lcov_branch_coverage=1 -c -i -d src/CMakeFiles/gbemu_core.dir/ -o coverage_base.info
  COMMAND ctest
  COMMAND lcov --rc lcov_branch_coverage=1 --directory src/CMakeFiles/gbemu_core.dir/ --directory src/CMakeFiles/gbemu_test.dir/ -c -o coverage_all.info > lcov.log 2>&1
  COMMAND lcov --rc lcov_branch_coverage=1 -e coverage_all.info ${PROJECT_SOURCE_DIR}/src/* -o coverage_src.info >> lcov.log 2>&1
  COMMAND lcov --rc lcov_branch_coverage=1 -r coverage_src.info ${PROJECT_SOURCE_DIR}/src/*_test.cc -o coverage_test.info >> lcov.log 2>&1
  COMMAND lcov --rc lcov_branch_coverage=1 -a coverage_base.info -a coverage_test.info -o coverage.info
  COMMAND genhtml -o lcovHtml --num-spaces 4 -s --legend --branch-coverage coverage.info >> lcov.log 2>&1
  COMMENT [[create coverage report as lcovHtml/index.html, and log as lcov.log]]
)
