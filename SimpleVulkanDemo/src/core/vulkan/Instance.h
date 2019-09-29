#ifndef INSTANCE_H
#define INSTANCE_H

#include "../svd_include.h"
#include "Enumerators.h"
#include "Initializers.h"

#include <string>
#include <vector>

namespace SVD
{

class Instance
{
public:
	Instance(const char* app_name,
		const std::vector<const char*>& requiredExtensions,
		std::vector<const char*> requiredLayers);
	~Instance();

	Instance() = delete;
	Instance(const Instance&) = delete;
	Instance(const Instance&&) = delete;
	Instance& operator=(const Instance&) = delete;
	Instance& operator=(Instance&&) = delete;

	std::vector<VkPhysicalDevice> getGPUs();
private:
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	std::vector<const char*> extensions;
	std::vector<VkPhysicalDevice> gpus;
};
} // namespace SVD
#endif // INSTANCE_H