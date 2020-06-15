#include "engine.hpp"
#include "Utils/consts.hpp"
#include <string.h>
#include "Utils/logger.hpp"
#ifdef PLATFORM_LINUX
    #include "Platform/linux/linuxplatform.hpp"
#endif
#ifdef PLATFORM_WINDOWS
    #include "Platform/windows/windowsplatform.hpp"
#endif
namespace ege
{
    Engine::Engine(std::string gameName)
    {
        LOG_TRACE("Started creating Engine object");
        this->gameName = gameName;
        state = NOT_INITIALIZED;
        LOG_TRACE("Finished creating ENgine object");
    }

    void Engine::init() 
    {
        LOG_INFO("Initializing Engine");
        ASSERT_FUNC(TRACE, 1==0);
        #ifdef PLATFORM_LINUX
            LOG_INFO("Initializing Linux platform wrapper");
            platform = new LinuxPlatform();
        #elif defined(PLATFORM_WINDOWS)
            LOG_INFO("Initializing Windows platform wrapper");
            platform = new WindowsPlatform();
        #else
            LOG_FATAL("PLATFORM NOT SUPPORTED!")
        #endif
        platform->init();
        createVulkanInstance();
        state = INITIALIZED;
    }

    void Engine::createVulkanInstance() 
    {

        LOG_INFO("Creating Vulkan instance");
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = gameName.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.pEngineName = "EGE Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.apiVersion = VK_API_VERSION_1_2;


        bool hasUnsupportedExtensions;

        LOG_INFO("Checking available Vulkan extensions");
        std::vector<const char*> unsupportedExtensions = checkRequiredExtensionsAvailable(&hasUnsupportedExtensions);
        if(hasUnsupportedExtensions) {
            LOG_ERROR("Unsupported Vulkan extensions required!");
            LOG_ERROR("Unsupported Extensions:")
            for(std::vector<const char*>::iterator it = unsupportedExtensions.begin(); it != unsupportedExtensions.end(); ++it)
            {
                LOG_ERROR(std::string("    ") + std::string(*it));
            }
            LOG_FATAL("Cannot run game due to unsupported extensions.")
        }
        std::vector<const char*> extensions = getRequiredExtensions();

        bool hasUnsupportedLayers;

        LOG_INFO("Checking available Vulkan extensions");
        std::vector<const char*> unsupportedLayers = checkRequiredLayersAvailable(&hasUnsupportedLayers);
        if(hasUnsupportedLayers) {
            LOG_ERROR("Unsupported Vulkan layers required!");
            LOG_ERROR("Unsupported Layers:")
            for(std::vector<const char*>::iterator it = unsupportedLayers.begin(); it != unsupportedLayers.end(); ++it)
            {
                LOG_ERROR(std::string("    ") + std::string(*it));
            }
            LOG_FATAL("Cannot run game due to unsupported layers.")
        }
        std::vector<const char*> layers = getRequiredLayers();


        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo; 
        createInfo.enabledExtensionCount = extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.ppEnabledLayerNames = layers.data();
        createInfo.enabledLayerCount = layers.size();

        FORCE_ASSERT_VK_S(FATAL, vkCreateInstance(&createInfo, nullptr, &vulkanInstance));
        LOG_INFO("Vulkan instance created");
        
    }

    std::vector<const char*> Engine::getRequiredExtensions()
    {
        std::vector<const char*> extensions = platform->getRequiredExtensions();
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        return extensions;
    }
    
    std::vector<VkExtensionProperties> Engine::getAvailableExtensions()
    {
        U32 count;
        vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
        std::vector<VkExtensionProperties> extensions(count);
        vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());
        return extensions;
    }

    std::vector<const char*> Engine::checkRequiredExtensionsAvailable(bool *hasUnsupportedExtensions)
    {
        *hasUnsupportedExtensions = false;
        std::vector<const char*> requiredExtensions = getRequiredExtensions();
        std::vector<VkExtensionProperties> availableExtensions = getAvailableExtensions();
        std::vector<const char*> unsupportedExtensions;
        for(std::vector<const char*>::iterator reqIt = requiredExtensions.begin(); reqIt != requiredExtensions.end(); ++reqIt)
        {
            bool available = false;
            for(std::vector<VkExtensionProperties>::iterator avalIt = availableExtensions.begin(); avalIt != availableExtensions.end(); ++avalIt)
            {
                if(!strcmp(avalIt->extensionName, *reqIt))
                {
                    available = true;
                    break;
                }
            }
            if(!available) {
                *hasUnsupportedExtensions = true;
                unsupportedExtensions.push_back(*reqIt);
            }
        }
        return unsupportedExtensions;
    }

    std::vector<const char*> Engine::getRequiredLayers()
    {
        return std::vector<const char*>{
            "VK_LAYER_KHRONOS_validation"
        };
    }

    std::vector<VkLayerProperties> Engine::getAvailableLayers()
    {
        U32 count;
        vkEnumerateInstanceLayerProperties(&count, nullptr);

        std::vector<VkLayerProperties> availableLayers(count);
        vkEnumerateInstanceLayerProperties(&count, availableLayers.data());

        return availableLayers;
    }

    std::vector<const char*> Engine::checkRequiredLayersAvailable(bool *hasUnsupportedLayers)
    {
        *hasUnsupportedLayers = false;
        std::vector<const char*> requiredLayers = getRequiredLayers();
        std::vector<VkLayerProperties> availableLayers = getAvailableLayers();
        std::vector<const char*> unsupportedLayers;
        for(std::vector<const char*>::iterator reqIt = requiredLayers.begin(); reqIt != requiredLayers.end(); ++reqIt)
        {
            bool available = false;
            for(std::vector<VkLayerProperties>::iterator avalIt = availableLayers.begin(); avalIt != availableLayers.end(); ++avalIt)
            {
                if(!strcmp(avalIt->layerName, *reqIt))
                {
                    available = true;
                    break;
                }
            }
            if(!available) {
                *hasUnsupportedLayers = true;
                unsupportedLayers.push_back(*reqIt);
            }
        }
        return unsupportedLayers;
    }

    void Engine::start(U32 width, U32 height, std::string title, Game* game)
    {
        lastTimeStamp = std::clock();
        window = platform->initWindow(width, height, title);
        this->game = game;
        this->game->init(this);
        state = RUNNING;
        while(!platform->mustQuit(window))
        {
            platform->poll(window);
            std::clock_t newTimeStamp = clock();
            F32 deltaTime = F32(newTimeStamp - lastTimeStamp) / CLOCKS_PER_SEC;
            lastTimeStamp = newTimeStamp;
            this->game->loop(deltaTime);
        }
        state = STOPPED;
    }

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT){
            LOG_ERROR(std::string("Vulkan Debug Call: ") + std::string(pCallbackData->pMessage));
        }

        if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT){
            LOG_WARN(std::string("Vulkan Debug Call: ") + std::string(pCallbackData->pMessage));
        }

        if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT){
            LOG_WARN(std::string("Vulkan Debug Call: ") + std::string(pCallbackData->pMessage));
        }

        if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT){
            LOG_TRACE(std::string("Vulkan Debug Call: ") + std::string(pCallbackData->pMessage));
        }

        return VK_FALSE;
    }

    void Engine::setupDebugMessanger()
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        createInfo.messageType = 
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;


    }

    Engine::~Engine()
    {

    }
} // namespace ege


