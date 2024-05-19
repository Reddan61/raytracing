#include "VulkanStorageBuffer.h"

VulkanStorageBuffer::VulkanStorageBuffer(VulkanInit* vulkan_init, bool local_device) : VulkanBuffer(vulkan_init, local_device)
{
}

VulkanStorageBuffer::~VulkanStorageBuffer()
{
}

void VulkanStorageBuffer::create_buffer(const void* buffer_data, VkDeviceSize buffer_size)
{
    if (this->local_device) {
        this->VulkanBuffer::_create_local_buffer(buffer_data, buffer_size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    }
    else {
        this->VulkanBuffer::_create_host_buffer(buffer_data, buffer_size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    }
}

void VulkanStorageBuffer::update(const void* buffer_data, VkDeviceSize buffer_size)
{
}
