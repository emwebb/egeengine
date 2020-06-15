#pragma once
#include <vulkan/vulkan.h>
#include <string>
namespace ege
{
    PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT;

    VkResult loadVulkanExtensionMethod(VkInstance instance, std::string methodName);
} // namespace ege
