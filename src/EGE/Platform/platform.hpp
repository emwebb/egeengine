#pragma once

#include <string>
#include <vector>
#include "../Utils/types.hpp"
namespace ege
{
    typedef void Window;
    class Platform
    {
    private:
    public:
        virtual void init() = 0;
        virtual Window* initWindow(U32 width, U32 height, std::string title) = 0;
        virtual bool mustQuit(Window* window) = 0;
        virtual void poll(Window* window) = 0;
        virtual void deleteWindow(Window* window) = 0;
        virtual std::vector<const char*> getRequiredExtensions() = 0;
    };
}
