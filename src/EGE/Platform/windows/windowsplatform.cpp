#include "windowsplatform.hpp"
#include "GLFW/glfw3.h"
namespace ege
{
    WindowsPlatform::WindowsPlatform() : Platform() {
        initDone = false;
    }

    void WindowsPlatform::init() {
        glfwInit();
    }
    
    Window* WindowsPlatform::initWindow(U32 width, U32 height, std::string title) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        return window;
    }


    bool WindowsPlatform::mustQuit(Window* window) {
        return glfwWindowShouldClose((GLFWwindow*)window);
    }

    void WindowsPlatform::poll(Window* window) {
        glfwPollEvents();
    }

    void WindowsPlatform::deleteWindow(Window* window) {
        glfwDestroyWindow((GLFWwindow*)window);
    }

    std::vector<const char*> WindowsPlatform::getRequiredExtensions() {
        U32 extensionCount;
        const char** extensions_c = glfwGetRequiredInstanceExtensions(&extensionCount);
        std::vector<const char*> extensions(extensions_c, extensions_c + extensionCount);
        return extensions;
    }

    WindowsPlatform::~WindowsPlatform() {
        if(initDone) {
            glfwTerminate();
        }
    }
}



