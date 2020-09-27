#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cg/Math.h>

#ifdef CHECK_MEMORY_LEAKS
#include <vld.h>
#endif
#ifdef ENABLE_OPENMP
#include <omp.h>
#endif

#include <boost/di.hpp>

namespace di = boost::di;
