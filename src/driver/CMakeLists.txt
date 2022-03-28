include(FetchContent)

FetchContent_Declare(SDL
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG "release-2.0.20"
)
FetchContent_MakeAvailable(SDL)

add_library(
    gbeml_driver SHARED
    sdl/sdl_window.cc
)
if (EMSCRIPTEN)
    target_compile_options(
        gbeml_driver PRIVATE
        "SHELL:-s USE_SDL=2"
    )
    target_link_libraries(
        gbeml_driver PRIVATE
        gbeml_core
    )
else()
    target_link_libraries(
        gbeml_driver PRIVATE
        gbeml_core
        SDL2
        glog
    )
endif()

target_include_directories(
    gbeml_driver PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
