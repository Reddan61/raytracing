#include "VulkanLocalBuffer.h"

VulkanLocalBuffer::VulkanLocalBuffer(VulkanInit* vulkan_init, VkBufferUsageFlagBits usage) : VulkanBuffer(vulkan_init)
{
    this->usage = usage;
}

VulkanLocalBuffer::~VulkanLocalBuffer()
{
}

void VulkanLocalBuffer::create_buffer(const void* buffer_data, VkDeviceSize buffer_size)
{
    this->buffer_size = buffer_size;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    this->VulkanBuffer::_create_buffer(
        buffer_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory
    );

    void* data;
    vkMapMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory, 0, buffer_size, 0, &data);
    memcpy(data, buffer_data, buffer_size);
    vkUnmapMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory);

    this->VulkanBuffer::_create_buffer(
        buffer_size,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        this->buffer,
        this->buffer_memory
    );

    this->VulkanBuffer::_copy_buffer(stagingBuffer, this->buffer, buffer_size);

    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory, nullptr);
}

void VulkanLocalBuffer::update(const void* buffer_data, VkDeviceSize buffer_size, VkDeviceSize src_offset, VkDeviceSize dst_offset)
{
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    this->VulkanBuffer::_create_buffer(
        buffer_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory
    );

    void* data;
    vkMapMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory, 0, buffer_size, 0, &data);
        memcpy(data, buffer_data, buffer_size);
    vkUnmapMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory);

    this->_copy_buffer(stagingBuffer, this->buffer, buffer_size, src_offset, dst_offset);

    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory, nullptr);
}
