#pragma once
#include "../VulkanBuffer/VulkanBuffer.h"

class VulkanLocalBuffer : public VulkanBuffer
{
public:
	VulkanLocalBuffer(VulkanInit* vulkan_init, VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
	~VulkanLocalBuffer();

	void create_buffer(const void *buffer_data, VkDeviceSize buffer_size) override;
	void update(const void* buffer_data, VkDeviceSize buffer_size, VkDeviceSize src_offset, VkDeviceSize dst_offset) override;
private:
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VkBufferUsageFlagBits usage;
};
