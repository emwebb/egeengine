#pragma once

#include "../platform.hpp"
namespace ege
{
    class WindowsPlatform:  public Platform
    {
    private:
        /* data */
        bool initDone;
    public:
        WindowsPlatform(/* args */);
        ~WindowsPlatform();

        void init();
        Window* initWindow(U32 width, U32 height, std::string title);
        bool mustQuit(Window* window);
        void poll(Window* window);
        void deleteWindow(Window* window);
        std::vector<const char*> getRequiredExtensions();
    }; 
} // namespace ege