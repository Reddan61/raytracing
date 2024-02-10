#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>

#include "../Window/Window.h"

class Window;

class Vulkan
{
public:
	Vulkan(Window *window);
	~Vulkan();
private:
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphics_family;
		std::optional<uint32_t> present_family;

		bool isComplete() {
			return graphics_family.has_value() && present_family.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> present_modes;
	};

	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phys_device = VK_NULL_HANDLE;
	VkDevice logical_device = VK_NULL_HANDLE;
	VkQueue graphics_queue = VK_NULL_HANDLE;
	VkQueue present_queue = VK_NULL_HANDLE;
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkSwapchainKHR swap_chain = VK_NULL_HANDLE;

	std::vector<VkImage> swap_ñhain_images;
	VkFormat swap_chain_image_format;
	VkExtent2D swap_chain_extent;

	VkDebugUtilsMessengerCreateInfoEXT debug_create_instance_info;
	VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;

	VkDebugUtilsMessengerEXT debug_messenger;
	const std::vector<const char*> validation_layers = {
		"VK_LAYER_KHRONOS_validation"
	};
	const std::vector<const char*> device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkApplicationInfo create_app_info();
	VkInstanceCreateInfo create_instance_info(Window *window, VkApplicationInfo& app_info);

	void show_available_extensions();
	void setup_debug_messegner();
	void pick_phys_device();
	void create_logical_device();
	void create_surface(Window *window);
	void create_swap_chain(Window *window);
	bool is_device_suitable(VkPhysicalDevice device);
	bool check_device_extensions_support(VkPhysicalDevice device);
	bool check_validation_layers_support();
	SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);
	VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D choose_swap_extent(Window *window, const VkSurfaceCapabilitiesKHR& capabilities);
	Vulkan::QueueFamilyIndices find_queue_families(VkPhysicalDevice device);
};

