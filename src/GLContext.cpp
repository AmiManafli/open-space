#include "cg/GLContext.h"

GLContext::GLContext(EntityManager *entityManager, std::string title, uint16_t width, uint16_t height)
        : entityManager(entityManager), title(title), width(width), height(height), viewFrustum(nullptr) {
}

GLContext::~GLContext() {
    delete meshProgram;
    delete meshTextureProgram;
    delete starProgram;
    delete planetProgram;
    delete gridProgram;
    delete highlightProgram;
    delete meshWithLightProgram;
    delete meshTestLightProgram;
    delete skyboxProgram;
    delete bloomProgram;
    delete blurProgram;
    delete viewFrustum;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

glm::mat4 GLContext::getView() {
    auto camera = getCamera();
    auto cameraComponent = entityManager->getComponent<CameraComponent>(camera);
    auto transformComponent = entityManager->getComponent<TransformComponent>(camera);
    return cameraComponent->getView(transformComponent);
}

glm::mat4 GLContext::getProjection() {
    auto cameraComponent = entityManager->getComponent<CameraComponent>(getCamera());
    return cameraComponent->getProjection(getAspect());
}

void GLContext::init() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("failed to initialize GLAD");
    }

    renderWireframe(displayWireframe);

    startTime = glfwGetTime();
    lastTime = startTime;
    lastFrameTime = lastTime;
    frameCount = 0;
    deltaTime = 0;
    timePerFrame = 0.0;

    setupImgui();

    printf("Size: %dx%d\n", width, height);

    int w, h;
    glfwGetWindowSize(window, &w, &h);
    printf("Real Size: %dx%d\n", w, h);
}

void GLContext::setupImgui() {
    const char* glslVersion = "#version 330";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

bool GLContext::shouldClose() {
    return glfwWindowShouldClose(window);
}

void GLContext::update() {
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrameTime;

    frameCount++;
    if (currentTime - lastTime >= 1.0) {
        timePerFrame = 1000.0 / double(frameCount);
        frameCount = 0;
        lastTime = currentTime;
    }

    lastFrameTime = currentTime;

    glfwPollEvents();
}

void GLContext::swapBuffers() {
    glfwSwapBuffers(window);
}

void GLContext::setActiveCamera(Entity *entity) {
    activeCamera = entity;
    if (viewFrustum) {
        delete viewFrustum;
    }
    if (entity) {
        auto camera = entityManager->getComponent<CameraComponent>(entity);
        auto cameraTransform = entityManager->getComponent<TransformComponent>(entity);
        viewFrustum = new ViewFrustum(*camera, *cameraTransform);
        updateViewFrustum();
    }
}

void GLContext::renderWireframe(bool enable) {
    glPolygonMode(GL_FRONT_AND_BACK, enable ? GL_LINE : GL_FILL);
    displayWireframe = enable;
}
