include(FetchContent)

if (NOT EMSCRIPTEN)
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
endif()

add_library(
    gbeml_core SHARED
    gameboy.cc
    types/types.cc
    bus/bus.cc
    bus/bus_impl.cc
    interrupt/interrupt_controller.cc
    interrupt/interrupt_controller_impl.cc
    register/register.cc
    memory/mbc.cc
    memory/ram.cc
    memory/ram_impl.cc
    memory/rom.cc
    cpu/alu.cc
    cpu/cpu.cc
    cpu/opcode.cc
    graphics/fetcher.cc
    graphics/lcdc.cc
    graphics/lcd_stat.cc
    graphics/palette.cc
    graphics/pixel.cc
    graphics/ppu.cc
    graphics/ppu_impl.cc
    graphics/sprite.cc
    graphics/tile.cc
    display/display.cc
    display/display_impl.cc
    timer/timer.cc
    timer/timer_impl.cc
    joypad/joypad.cc
    joypad/joypad_impl.cc
)
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(
        gbeml_core PRIVATE
        # -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
        -Werror -Wall -Wextra
        $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>
        $<$<CONFIG:DEBUG>:--coverage>
    )
    target_link_options(
        gbeml_core PRIVATE
        $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>
        $<$<CONFIG:DEBUG>:--coverage>
    )
else()
    target_compile_options(
        gbeml_core PRIVATE
        -Werror -Wall -Wextra -Wno-return-type
        $<$<CONFIG:DEBUG>:--coverage>
    )
    target_link_options(
        gbeml_core PRIVATE
        $<$<CONFIG:DEBUG>:--coverage>
    )
endif()

if (EMSCRIPTEN)
    target_include_directories(
        gbeml_core PUBLIC
        ${CMAKE_SOURCE_DIR}/src
    )
else()
    target_link_libraries(
        gbeml_core
        glog
    )
    target_include_directories(
        gbeml_core PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/third_party/debugbreak
    )
endif()

add_executable(
    gbeml_test EXCLUDE_FROM_ALL
    bus/bus_impl_test.cc
    types/types_test.cc
    register/register_test.cc
    cpu/alu_test.cc
    cpu/cpu_test.cc
    graphics/lcdc_test.cc
    graphics/lcd_stat_test.cc
    graphics/palette_test.cc
    graphics/ppu_impl_test.cc
    graphics/sprite_test.cc
    graphics/tile_test.cc
    joypad/joypad_impl_test.cc
    timer/timer_impl_test.cc
    interrupt/interrupt_controller_impl_test.cc
)
target_link_libraries(
    gbeml_test
    gbeml_core
    gtest_main
    gmock_main
)
target_compile_options(
    gbeml_test PRIVATE
)
target_link_options(
    gbeml_test PRIVATE
)
add_dependencies(
    gbeml_test
    clean_gcda
)

include(GoogleTest)
gtest_discover_tests(
    gbeml_test
    PROPERTIES LABELS gbeml
)

add_custom_target(
    clean_gcda
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND find . -name "*.gcda" -depth -delete
)

add_custom_target(
  coverage
  DEPENDS gbeml_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND lcov --rc lcov_branch_coverage=1 -c -i -d src/core/CMakeFiles -o coverage_base.info > lcov.log 2>&1
  COMMAND ctest -L gbeml -j 8
  COMMAND lcov --rc lcov_branch_coverage=1 -d src/core/CMakeFiles -c -o coverage_test.info > lcov.log 2>&1
  COMMAND lcov --rc lcov_branch_coverage=1 -a coverage_base.info -a coverage_test.info -o coverage_all.info > lcov.log 2>&1
  COMMAND lcov --rc lcov_branch_coverage=1 -r coverage_all.info *_test.cc *gtest* */usr/include/c++/v1/* *glog* *debugbreak* -o coverage.info >> lcov.log 2>&1
  COMMAND genhtml -o lcovHtml --num-spaces 4 -s --legend --branch-coverage coverage.info >> lcov.log 2>&1
  COMMENT [[create coverage report as lcovHtml/index.html, and log as lcov.log]]
)
