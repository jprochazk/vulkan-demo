#ifndef ENUMERATORS_H
#define ENUMERATORS_H

#include "../svd_include.h"

#include <vector>

namespace SVD
{
namespace Enumerators
{

inline std::vector<VkExtensionProperties> extensionProperties()
{
	uint32_t instanceExtensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr);

	std::vector<VkExtensionProperties> availableInstanceExtensions(instanceExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, availableInstanceExtensions.data());

	return availableInstanceExtensions;
}

inline std::vector<VkLayerProperties> layerProperties()
{
	uint32_t instanceLayerCount;
	vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);

	std::vector<VkLayerProperties> instanceLayers(instanceLayerCount);
	vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayers.data());

	return instanceLayers;
}

inline std::vector<VkPhysicalDevice> physicalDevices(VkInstance& instanceHandle)
{
	std::vector<VkPhysicalDevice> gpus;

	uint32_t gpuCount = 0;
	vkEnumeratePhysicalDevices(instanceHandle, &gpuCount, nullptr);
	CORE_AASSERT_TRUE((gpuCount > 0), "No GPUs found");

	gpus.resize(gpuCount);
	vkEnumeratePhysicalDevices(instanceHandle, &gpuCount, gpus.data());

	return gpus;
}

inline std::vector<VkQueueFamilyProperties> queueFamilyProperties(VkPhysicalDevice& physicalDevice)
{
	std::vector<VkQueueFamilyProperties> props;
	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	CORE_AASSERT_TRUE((queueFamilyCount > 0), "No queue families found for GPU");

	props.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, props.data());

	return props;
}

inline std::vector<VkExtensionProperties> deviceExtensionProperties(VkPhysicalDevice& physicalDevice)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

	return availableExtensions;
}

} // namespace Enumerators
} // namespace SVD

#endif // ENUMERATORS_H