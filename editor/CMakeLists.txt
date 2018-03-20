cmake_minimum_required(VERSION 3.5)
project(editor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_OSX_ARCHITECTURES "i386")

set(SOURCE_FILES
        src/main.cpp
        src/Renderer/Player.cpp
        src/Renderer/TileMap.cpp
        src/Renderer/GameRenderer.cpp
        src/Renderer/PaletteManager.cpp
        src/globals.cpp
        src/Renderer/MiniMapManager.cpp
        src/Application/Application.cpp
        src/UI/CustomSurface.cpp
        src/UI/CustomListener.cpp
        src/UI/CustomSurfaceFactory.cpp
        src/Misc/Util.cpp
        src/Core/CoreEngine.cpp
        src/Core/EngineManager.cpp
        src/Core/CustomHandler.cpp
        src/Core/EngineComponent.cpp
        src/Core/GameObject.cpp
        src/Core/GameObjectComponent.cpp
        src/Renderer/GameObjectManager.cpp
        src/Renderer/Renderer.cpp
        src/Renderer/Controller.cpp
        src/Renderer/Collider.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(editor ${SOURCE_FILES})


FIND_LIBRARY(AWESOMIUM Awesomium)
FIND_LIBRARY(SFML SFML)

FIND_LIBRARY(SFML_GRAPHICS sfml-graphics)
FIND_LIBRARY(SFML_WINDOW sfml-window)
FIND_LIBRARY(SFML_SYSTEM sfml-system)
FIND_LIBRARY(SFML_NETWORK sfml-network)

TARGET_LINK_LIBRARIES(editor ${SFML} ${SFML_NETWORK} ${SFML_GRAPHICS} ${SFML_WINDOW} ${SFML_SYSTEM} ${AWESOMIUM})

