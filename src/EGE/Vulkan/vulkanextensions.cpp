#include "vulkanextensions.hpp"

VkResult ege::loadVulkanExtensionMethod(VkInstance instance, std::string methodName, void** methodPointer) {
    *methodPointer = (void*) vkGetInstanceProcAddr(instance, methodName.c_str());

    if(*methodPointer == nullptr) {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    } else {
        return VK_SUCCESS;
    }
}