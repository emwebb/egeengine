#include "linuxplatform.hpp"
#include "GLFW/glfw3.h"
namespace ege
{
    LinuxPlatform::LinuxPlatform() : Platform() {
        initDone = false;
    }

    void LinuxPlatform::init() {
        glfwInit();
    }
    
    Window* LinuxPlatform::initWindow(U32 width, U32 height, std::string title) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        return window;
    }


    bool LinuxPlatform::mustQuit(Window* window) {
        return glfwWindowShouldClose((GLFWwindow*)window);
    }

    void LinuxPlatform::poll(Window* window) {
        glfwPollEvents();
    }

    void LinuxPlatform::deleteWindow(Window* window) {
        glfwDestroyWindow((GLFWwindow*)window);
    }

    std::vector<const char*> LinuxPlatform::getRequiredExtensions() {
        U32 extensionCount;
        const char** extensions_c = glfwGetRequiredInstanceExtensions(&extensionCount);
        std::vector<const char*> extensions(extensions_c, extensions_c + extensionCount);
        return extensions;
    }

    LinuxPlatform::~LinuxPlatform() {
        if(initDone) {
            glfwTerminate();
        }
    }
}



