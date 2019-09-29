#include "Instance.h"

inline std::string getDebugMessageTypeString(VkDebugUtilsMessageTypeFlagsEXT messageType)
{
	if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) return "VALIDATION";
	else if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) return "GENERAL";
	else if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) return "PERFORMANCE";
	else return "UNKNOWN TYPE";
}

inline std::string getDebugMessageSeverityString(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity)
{
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) return "VERBOSE";
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) return "INFO";
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) return "WARNING";
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) return "ERROR";
	else return "UNKNOWN SEVERITY";
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) 
{
	std::string msgType = getDebugMessageTypeString(messageType);
	std::string msgSeverity = getDebugMessageSeverityString(messageSeverity);
	std::cerr << "[" << msgType << " => " << msgSeverity << "] " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(const VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(const VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger, VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

bool validate_extensions(
	const std::vector<const char *>& required,
	const std::vector<VkExtensionProperties> &available)
{
	std::vector<const char*> unsupported_extensions;
	for (auto extension : required)
	{
		bool found = false;
		for (auto &available_extension : available)
		{
			if (strcmp(available_extension.extensionName, extension) == 0)
			{
				found = true;
				break;
			}
		}
		if (!found) unsupported_extensions.push_back(extension);
	}

	for (auto& ext : unsupported_extensions)
	{
		std::cout << "Unsupported instance extension: " << ext << std::endl;
	}

	return true;
}

bool validate_layers(
	const std::vector<const char *>& required,
	const std::vector<VkLayerProperties> &available)
{
	std::vector<const char*> unsupported_layers;
	for (auto layer : required)
	{
		bool found = false;
		for (auto &available_layer : available)
		{
			if (strcmp(available_layer.layerName, layer) == 0)
			{
				found = true;
				break;
			}
		}
		if (!found) unsupported_layers.push_back(layer);
	}

	for (auto& l : unsupported_layers)
	{
		std::cout << "Unsupported validation layer: " << l << std::endl;
	}

	return true;
}

SVD::Instance::Instance(const char* app_name,
	const std::vector<const char*>& requiredExtensions,
	std::vector<const char*> requiredLayers)
{
	// INSTANCE EXTENSIONS
	if (VULKAN_VALIDATION) { 
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); 
	}
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	auto availableInstanceExtensions = Enumerators::extensionProperties();
	bool bres = validate_extensions(extensions, availableInstanceExtensions);
	CORE_AASSERT_TRUE((bres), "Required instance extensions are missing");

	// VALIDATION LAYERS
	if (VULKAN_VALIDATION) requiredLayers.push_back("VK_LAYER_KHRONOS_validation");
	auto instanceLayers = Enumerators::layerProperties();
	bres = validate_layers(requiredLayers, instanceLayers);
	CORE_AASSERT_TRUE((bres), "Required validation layers are missing");

	// INSTANCE CREATION
	VkApplicationInfo appInfo = Initializers::applicationInfo(app_name);
	VkInstanceCreateInfo createInfo = Initializers::instanceCreateInfo(extensions, requiredLayers, &appInfo);

	VkResult vres = vkCreateInstance(&createInfo, nullptr, &instance);
	CORE_AASSERT_VULKAN(vres, "Failed to create instance");

	// DEBUG MESSENGER
	VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = Initializers::debugMessengerCreateInfo(debugCallback);
	if (VULKAN_VALIDATION) { 
		vres = CreateDebugUtilsMessengerEXT(instance, &debugMessengerCreateInfo, nullptr, &debugMessenger);
		CORE_AASSERT_VULKAN(vres, "Failed to create debug messenger");
	}

	// ENUM PHYSICAL DEVICES
	gpus = Enumerators::physicalDevices(instance);
}

SVD::Instance::~Instance()
{
	if (VULKAN_VALIDATION) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	if (instance != VK_NULL_HANDLE) {
		vkDestroyInstance(instance, nullptr);
	}
}

std::vector<VkPhysicalDevice> SVD::Instance::getGPUs()
{
	return gpus;
}