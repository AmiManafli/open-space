cmake_minimum_required(VERSION 3.15)
project(cg1_project)
set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -fno-omit-frame-pointer -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# GLFW
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw" ${CMAKE_CURRENT_BINARY_DIR}/glfw)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/include")

set(PROJECT_LIBRARIES)

# OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
    set(PROJECT_LIBRARIES ${PROJECT_LIBRARIES} ${OPENGL_LIBRARIES})
endif(OPENGL_FOUND)

# Assimp
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_package(assimp CONFIG REQUIRED)
    include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp")
else()
    find_package(assimp REQUIRED)
    link_directories(${ASSIMP_LIBRARY_DIRS})
    include_directories(${ASSIMP_INCLUDE_DIRS})
endif()

# GLAD
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad")

# stb image
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/stb")

# Noise libraries
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/noise")

# JSON library
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/json")

# GLM
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm")

# ImGui
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui" PROJECT_SRCS)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui")

add_executable(${CMAKE_PROJECT_NAME}
        ${PROJECT_SRCS}
        src/main.cpp
        lib/glad/glad.c
        src/GLContext.cpp
        include/cg/GLContext.h
        include/cg/GLHeader.h
        src/shaders/Shader.cpp
        include/cg/shaders/Shader.h
        src/shaders/ShaderProgram.cpp
        include/cg/shaders/ShaderProgram.h
        src/Axis.cpp
        include/cg/Axis.h
        src/entities/Entity.cpp
        include/cg/entities/Entity.h
        src/entities/Component.cpp
        include/cg/entities/Component.h
        src/entities/System.cpp
        include/cg/entities/System.h
        src/entities/EntityManager.cpp
        include/cg/entities/EntityManager.h
        src/entities/systems/MovementSystem.cpp
        include/cg/entities/systems/MovementSystem.h
        src/entities/systems/RenderSystem.cpp
        include/cg/entities/systems/RenderSystem.h
        src/entities/components/TransformComponent.cpp
        include/cg/entities/components/TransformComponent.h
        include/cg/entities/components/MeshComponent.h
        src/entities/components/MeshComponent.cpp
        src/entities/EntityBuilder.cpp
        include/cg/entities/EntityBuilder.h
        src/entities/systems/InputSystem.cpp
        include/cg/entities/systems/InputSystem.h
        src/entities/components/CameraComponent.cpp
        include/cg/entities/components/CameraComponent.h
        src/entities/components/HighlightComponent.cpp
        include/cg/entities/components/HighlightComponent.h
        src/Application.cpp
        include/cg/Application.h
        src/ui/UserInterface.cpp
        include/cg/ui/UserInterface.h
        src/entities/components/VelocityComponent.cpp
        include/cg/entities/components/VelocityComponent.h
        src/entities/components/LightComponent.cpp
        include/cg/entities/components/LightComponent.h
        src/terrain/Terrain.cpp
        include/cg/terrain/Terrain.h
        lib/noise/OpenSimplexNoise.h lib/noise/PerlinNoise.hpp src/terrain/Noise.cpp include/cg/terrain/Noise.h src/terrain/OpenSimplexNoise.cpp include/cg/terrain/OpenSimplexNoise.h src/terrain/PerlinNoise.cpp include/cg/terrain/PerlinNoise.h lib/json/json.hpp include/cg/terrain/TestNoise.h include/cg/IcoSphere.cpp include/cg/IcoSphere.h src/entities/components/MassComponent.cpp include/cg/entities/components/MassComponent.h src/entities/systems/GravitySystem.cpp include/cg/entities/systems/GravitySystem.h src/entities/components/OrbitComponent.cpp include/cg/entities/components/OrbitComponent.h src/entities/systems/OrbitSystem.cpp include/cg/entities/systems/OrbitSystem.h include/cg/Math.h include/cg/SpaceshipControl.h src/SpaceshipControl.cpp include/cg/Math.h src/ui/EntityWindow.cpp include/cg/ui/EntityWindow.h src/skybox/Skybox.cpp include/cg/skybox/Skybox.h src/skybox/SkyboxStar.cpp include/cg/skybox/SkyboxStar.h include/cg/entities/components/SelectableComponent.h include/cg/procedural/Universe.cpp include/cg/procedural/Universe.h include/cg/procedural/Galaxy.cpp include/cg/procedural/Galaxy.h include/cg/procedural/SolarSystem.cpp include/cg/procedural/SolarSystem.h include/cg/procedural/Planet.cpp include/cg/procedural/Planet.h include/cg/procedural/Generator.h include/cg/procedural/Star.cpp include/cg/procedural/Star.h include/cg/procedural/Moon.cpp include/cg/procedural/Moon.h include/cg/procedural/Generator.cpp include/cg/entities/UniverseEntityFactory.cpp include/cg/entities/UniverseEntityFactory.h src/collision/BoundingSphere.cpp include/cg/collision/BoundingSphere.h src/entities/components/CollisionComponent.cpp include/cg/entities/components/CollisionComponent.h src/entities/systems/CollisionSystem.cpp include/cg/entities/systems/CollisionSystem.h include/cg/Cube.cpp include/cg/Cube.h)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    glfw
    assimp::assimp
    ${PROJECT_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    ${IMGUI_LIBRARIES}
)
