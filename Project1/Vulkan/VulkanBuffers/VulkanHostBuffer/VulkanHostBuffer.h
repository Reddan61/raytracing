#pragma once
#include "../VulkanBuffer/VulkanBuffer.h"

class VulkanHostBuffer : public VulkanBuffer
{
public:
	VulkanHostBuffer(VulkanInit* vulkan_init, VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
	~VulkanHostBuffer();

	void create_buffer(const void* buffer_data, VkDeviceSize buffer_size) override;
	void update(const void* buffer_data, VkDeviceSize buffer_size, VkDeviceSize src_offset, VkDeviceSize dst_offset) override;
private:
	void* buffer_mapped = nullptr;
};

