#include "Window.h"

SVD::Window::Window(const char* name, int width, int height, bool resizable)
	: winName(name), width(width), height(height)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(width, height, name, nullptr, nullptr);

	// TODO: implement resizable
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

SVD::Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

std::vector<const char*> SVD::Window::getRequiredExtensions()
{
	uint32_t extensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	return std::vector<const char*>(glfwExtensions, glfwExtensions + extensionCount);
}

bool SVD::Window::shouldClose()
{
	return glfwWindowShouldClose(window);
}