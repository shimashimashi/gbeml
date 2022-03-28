add_executable(
    gbeml
    main.cc
)
target_link_libraries(
    gbeml
    gbeml_core
    gbeml_driver
    SDL2
)
target_include_directories(
    gbeml PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
target_compile_options(
    gbeml PRIVATE
)
