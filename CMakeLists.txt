cmake_minimum_required(VERSION 3.15)
project(cg1_project)

set(CMAKE_CXX_STANDARD 17)

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
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp" ${CMAKE_CURRENT_BINARY_DIR}/assimp)
find_package(assimp REQUIRED)
link_directories(${ASSIMP_LIBRARY_DIRS})
include_directories(${ASSIMP_INCLUDE_DIRS})

# GLAD
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad")

# GLM
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm")

# ImGui
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui" PROJECT_SRCS)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui")

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp lib/glad/glad.c src/GLContext.cpp include/GLContext.h include/GLHeader.h src/RenderSystemOld.cpp include/RenderSystemOld.h src/InputManager.cpp include/InputManager.h src/Camera.cpp include/Camera.h src/shaders/Shader.cpp include/shaders/Shader.h src/meshes/Texture.cpp include/meshes/Texture.h src/meshes/Mesh.cpp include/meshes/Mesh.h src/meshes/Model.cpp include/meshes/Model.h src/shaders/ShaderProgram.cpp include/shaders/ShaderProgram.h src/Grid.cpp include/Grid.h ${PROJECT_SRCS} src/Axis.cpp include/Axis.h src/entities/Entity.cpp include/entities/Entity.h src/entities/Component.cpp include/entities/Component.h src/entities/System.cpp include/entities/System.h src/entities/EntityManager.cpp include/entities/EntityManager.h src/entities/systems/MovementSystem.cpp include/entities/systems/MovementSystem.h src/entities/systems/RenderSystem.cpp include/entities/systems/RenderSystem.h include/entities/components/PositionComponent.h)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${PROJECT_LIBRARIES} ${GLFW_LIBRARIES} ${ASSIMP_LIBRARIES} ${IMGUI_LIBRARIES})
