#pragma once
#include "../Config.h"

#include "../Vulkan/Vulkan.h"
#include "../Camera.h"
#include "../Scene.h"

class Vulkan;
class VulkanInit;

class Window
{
friend class Vulkan;
friend class VulkanInit;
friend static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

public:
	Window(const uint32_t WIDTH, const uint32_t HEIGHT, Scene *scene);
	~Window();

	void run();
private:
	GLFWwindow* _window = nullptr;
	Vulkan* vulkan = nullptr;
	Scene* scene = nullptr;

	void glfw_clean_up();
	void draw();
	void on_resize();
	std::pair<uint32_t, const char**> get_required_from_vulkan_extensions();
};

