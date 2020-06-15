#pragma once

#include "Platform/platform.hpp"
#include "game.hpp"
#include "Utils/types.hpp"
#include <vector>
#include <string>
#include <ctime>
#include <vulkan/vulkan.h>

namespace ege
{
    typedef enum EngineState{
        NOT_INITIALIZED,
        INITIALIZED,
        RUNNING,
        STOPPED
    } EngineState;

    class Engine
    {
    private:
        Platform* platform;
        Window* window;
        Game* game;
        EngineState state;
        std::clock_t lastTimeStamp;
        VkInstance vulkanInstance;
        std::string gameName;
        void createVulkanInstance();

        std::vector<const char*> getRequiredExtensions();
        std::vector<VkExtensionProperties> getAvailableExtensions();
        std::vector<const char*> checkRequiredExtensionsAvailable(bool *hasUnsupportedExtensions);

        std::vector<const char*> getRequiredLayers();
        std::vector<VkLayerProperties> getAvailableLayers();
        std::vector<const char*> checkRequiredLayersAvailable(bool *hasUnsupportedLayers);

        void setupDebugMessanger();

    public:
        Engine(std::string gameName);
        void init();
        void start(U32 width, U32 height, std::string title, Game* game);
        ~Engine();
    };
} 