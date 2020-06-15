#pragma once

#ifdef __linux__
#define PLATFORM_LINUX
#endif

#ifdef _WIN32
#define PLATFORM_WINDOWS
#endif

#ifdef PLATFORM_LINUX
    #define PLATFORM_NAME "Linux"
#endif

#ifdef PLATFORM_WINDOWS
    #define PLATFORM_NAME "Windows"
#endif