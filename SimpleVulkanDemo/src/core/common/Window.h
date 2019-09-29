#ifndef WINDOW_H
#define WINDOW_H

#include "../svd_include.h"
#include <vector>

namespace SVD 
{

class Window
{
public:
	Window(const char* name, int width, int height, bool resizable = false);
	~Window();

	Window() = delete;
	Window(const Window&) = delete;
	Window(const Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	std::vector<const char*> getRequiredExtensions();

	bool shouldClose();
	//void setResizeCallback(GLFWframebuffersizefun fun);
private:
	GLFWwindow* window;
	std::string winName;
	int width, height;

};

} // namespace SVD

#endif // WINDOW_H