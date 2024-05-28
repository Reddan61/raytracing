#pragma once
#include "../../VulkanInit/VulkanInit.h"

class VulkanBuffer
{
public:
	VulkanBuffer(VulkanInit* vulkan_init, VkBufferUsageFlagBits usage);
	~VulkanBuffer();

	VkBuffer getBuffer();
	VkDeviceSize getSize();

	virtual void create_buffer(const void* buffer_data, VkDeviceSize buffer_size) = 0;
	virtual void update(const void* buffer_data, VkDeviceSize buffer_size, VkDeviceSize src_offset, VkDeviceSize dst_offset) = 0;
protected:
	VulkanInit* vulkan_init = nullptr;
	VkBuffer buffer;
	VkDeviceMemory buffer_memory;
	VkDeviceSize buffer_size;
	VkBufferUsageFlagBits usage;

	void _create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void _copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize src_offset = 0, VkDeviceSize dst_offset = 0);

	uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkCommandBuffer begin_single_time_commands();
	void end_single_time_commands(VkCommandBuffer commandBuffer);
};

