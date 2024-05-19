#include "VulkanUniformBuffer.h"

VulkanUniformBuffer::VulkanUniformBuffer(VulkanInit* vulkan_init, bool local_device) : VulkanBuffer(vulkan_init, local_device)
{
}

VulkanUniformBuffer::~VulkanUniformBuffer()
{
}

void VulkanUniformBuffer::create_buffer(const void* buffer_data, VkDeviceSize buffer_size)
{
    if (this->local_device) {
        this->VulkanBuffer::_create_local_buffer(buffer_data, buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    }
    else {
        this->VulkanBuffer::_create_host_buffer(buffer_data, buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    }
}

void VulkanUniformBuffer::update(const void* buffer_data, VkDeviceSize buffer_size)
{
    if (this->local_device) {

    }
    else {
        memcpy(this->buffer_mapped, buffer_data, buffer_size);
    }
}
