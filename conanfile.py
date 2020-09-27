from conans import ConanFile

class OpenSpaceSimulator(ConanFile):
    name = 'open-space-simulator'
    version = '0.0.1'
    generators = 'cmake'
    settings = 'os'
    requires = 'glad/0.1.33', 'glfw/3.3.2', 'glm/0.9.9.8', 'imgui/1.78', 'stb/20200203', 'nlohmann_json/3.9.1', 'di/1.2.0'
    default_options = {
        'di:with_extensions': True
    }

    def imports(self):
        self.copy('./res/bindings', 'imgui_impl_glfw.cpp', '../bindings')
        self.copy('./res/bindings', 'imgui_impl_opengl3.cpp', '../bindings')
        self.copy('./res/bindings', 'imgui_impl_glfw.h', '../bindings')
        self.copy('./res/bindings', 'imgui_impl_opengl3.h', '../bindings')

    def requirements(self):
        if self.settings.os != 'Windows':
            self.requires('llvm-openmp/10.0.0')