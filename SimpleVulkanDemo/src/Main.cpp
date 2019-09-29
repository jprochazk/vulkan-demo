
#include "core/vulkan/Instance.h"
#include "core/vulkan/Device.h"
#include "core/common/Window.h"

int main()
{
	SVD::Window window("Sample app", 800, 600);

	std::vector<const char*> requiredInstanceExtensions = window.getRequiredExtensions();
	std::vector<const char*> requiredValidationLayers = {};
	SVD::Instance instance("Sample app", requiredInstanceExtensions, requiredValidationLayers);

	VkPhysicalDeviceFeatures requiredFeatures = {};
	std::vector<const char*> requiredDeviceExtensions;
	requiredDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	SVD::Device device(instance.getGPUs(), requiredFeatures, requiredDeviceExtensions);

	while (!window.shouldClose())
	{
		glfwPollEvents();
	}

	return 0;
}