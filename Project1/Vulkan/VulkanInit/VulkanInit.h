#pragma once
#include <vector>
#include <optional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <set>
#include <algorithm>
#include "../../Config.h"

class VulkanInit
{
public:
	VulkanInit(GLFWwindow* window);
	~VulkanInit();

	VkDevice getLogicalDevice();
	VkPhysicalDevice getPhysicalDevice();
	VkRenderPass getRenderPass();
	VkSwapchainKHR getSwapChain();
	VkExtent2D getSwapChainExtent();
	VkCommandPool getCommandPool();
	VkQueue getComputeQueue();
	VkQueue getGraphicsQueue();
	VkQueue getPresentQueue();
	std::vector<VkImageView> getSwapChainImageViews();
	std::vector<VkFramebuffer> getSwapChainFrameBuffers();

	void recreate_swapchain(GLFWwindow* window);
	VkImageView create_image_view(VkImage image, VkFormat format);
private:
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphics_and_compute_family;
		std::optional<uint32_t> present_family;

		bool isComplete() {
			return graphics_and_compute_family.has_value() && present_family.has_value();
		}
	};
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> present_modes;
	};

	const std::vector<const char*> validation_layers = {
		"VK_LAYER_KHRONOS_validation"
	};
	const std::vector<const char*> device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	VkInstance instance;
	VkDebugUtilsMessengerEXT debug_messenger;
	VkDebugUtilsMessengerCreateInfoEXT debug_create_instance_info;
	VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;
	VkSurfaceKHR surface;
	VkPhysicalDevice phys_device;
	VkDevice logical_device;
	VkQueue graphics_queue;
	VkQueue present_queue;
	VkQueue compute_queue;
	VkSwapchainKHR swap_chain;
	std::vector<VkImage> swap_ñhain_images;
	VkFormat swap_chain_image_format;
	VkExtent2D swap_chain_extent;
	std::vector<VkImageView> swap_chain_image_views;
	VkRenderPass render_pass;
	VkCommandPool command_pool;
	std::vector<VkFramebuffer> swap_chain_framebuffers;

	VkApplicationInfo create_app_info();
	VkInstanceCreateInfo create_instance_info(VkApplicationInfo& app_info);
	bool check_validation_layers_support();
	void setup_debug_messegner();
	void create_surface(GLFWwindow* window);
	void pick_phys_device();
	bool is_device_suitable(VkPhysicalDevice device);
	VulkanInit::QueueFamilyIndices find_queue_families(VkPhysicalDevice device);
	bool check_device_extensions_support(VkPhysicalDevice device);
	VulkanInit::SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);
	void create_logical_device();
	VkSurfaceFormatKHR choose_swap_surface_format(
		const std::vector<VkSurfaceFormatKHR>& availableFormats
	);
	VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D choose_swap_extent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);
	void create_render_pass();
	void create_command_pool();
	void show_available_extensions();
	void create_framebuffers();
	void cleanup_swapchain();
	void create_swap_chain(GLFWwindow* window);
	void create_image_views();

	std::pair<uint32_t, const char**> get_required_vulkan_extensions();
};

