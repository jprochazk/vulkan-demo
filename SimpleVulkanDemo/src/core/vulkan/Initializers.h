#ifndef INITIALIZERS_H
#define INITIALIZERS_H

#include "../svd_include.h"

namespace SVD
{
namespace Initializers
{

inline VkApplicationInfo applicationInfo(
	const char* appName,
	uint32_t appVersion = VK_MAKE_VERSION(1, 0, 0),
	const char* engineName = "no engine",
	uint32_t engineVersion = VK_MAKE_VERSION(1, 0, 0),
	uint32_t apiVersion = VK_API_VERSION_1_1)
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName;
	appInfo.applicationVersion = appVersion;
	appInfo.pEngineName = engineName;
	appInfo.engineVersion = engineVersion;
	appInfo.apiVersion = apiVersion;
	return appInfo;
}

inline VkInstanceCreateInfo instanceCreateInfo(
	const std::vector<const char*>& enabledExtensions,
	const std::vector<const char*>& enabledLayers,
	VkApplicationInfo* appInfo = nullptr
)
{
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
	createInfo.ppEnabledExtensionNames = enabledExtensions.data();
	createInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
	createInfo.ppEnabledLayerNames = enabledLayers.data();

	VkApplicationInfo t_appInfo = applicationInfo("Sample Application");
	createInfo.pApplicationInfo = (appInfo == nullptr) ? &t_appInfo : appInfo;

	return createInfo;
}

inline VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo(PFN_vkDebugUtilsMessengerCallbackEXT debugCallback, bool showInfo = false)
{
	VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {};
	debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

	VkDebugUtilsMessageSeverityFlagsEXT messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	if (showInfo) messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	debugMessengerCreateInfo.messageSeverity = messageSeverity;

	debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugMessengerCreateInfo.pfnUserCallback = debugCallback;
	debugMessengerCreateInfo.pUserData = nullptr; // Optional
	return debugMessengerCreateInfo;
}

inline VkDeviceCreateInfo deviceCreateInfo(
	std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
	VkPhysicalDeviceFeatures& enabledFeatures,
	std::vector<const char*>& enabledExtensions)
{
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
	deviceCreateInfo.pNext = nullptr; // Extension specific (maybe support in the future)
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();

	return deviceCreateInfo;
}

} // namespace Initializers
} // namespace SVD

#endif // INITIALIZERS_H