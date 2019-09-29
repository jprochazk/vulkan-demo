#include "Device.h"

bool validate_device_features(
	const VkPhysicalDeviceFeatures& required, 
	const VkPhysicalDeviceFeatures& available)
{
	std::vector<const char*> unavailable;
#define FCHECK(x) if(required.x && !available.x)\
	{\
		std::cout << #x << " required: " << required.x << ", available: " << available.x << std::endl;\
		unavailable.push_back(#x);\
	}

	FCHECK(robustBufferAccess);
	FCHECK(fullDrawIndexUint32);
	FCHECK(imageCubeArray);
	FCHECK(independentBlend);
	FCHECK(geometryShader);
	FCHECK(tessellationShader);
	FCHECK(sampleRateShading);
	FCHECK(dualSrcBlend);
	FCHECK(logicOp);
	FCHECK(multiDrawIndirect);
	FCHECK(drawIndirectFirstInstance);
	FCHECK(depthClamp);
	FCHECK(depthBiasClamp);
	FCHECK(fillModeNonSolid);
	FCHECK(depthBounds);
	FCHECK(wideLines);
	FCHECK(largePoints);
	FCHECK(alphaToOne);
	FCHECK(multiViewport);
	FCHECK(samplerAnisotropy);
	FCHECK(textureCompressionETC2);
	FCHECK(textureCompressionASTC_LDR);
	FCHECK(textureCompressionBC);
	FCHECK(occlusionQueryPrecise);
	FCHECK(pipelineStatisticsQuery);
	FCHECK(vertexPipelineStoresAndAtomics);
	FCHECK(fragmentStoresAndAtomics);
	FCHECK(shaderTessellationAndGeometryPointSize);
	FCHECK(shaderImageGatherExtended);
	FCHECK(shaderStorageImageExtendedFormats);
	FCHECK(shaderStorageImageMultisample);
	FCHECK(shaderStorageImageReadWithoutFormat);
	FCHECK(shaderUniformBufferArrayDynamicIndexing);
	FCHECK(shaderSampledImageArrayDynamicIndexing);
	FCHECK(shaderStorageBufferArrayDynamicIndexing);
	FCHECK(shaderStorageImageArrayDynamicIndexing);
	FCHECK(shaderClipDistance);
	FCHECK(shaderCullDistance);
	FCHECK(shaderFloat64);
	FCHECK(shaderInt64);
	FCHECK(shaderInt16);
	FCHECK(shaderResourceResidency);
	FCHECK(shaderResourceMinLod);
	FCHECK(sparseBinding);
	FCHECK(sparseResidencyBuffer);
	FCHECK(sparseResidencyImage2D);
	FCHECK(sparseResidencyImage3D);
	FCHECK(sparseResidency2Samples);
	FCHECK(sparseResidency4Samples);
	FCHECK(sparseResidency8Samples);
	FCHECK(sparseResidency16Samples);
	FCHECK(sparseResidencyAliased);
	FCHECK(variableMultisampleRate);
	FCHECK(inheritedQueries);
#undef FCHECK

	if (unavailable.empty()) return true;

	for (auto& feature : unavailable)
	{
		std::cout << "Unavailable feature: " << feature << std::endl;
	}
	return false;
}

bool validate_device_extensions(
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
		if(!found) unsupported_extensions.push_back(extension);
	}

	for (auto& ext : unsupported_extensions)
	{
		std::cout << "Unsupported device extension: " << ext << std::endl;
	}

	return true;
}

VkPhysicalDevice chooseGPU(const std::vector<VkPhysicalDevice>& gpus)
{
	for (auto& gpu : gpus)
	{
		VkPhysicalDeviceProperties props = {};
		vkGetPhysicalDeviceProperties(gpu, &props);

		if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			return gpu;
	}

	return gpus.at(0);
}

void populatePhysicalDeviceProperties(VkPhysicalDevice& physicalDevice, SVD::PhysicalDeviceProperties& props)
{
	vkGetPhysicalDeviceProperties(physicalDevice, &props.properties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &props.features);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &props.memory);

	props.queueFamilies = SVD::Enumerators::queueFamilyProperties(physicalDevice);
}

