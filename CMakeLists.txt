cmake_minimum_required(VERSION 3.20)
project(ECLIPSE VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Platform detection
if(EMSCRIPTEN)
    set(PLATFORM_WEB TRUE)
    message(STATUS "Building for Web (Emscripten)")
else()
    set(PLATFORM_DESKTOP TRUE)
    message(STATUS "Building for Desktop")
endif()

# vcpkg integration
if(NOT PLATFORM_WEB)
    find_package(raylib CONFIG REQUIRED)
    find_package(SQLite3 REQUIRED)
    find_package(Catch2 3 CONFIG REQUIRED)
endif()

# Core library
add_library(eclipse_core STATIC
    src/core/grid.cpp
    src/core/grid.h
    src/core/constraints.cpp
    src/core/constraints.h
    src/core/solver.cpp
    src/core/solver.h
    src/core/generator.cpp
    src/core/generator.h
    src/core/region.cpp
    src/core/region.h
    src/core/daily_seed.cpp
    src/core/daily_seed.h
)

target_include_directories(eclipse_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(MSVC)
    target_compile_options(eclipse_core PRIVATE /W4)
else()
    target_compile_options(eclipse_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Desktop application
if(PLATFORM_DESKTOP)
    add_executable(eclipse_app
        src/app/main.cpp
        src/app/ui.cpp
        src/app/ui.h
        src/app/game_state.cpp
        src/app/game_state.h
        src/app/persistence.cpp
        src/app/persistence.h
    )

    target_link_libraries(eclipse_app PRIVATE 
        eclipse_core
        raylib
        SQLite::SQLite3
    )

    # Copy assets
    add_custom_command(TARGET eclipse_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:eclipse_app>/assets
    )

    # Tests
    add_executable(eclipse_tests
        tests/test_solver.cpp
        tests/test_generator.cpp
        tests/test_constraints.cpp
    )

    target_link_libraries(eclipse_tests PRIVATE
        eclipse_core
        Catch2::Catch2WithMain
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(eclipse_tests)
endif()

# Web build
if(PLATFORM_WEB)
    add_executable(eclipse_web
        src/app/main.cpp
        src/app/ui.cpp
        src/app/ui.h
        src/app/game_state.cpp
        src/app/game_state.h
        src/app/persistence.cpp
        src/app/persistence.h
    )

    target_link_libraries(eclipse_web PRIVATE eclipse_core)

    # Emscripten settings
    set_target_properties(eclipse_web PROPERTIES
        SUFFIX ".html"
        LINK_FLAGS "-s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 --preload-file ${CMAKE_SOURCE_DIR}/assets@assets"
    )

    # Find raylib for web
    target_include_directories(eclipse_web PRIVATE ${RAYLIB_INCLUDE_DIRS})
    target_link_libraries(eclipse_web PRIVATE raylib)
endif()

