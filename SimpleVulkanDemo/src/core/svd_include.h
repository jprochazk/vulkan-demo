#ifndef SVDI_H
#define SVDI_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include <iostream>

#ifndef NDEBUG
#define VULKAN_VALIDATION true

namespace 
{
std::string errString(VkResult res)
{
	switch (res)
	{
#define STR(r) case VK_ ##r: return #r
		STR(NOT_READY);
		STR(TIMEOUT);
		STR(EVENT_SET);
		STR(EVENT_RESET);
		STR(INCOMPLETE);
		STR(ERROR_OUT_OF_HOST_MEMORY);
		STR(ERROR_OUT_OF_DEVICE_MEMORY);
		STR(ERROR_INITIALIZATION_FAILED);
		STR(ERROR_DEVICE_LOST);
		STR(ERROR_MEMORY_MAP_FAILED);
		STR(ERROR_LAYER_NOT_PRESENT);
		STR(ERROR_EXTENSION_NOT_PRESENT);
		STR(ERROR_FEATURE_NOT_PRESENT);
		STR(ERROR_INCOMPATIBLE_DRIVER);
		STR(ERROR_TOO_MANY_OBJECTS);
		STR(ERROR_FORMAT_NOT_SUPPORTED);
		STR(ERROR_SURFACE_LOST_KHR);
		STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
		STR(SUBOPTIMAL_KHR);
		STR(ERROR_OUT_OF_DATE_KHR);
		STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
		STR(ERROR_VALIDATION_FAILED_EXT);
		STR(ERROR_INVALID_SHADER_NV);
#undef STR
	default:
		return "UNKNOWN_ERROR";
	}
}

#define CORE_ASSERT_TRUE(Expr, Msg) \
	if(!Expr)\
	{\
		std::cerr << "Line " << __LINE__ << ", file " << __FILE__ << ": " << Msg << std::endl;\
	}
#define CORE_ASSERT_VULKAN(Res, Msg)	\
	if((Res != VK_SUCCESS))				\
	{									\
		std::cerr << errString(Res).c_str() << " at line " << __LINE__ << " in file " << __FILE__ << ": " << Msg << std::endl;\
	}
#define CORE_AASSERT_TRUE(Expr, Msg) \
	if(!Expr)\
	{\
		std::cerr << "Line " << __LINE__ << ", file " << __FILE__ << ": " << Msg << std::endl;\
		abort();\
	}
#define CORE_AASSERT_VULKAN(Res, Msg)	\
	if((Res != VK_SUCCESS))				\
	{									\
		std::cerr << errString(Res).c_str() << " at line " << __LINE__ << " in file " << __FILE__ << ": " << Msg << std::endl;\
		abort();\
	}

}
#else
#define VULKAN_VALIDATION false
#define CORE_ASSERT_TRUE(Expr, Msg)
#define CORE_ASSERT_VULKAN(Res, Msg)
#define CORE_AASSERT_TRUE(Expr, Msg)
#define CORE_AASSERT_VULKAN(Res, Msg)
#endif // NDEBUG

#endif // SVDI_H