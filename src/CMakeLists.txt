find_package(GTest REQUIRED)
include_directories(/usr/local/include)

add_library(
    gbemu_lib
    mbc.cc
    rom.cc
    cpu.cc
    bus.cc
    register.cc
    types.cc
)
target_compile_options(
    gbemu_lib PUBLIC
    -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic
    -Wno-padded -Wno-poison-system-directories
    -g
    --coverage
)
target_link_options(
    gbemu_lib PUBLIC
    -g
    --coverage
)

add_executable(
    gbemu
    main.cc
)
target_link_libraries(gbemu gbemu_lib)
target_compile_options(
    gbemu PRIVATE
)

add_executable(
    gbemu_test
    register_test.cc
)
target_link_libraries(gbemu_test gbemu_lib GTest::GTest GTest::Main)
target_compile_options(
    gbemu_test PRIVATE
    -Wno-global-constructors
)
target_link_options(
    gbemu_test PRIVATE
)
gtest_discover_tests(gbemu_test)

add_custom_target(
  coverage
  DEPENDS gbemu_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND ctest
  COMMAND lcov --directory src/CMakeFiles/gbemu_lib.dir/ --directory src/CMakeFiles/gbemu_test.dir/ -c -o coverage.info > lcov.log 2>&1
  COMMAND lcov -e coverage.info ${PROJECT_SOURCE_DIR}/src/* -o filtered_coverage.info >> lcov.log 2>&1
  COMMAND lcov -r filtered_coverage.info ${PROJECT_SOURCE_DIR}/src/*_test.cc -o filtered_coverage.info >> lcov.log 2>&1
  COMMAND genhtml -o lcovHtml --num-spaces 4 -s --legend filtered_coverage.info >> lcov.log 2>&1
  COMMENT [[create coverage report as lcovHtml/index.html, and log as lcov.log]]
)
