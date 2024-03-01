#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../Vulkan/Vulkan.h"


class Window
{
friend class Vulkan;
friend static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

public:
	Window(const uint32_t WIDTH, const uint32_t HEIGHT);
	~Window();

	void run();
private:
	GLFWwindow* _window = nullptr;
	Vulkan* vulkan = nullptr;

	float lastFrameTime = 0.0f;
	double lastTime = 0.0f;

	void glfw_clean_up();
	void draw();
	void on_resize();
	std::pair<uint32_t, const char**> get_required_from_vulkan_extensions();
};

