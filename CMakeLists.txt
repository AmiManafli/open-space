cmake_minimum_required(VERSION 3.15)
project(cg1_project)

#find_package(glfw3 3.3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# GLFW
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw" ${CMAKE_CURRENT_BINARY_DIR}/glfw)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/include")

# OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
    set(PROJECT_LIBRARIES ${PROJECT_LIBRARIES} ${OPENGL_LIBRARIES})
endif(OPENGL_FOUND)

# Assimp
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp" ${CMAKE_CURRENT_BINARY_DIR}/assimp)

# GLAD
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad")

# GLM
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm")

# ImGui
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui" PROJECT_SRCS)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui")

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/RenderWindow.cpp include/RenderWindow.h include/GLHeader.h src/RenderSystem.cpp include/RenderSystem.h src/InputManager.cpp include/InputManager.h src/Camera.cpp include/Camera.h src/Shader.cpp include/Shader.h src/Texture.cpp include/Texture.h)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${PROJECT_LIBRARIES})
