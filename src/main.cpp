#include "ImGuiManager.h"
#include "Logging.h"

#include <GLFW/glfw3.h>

int main() {
    Logging::initSpdLog();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Sirius GCS", nullptr, nullptr);
    if (window == nullptr) {
        GCS_LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 800, 800);

    ImGuiManager::init(window);

    GCS_LOG_TRACE("Starting program");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiManager::render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGuiManager::shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    GCS_LOG_TRACE("Terminating program");
}