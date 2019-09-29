#ifndef DEVICE_H
#define DEVICE_h

#include "../svd_include.h"
#include "Enumerators.h"
#include "Initializers.h"
#include <vector>

namespace SVD
{

typedef struct _PhysicalDeviceProperties {
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures features;
	VkPhysicalDeviceMemoryProperties memory;
	std::vector<VkQueueFamilyProperties> queueFamilies;
} PhysicalDeviceProperties;

typedef struct _LogicalDeviceProperties {
	VkPhysicalDeviceFeatures enabledFeatures;
	std::vector<const char*> enabledExtensions;
} LogicalDeviceProperties;

typedef struct _QueueFamilyIndices {
	uint32_t graphics;
	uint32_t compute;
	uint32_t transfer;
} QueueFamilyIndices;

class Device
{
public:
	Device(const std::vector<VkPhysicalDevice>& gpus,
		const VkPhysicalDeviceFeatures& requiredFeatures,
		const std::vector<const char*>& requiredExtensions);
	~Device();

	Device() = delete;
	Device(const Device&) = delete;
	Device(const Device&&) = delete;
	Device& operator=(const Device&) = delete;
	Device& operator=(Device&&) = delete;
private:
	VkDevice device;

	VkPhysicalDevice gpu;
	PhysicalDeviceProperties physicalDeviceProperties;
	LogicalDeviceProperties logicalDeviceProperties;
	QueueFamilyIndices queueFamilyIndices;
};

} // namespace SVD

#endif // DEVICE_H