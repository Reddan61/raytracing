#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>
#include <fstream>
#include <random>
#include <chrono>

#include "./VulkanInit/VulkanInit.h"

class Window;
class VulkanInit;

class Vulkan
{
friend class Window;

public:
	Vulkan(Window *window);
	~Vulkan();
private:
	const int MAX_FRAMES_IN_FLIGHT = 2;
	uint32_t current_frame = 0;

	bool framebuffer_resized = false;

	VulkanInit* vulkan_init = nullptr;

	VkPipeline graphics_pipeline;
	void create_command_buffers();
	void set_framebuffer_resized(bool isResized);

	VkDescriptorPool compute_descriptor_pool;
	std::vector<VkDescriptorSet> compute_descriptor_sets;
	VkDescriptorSetLayout compute_descriptor_set_layout;
	VkPipelineLayout compute_pipeline_layout;
	VkPipeline compute_pipeline;
	std::vector<VkCommandBuffer> compute_command_buffers;

	VkBuffer spheres_buffer;
	VkDeviceMemory spheres_buffer_memory;
	void create_spheres_buffer(Window* window);

	VkBuffer triangles_buffer;
	VkDeviceMemory triangles_buffer_memory;
	void create_triangles_buffer(Window* window);	
	
	VkBuffer bvhs_origins_buffer;
	VkDeviceMemory bvhs_origins_buffer_memory;
	VkBuffer bvhs_leaves_buffer;
	VkDeviceMemory bvhs_leaves_buffer_memory;
	void create_bvhs_buffers(Window* window);

	VkBuffer point_lights_buffer;
	VkDeviceMemory point_lights_buffer_memory;
	void create_point_lights_buffer(Window* window);
		
	VkBuffer scene_buffer;
	VkDeviceMemory scene_buffer_memory;
	void* scene_buffer_mapped;
	void create_scene_buffer(Window* window);
	void update_scene_buffer(Window* window);

	void create_compute_descriptor_pool();
	void create_compute_descriptor_set_layout();
	void create_compute_descriptor_sets(Window *window);
	void record_compute_command_buffer(VkCommandBuffer commandBuffer);
	void create_compute_command_buffers();
	void create_compute_pipe_line();

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

	// utils
	VkShaderModule create_shader_module(const std::vector<char>& code);
	uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void create_image(uint32_t width, uint32_t height,
		VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
		VkImage& image, VkDeviceMemory& imageMemory
	);
	VkCommandBuffer begin_single_time_commands();
	void end_single_time_commands(VkCommandBuffer commandBuffer);
	void transition_image_layout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void transition_image_layout_with_buffer(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};

