cmake_minimum_required(VERSION 3.15)
project(cg1_project)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_SOURCE_DIR}/bindings)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

set(PROJECT_LIBRARIES)

include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/noise")

add_executable(${CMAKE_PROJECT_NAME}
        ${PROJECT_SRCS}
        src/main.cpp
        src/GLContext.cpp
        src/shaders/Shader.cpp
        src/shaders/ShaderProgram.cpp
        src/Axis.cpp
        src/entities/Entity.cpp
        src/entities/Component.cpp
        src/entities/EntityManager.cpp
        src/entities/systems/MovementSystem.cpp
        src/entities/systems/RenderSystem.cpp
        src/entities/components/TransformComponent.cpp
        src/entities/components/MeshComponent.cpp
        src/entities/EntityBuilder.cpp
        src/entities/systems/InputSystem.cpp
        src/entities/components/CameraComponent.cpp
        src/entities/components/HighlightComponent.cpp
        src/Application.cpp
        src/ui/UserInterface.cpp
        src/entities/components/VelocityComponent.cpp
        src/entities/components/LightComponent.cpp
        src/terrain/Terrain.cpp
        lib/noise/PerlinNoise.hpp
        src/terrain/Noise.cpp
        src/terrain/OpenSimplexNoise.cpp
        src/terrain/PerlinNoise.cpp
        include/cg/IcoSphere.cpp
        src/entities/components/MassComponent.cpp
        src/entities/systems/GravitySystem.cpp
        src/entities/components/OrbitComponent.cpp
        src/entities/systems/OrbitSystem.cpp
        src/SpaceshipControl.cpp
        src/ui/EntityWindow.cpp
        src/skybox/Skybox.cpp
        src/skybox/SkyboxStar.cpp
        include/cg/procedural/Universe.cpp
        include/cg/procedural/Galaxy.cpp
        include/cg/procedural/SolarSystem.cpp
        include/cg/procedural/Planet.cpp
        include/cg/procedural/Star.cpp
        include/cg/procedural/Moon.cpp
        include/cg/procedural/Generator.cpp
        include/cg/entities/UniverseEntityFactory.cpp
        src/collision/BoundingSphere.cpp
        src/entities/components/CollisionComponent.cpp
        src/entities/systems/CollisionSystem.cpp
        include/cg/PlanetGenerator.cpp
        include/cg/ui/PlanetEditor.cpp
        include/cg/PlanetSide.cpp
        include/cg/ViewFrustum.cpp
        include/cg/ViewPlane.cpp
        include/cg/ui/SkyboxEditor.cpp
        bindings/imgui_impl_glfw.cpp
        bindings/imgui_impl_opengl3.cpp
)

target_precompile_headers(${CMAKE_PROJECT_NAME} PUBLIC include/cg/GLHeader.h)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        ${PROJECT_LIBRARIES}
        ${CONAN_LIBS}
)
