#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <csignal>
#include "consts.hpp"
#define LOG_TIME_FORMAT "%y-%m-%d %H:%M:%S"
#define LOG(level,object,line,message){\
    auto t = std::time(nullptr);\
    auto tm = std::localtime(&t);\
    const auto now = std::chrono::system_clock::now();\
    const auto nowMs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;\
    std::cout << "[" << std::put_time(tm, LOG_TIME_FORMAT) << "." << std::setfill('0') << std::setw(6) << nowMs.count() << "][" << level << "][" << object << ":" << line << "]:" << message << std::endl;\
}
#define LOG_B(level,message) LOG(level,__FILE__,__LINE__,message)

#if LOG_LEVEL >= 5
    #define LOG_LEVEL_TRACE
#endif 

#if LOG_LEVEL >= 5
    #define LOG_LEVEL_TRACE
#endif

#if LOG_LEVEL >= 4
    #define LOG_LEVEL_INFO
#endif

#if LOG_LEVEL >= 3
    #define LOG_LEVEL_WARN
#endif 

#if LOG_LEVEL >= 2
    #define LOG_LEVEL_ERROR
#endif 

#define LOG_LEVEL_FATAL

#ifdef LOG_LEVEL_TRACE
    #define LOG_TRACE(message) LOG_B("TRACE", message)
#else
    #define LOG_TRACE(message)
#endif

#ifdef LOG_LEVEL_INFO
    #define LOG_INFO(message) LOG_B("INFO", message)
#else
    #define LOG_INFO(message)
#endif

#ifdef LOG_LEVEL_WARN
    #define LOG_WARN(message) LOG_B("WARN", message)
#else
    #define LOG_WARN(message)
#endif

#ifdef LOG_LEVEL_ERROR
    #define LOG_ERROR(message) LOG_B("ERROR", message)
#else
    #define LOG_ERROR(message)
#endif

#ifdef LOG_LEVEL_FATAL
    #define LOG_FATAL(message) LOG_B("FATAL", message)\
        std::raise(SIGINT);
#else
    #define LOG_FATAL(message)\
        std::raise(SIGINT);
#endif

#define FORCE_ASSERT(level, result, message){\
        if(!(result)){\
            LOG_##level(std::string("Assertion Failed :") + std::string(message));\
        }\
    }

#define FORCE_ASSERT_FUNC(level, func){\
        FORCE_ASSERT(level, func, #func)\
    }

#ifdef PLATFORM_LINUX
#define FORCE_ASSERT_VK_S(level, vkfunc)({\
    auto vkResult = vkfunc;\
    FORCE_ASSERT(level, vkResult == VK_SUCCESS, "Vulkan command did not return VK_SUCCESS:" #vkfunc )\
    vkResult;\
})
#else
#define FORCE_ASSERT_VK_S(level, vkfunc)([=](){\
    auto vkResult = vkfunc; \
    FORCE_ASSERT(level, vkResult == VK_SUCCESS, "Vulkan command did not return VK_SUCCESS:" #vkfunc)\
})
#endif

#ifdef DO_ASSERTS
    #define ASSERT(level, result, message) FORCE_ASSERT(level, result, message)

    #define ASSERT_FUNC(level, func){\
        ASSERT(level, func, #func)\
    }

    #define ASSRT_FUNC_FR(level, func) ASSERT_FUNC(level, func)

    #define ASSERT_VK_S(level, vkfunc) FORCE_ASSERT_VK_S(level, vkfunc)

#else
    #define ASSERT(level, result, message)
    #define ASSERT_FUNC(level, func)
    #define ASSERT_FUNC_FR(level, func) func
    #define ASSERT_VK_S(level, vkfunc) vkfunc
#endif