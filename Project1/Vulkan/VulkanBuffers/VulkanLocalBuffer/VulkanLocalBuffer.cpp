#include "VulkanLocalBuffer.h"

VulkanLocalBuffer::VulkanLocalBuffer(VulkanInit* vulkan_init, VkBufferUsageFlagBits usage) : VulkanBuffer(vulkan_init, usage)
{
}

VulkanLocalBuffer::~VulkanLocalBuffer()
{
    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), this->stagingBuffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), this->stagingBufferMemory, nullptr);
}

void VulkanLocalBuffer::create_buffer(const void* buffer_data, VkDeviceSize buffer_size)
{
    this->buffer_size = buffer_size;

    this->VulkanBuffer::_create_buffer(
        buffer_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        this->stagingBuffer,
        this->stagingBufferMemory
    );

    void* data;
    vkMapMemory(this->vulkan_init->getLogicalDevice(), this->stagingBufferMemory, 0, this->buffer_size, 0, &data);
        memcpy(data, buffer_data, this->buffer_size);
    vkUnmapMemory(this->vulkan_init->getLogicalDevice(), this->stagingBufferMemory);

    this->VulkanBuffer::_create_buffer(
        this->buffer_size,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        this->buffer,
        this->buffer_memory
    );

    this->VulkanBuffer::_copy_buffer(stagingBuffer, this->buffer, buffer_size);
}

void VulkanLocalBuffer::update(const void* buffer_data, VkDeviceSize buffer_size, VkDeviceSize src_offset, VkDeviceSize dst_offset)
{
    void* data;
    vkMapMemory(this->vulkan_init->getLogicalDevice(), this->stagingBufferMemory, src_offset, buffer_size, 0, &data);
        memcpy(data, buffer_data, buffer_size);
    vkUnmapMemory(this->vulkan_init->getLogicalDevice(), this->stagingBufferMemory);

    this->_copy_buffer(stagingBuffer, this->buffer, buffer_size, src_offset, dst_offset);
}
