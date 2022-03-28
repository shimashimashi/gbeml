add_executable(
    index
    main.cc
)
target_include_directories(
    index PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/emsdk
)
if (EMSCRIPTEN)
    target_compile_options(
        index PRIVATE
    )
    target_link_libraries(
        index PRIVATE
        gbeml_core
        gbeml_driver
        SDL2
        --preload-file=${CMAKE_SOURCE_DIR}/src/app/web/rom@/rom
    )
else()
    target_link_libraries(
        index PRIVATE
        gbeml_core
        gbeml_driver
        SDL2
    )
endif()
