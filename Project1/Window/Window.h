#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../Vulkan/Vulkan.h"


class Window
{
friend class Vulkan;

public:
	Window(const uint32_t WIDTH, const uint32_t HEIGHT);
	~Window();

	void run();

private:
	GLFWwindow* _window = nullptr;

	void glfw_clean_up();
	std::pair<uint32_t, const char**> get_required_from_vulkan_extensions();
};

