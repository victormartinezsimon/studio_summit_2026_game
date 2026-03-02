cmake_minimum_required(VERSION 3.12)

# Nombre del proyecto
project(StudioSubmitGame)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Añade los archivos fuente y de cabecera
set(SOURCES
    src/Main.cpp
    src/Plane.cpp
    src/Bullet.cpp
    src/TextPainter.cpp
    src/InputManager.cpp
)

set (HEADERS
    src/Pool.h
    src/Plane.h
    src/Bullet.h
    src/TextPainter.h
    src/InputManager.h
    src/GameConfig.h
    src/WorldObject.h
    )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  

find_path(RAYLIB_CPP_INCLUDE_DIRS "raylib-cpp.hpp")


if (WIN32)

    source_group("Headers" FILES ${HEADERS} )
    source_group("Sources" FILES ${SOURCES} )
    add_executable(StudioSubmitGame ${SOURCES} ${HEADERS} )
    target_include_directories(StudioSubmitGame PRIVATE ${RAYLIB_CPP_INCLUDE_DIRS})

elseif (UNIX)

    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
        set(PLATFORM_LIBS atomic)
    endif()

    source_group("Headers" FILES ${HEADERS} )
    source_group("Sources" FILES ${SOURCES} )
    target_include_directories(StudioSubmitGame PRIVATE ${RAYLIB_CPP_INCLUDE_DIRS})

    add_executable(StudioSubmitGame ${SOURCES} ${HEADERS} )

    target_link_libraries(StudioSubmitGame ${PLATFORM_LIBS})

endif()

target_link_libraries(StudioSubmitGame PRIVATE raylib)
target_compile_features(StudioSubmitGame PRIVATE cxx_std_20)

message(STATUS "CMAKE Ended ")