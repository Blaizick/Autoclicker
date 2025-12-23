cmake_minimum_required(VERSION 4.0)
project(Autoclicker)

set(CMAKE_CXX_STANDARD 26)

set(PROJECT_SOURCES src/Autoclicker/main.cpp
        src/Autoclicker/Autoclicker.cpp
        src/Autoclicker/Autoclicker.h
)

set(IMGUI_SOURCES lib/imgui/imgui.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/backends/imgui_impl_opengl3.cpp
        lib/imgui/backends/imgui_impl_sdl3.cpp
)
set(IMGUI_INCLUDE_DIRECTORIES lib/imgui
        lib/imgui/backends)

add_executable(Autoclicker ${PROJECT_SOURCES}
        ${IMGUI_SOURCES}
)

target_include_directories(Autoclicker PRIVATE ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/lib/sdl3/include
        ${IMGUI_INCLUDE_DIRECTORIES}
)

target_compile_definitions(Autoclicker PRIVATE SDL_STATIC)

target_link_libraries(Autoclicker PRIVATE ${CMAKE_SOURCE_DIR}/lib/sdl3/lib/libSDL3.a
        opengl32
        winmm
        imm32
        version
        setupapi
        ole32
        oleaut32
        uuid
        advapi32
        shell32
        user32
        gdi32
)