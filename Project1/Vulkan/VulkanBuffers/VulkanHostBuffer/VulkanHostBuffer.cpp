#include "VulkanHostBuffer.h"

VulkanHostBuffer::VulkanHostBuffer(VulkanInit* vulkan_init, VkBufferUsageFlagBits usage) : VulkanBuffer(vulkan_init, usage)
{
}

VulkanHostBuffer::~VulkanHostBuffer()
{
}

void VulkanHostBuffer::create_buffer(const void* buffer_data, VkDeviceSize buffer_size)
{
    this->buffer_size = buffer_size;

    this->VulkanBuffer::_create_buffer(
        buffer_size,
        this->usage,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        this->buffer,
        this->buffer_memory
    );

    vkMapMemory(this->vulkan_init->getLogicalDevice(), this->buffer_memory, 0, buffer_size, 0, &this->buffer_mapped);
        memcpy(this->buffer_mapped, buffer_data, buffer_size);
}

void VulkanHostBuffer::update(const void* buffer_data, VkDeviceSize buffer_size, VkDeviceSize src_offset, VkDeviceSize dst_offset)
{
    memcpy(this->buffer_mapped, buffer_data, buffer_size);
}
