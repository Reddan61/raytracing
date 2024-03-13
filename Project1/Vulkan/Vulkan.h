#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <array>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>
#include <fstream>
#include <random>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#include "../Window/Window.h"

class Window;

class Vulkan
{
friend class Window;

public:
	Vulkan(Window *window);
	~Vulkan();
private:
	const int MAX_FRAMES_IN_FLIGHT = 2;
	uint32_t current_frame = 0;

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

	bool framebuffer_resized = false;

	// init
	VkInstance instance;
	VkPhysicalDevice phys_device;
	VkDevice logical_device;
	VkSurfaceKHR surface;
	VkSwapchainKHR swap_chain;
	VkRenderPass render_pass;
	VkPipeline graphics_pipeline;
	VkCommandPool command_pool;
	std::vector<VkImage> swap_ñhain_images;
	VkFormat swap_chain_image_format;
	VkExtent2D swap_chain_extent;
	std::vector<VkImageView> swap_chain_image_views;
	std::vector<VkFramebuffer> swap_chain_framebuffers;
	VkDebugUtilsMessengerCreateInfoEXT debug_create_instance_info;
	VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;
	VkDebugUtilsMessengerEXT debug_messenger;
	const std::vector<const char*> validation_layers = {
		"VK_LAYER_KHRONOS_validation"
	};
	const std::vector<const char*> device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	void show_available_extensions();
	void setup_debug_messegner();
	void pick_phys_device();
	void create_logical_device();
	void create_surface(Window* window);
	void create_swap_chain(Window* window);
	void create_image_views();
	void create_render_pass();
	void create_framebuffers();
	void create_command_pool();
	void create_command_buffers();
	void cleanup_swapchain();
	void set_framebuffer_resized(bool isResized);
	bool is_device_suitable(VkPhysicalDevice device);
	bool check_device_extensions_support(VkPhysicalDevice device);
	bool check_validation_layers_support();
	VkApplicationInfo create_app_info();
	VkInstanceCreateInfo create_instance_info(Window* window, VkApplicationInfo& app_info);

	// compute
	VkQueue compute_queue;
	VkDescriptorPool compute_descriptor_pool;
	std::vector<VkDescriptorSet> compute_descriptor_sets;
	VkDescriptorSetLayout compute_descriptor_set_layout;
	VkPipelineLayout compute_pipeline_layout;
	VkPipeline compute_pipeline;
	std::vector<VkCommandBuffer> compute_command_buffers;
	void create_compute_descriptor_pool();
	void create_compute_descriptor_set_layout();
	void create_compute_descriptor_sets();
	void record_compute_command_buffer(VkCommandBuffer commandBuffer);
	void create_compute_command_buffers();
	void create_compute_pipe_line();

	// graphics
	VkQueue graphics_queue;
	VkQueue present_queue;
	VkPipelineLayout graphics_pipeline_layout;
	VkDescriptorPool graphics_descriptor_pool;
	VkDescriptorSetLayout graphics_descriptor_set_layout;
	std::vector<VkDescriptorSet> graphics_descriptor_sets;
	std::vector<VkCommandBuffer> command_buffers;
	std::vector<VkImage> screen_images;
	std::vector<VkDeviceMemory> screen_image_memories;
	std::vector<VkImageView> screen_image_views;
	std::vector<VkSampler> screen_image_samplers;
	void create_graphics_descriptor_set_layout();
	void create_graphics_descriptor_pool();
	void create_graphics_descriptor_sets();
	void record_graphics_command_buffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void create_screen_images();
	void create_screen_image_views();
	void create_screen_image_sampler();
	void create_graphics_pipe_line();

	// semaphores & fences
	std::vector<VkSemaphore> image_available_semaphores;
	std::vector<VkSemaphore> render_finished_semaphores;
	std::vector<VkSemaphore> compute_finished_smaphores;
	std::vector<VkFence> in_flight_fences;
	std::vector<VkFence> compute_in_flight_fences;
	void create_sync_objects();

	void draw_frame(Window* window);
	void recreate_swapchain(Window *window);

	// utils
	SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);
	VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D choose_swap_extent(Window *window, const VkSurfaceCapabilitiesKHR& capabilities);
	Vulkan::QueueFamilyIndices find_queue_families(VkPhysicalDevice device);
	VkShaderModule create_shader_module(const std::vector<char>& code);
	uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void create_image(uint32_t width, uint32_t height,
		VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
		VkImage& image, VkDeviceMemory& imageMemory
	);
	VkImageView create_image_view(VkImage image, VkFormat format);
	VkCommandBuffer begin_single_time_commands();
	void end_single_time_commands(VkCommandBuffer commandBuffer);
	void transition_image_layout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void transition_image_layout_with_buffer(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};

