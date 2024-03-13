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

	double last_time, current_time;
	int num_frames;
	float frame_time;

	void glfw_clean_up();
	void draw();
	void on_resize();
	void calculate_fps();
	std::pair<uint32_t, const char**> get_required_from_vulkan_extensions();
};

