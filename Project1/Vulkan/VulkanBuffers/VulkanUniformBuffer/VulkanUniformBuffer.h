#pragma once
#include "../VulkanBuffer/VulkanBuffer.h"

class VulkanUniformBuffer : public VulkanBuffer
{
public:
	VulkanUniformBuffer(VulkanInit* vulkan_init, bool local_device = true);
	~VulkanUniformBuffer();

	void create_buffer(const void* buffer_data, VkDeviceSize buffer_size) override;
	void update(const void* buffer_data, VkDeviceSize buffer_size) override;
};