uint32_t getQueueFamilyIndex(
	const std::vector<VkQueueFamilyProperties>& queueFamilyProperties, 
	const VkQueueFlagBits& queueFlags)
{

	for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
	{
		// Dedicated queue for compute
		// Try to find a queue family index that supports compute but not graphics
		if (queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
			{
				return i;
				break;
			}
		}

		// Dedicated queue for transfer
		// Try to find a queue family index that supports transfer but not graphics and compute
		if (queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
			{
				return i;
				break;
			}
		}

		// For other queue types or if no separate compute queue is present, return the first one to support the requested flags
		if (queueFamilyProperties[i].queueFlags & queueFlags)
		{
			return i;
			break;
		}
	}

	return -1;
}

std::vector<VkDeviceQueueCreateInfo> populateQueueCreateInfos(
	SVD::QueueFamilyIndices& queueFamilyIndices,
	const SVD::PhysicalDeviceProperties& physicalDeviceProperties,
	const VkQueueFlags& requestedQueueTypes,
	const float& defaultQueuePriority)
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

	if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
	{
		queueFamilyIndices.graphics = getQueueFamilyIndex(physicalDeviceProperties.queueFamilies, VK_QUEUE_GRAPHICS_BIT);
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queueFamilyIndices.graphics;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &defaultQueuePriority;
		queueCreateInfos.push_back(queueInfo);
	}
	else
	{
		queueFamilyIndices.graphics = VK_NULL_HANDLE;
	}

	// Dedicated compute queue
	if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
	{
		queueFamilyIndices.compute = getQueueFamilyIndex(physicalDeviceProperties.queueFamilies, VK_QUEUE_COMPUTE_BIT);
		if (queueFamilyIndices.compute != queueFamilyIndices.graphics)
		{
			// If compute family index differs, we need an additional queue create info for the compute queue
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = queueFamilyIndices.compute;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(queueInfo);
		}
	}
	else
	{
		// Else we use the same queue
		queueFamilyIndices.compute = queueFamilyIndices.graphics;
	}

	// Dedicated transfer queue
	if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
	{
		queueFamilyIndices.transfer = getQueueFamilyIndex(physicalDeviceProperties.queueFamilies, VK_QUEUE_TRANSFER_BIT);
		if ((queueFamilyIndices.transfer != queueFamilyIndices.graphics) && (queueFamilyIndices.transfer != queueFamilyIndices.compute))
		{
			// If compute family index differs, we need an additional queue create info for the compute queue
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = queueFamilyIndices.transfer;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(queueInfo);
		}
	}
	else
	{
		// Else we use the same queue
		queueFamilyIndices.transfer = queueFamilyIndices.graphics;
	}

	return queueCreateInfos;
}

VkResult createLogicalDevice(
	VkDevice* deviceHandle,
	VkPhysicalDevice& physicalDevice,
	SVD::PhysicalDeviceProperties& physicalDeviceProperties,
	SVD::LogicalDeviceProperties& logicalDeviceProperties,
	SVD::QueueFamilyIndices& queueFamilyIndices
	)
{
	float defaultQueuePriority = 1.0f;
	VkQueueFlags requestQueues = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT;
	auto queueCreateInfos = populateQueueCreateInfos(queueFamilyIndices, physicalDeviceProperties, requestQueues, defaultQueuePriority);

	VkDeviceCreateInfo deviceCreateInfo = SVD::Initializers::deviceCreateInfo(queueCreateInfos, logicalDeviceProperties.enabledFeatures, logicalDeviceProperties.enabledExtensions);
	return vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, deviceHandle);
}

SVD::Device::Device(
	const std::vector<VkPhysicalDevice>& gpus,
	const VkPhysicalDeviceFeatures& requiredFeatures,
	const std::vector<const char*>& requiredExtensions)
{
	gpu = chooseGPU(gpus);
	populatePhysicalDeviceProperties(gpu, physicalDeviceProperties);

	bool bres = validate_device_extensions(requiredExtensions, Enumerators::deviceExtensionProperties(gpu));
	CORE_AASSERT_TRUE((bres), "Found unsupported device extensions");
	logicalDeviceProperties.enabledExtensions = requiredExtensions;

	bres = validate_device_features(requiredFeatures, physicalDeviceProperties.features);
	CORE_AASSERT_TRUE((bres), "Found unsupported device features");
	logicalDeviceProperties.enabledFeatures = requiredFeatures;

	VkResult vres = createLogicalDevice(&device, gpu, physicalDeviceProperties, logicalDeviceProperties, queueFamilyIndices);
	CORE_AASSERT_VULKAN(vres, "Failed to create logical device!");
}

SVD::Device::~Device()
{
	if (device)
	{
		vkDeviceWaitIdle(device);
		vkDestroyDevice(device, nullptr);
	}
}
