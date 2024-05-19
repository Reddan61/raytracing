#include "VulkanBuffer.h"

VulkanBuffer::VulkanBuffer(VulkanInit* vulkan_init, bool local_device)
{
    this->vulkan_init = vulkan_init;
    this->local_device = local_device;
}

VulkanBuffer::~VulkanBuffer()
{
    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), this->buffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), this->buffer_memory, nullptr);
}

VkBuffer VulkanBuffer::getBuffer()
{
    return this->buffer;
}

VkDeviceSize VulkanBuffer::getSize()
{
    return this->buffer_size;
}

void VulkanBuffer::_create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(this->vulkan_init->getLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(this->vulkan_init->getLogicalDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = this->find_memory_type(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(this->vulkan_init->getLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(this->vulkan_init->getLogicalDevice(), buffer, bufferMemory, 0);
}

void VulkanBuffer::_copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = this->begin_single_time_commands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    this->end_single_time_commands(commandBuffer);
}

void VulkanBuffer::_create_local_buffer(const void* buffer_data, VkDeviceSize buffer_size, VkBufferUsageFlags usage)
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
        usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        this->buffer,
        this->buffer_memory
    );

    this->VulkanBuffer::_copy_buffer(stagingBuffer, this->buffer, buffer_size);

    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory, nullptr);
}

void VulkanBuffer::_create_host_buffer(const void* buffer_data, VkDeviceSize buffer_size, VkBufferUsageFlags usage)
{
    this->buffer_size = buffer_size;

    this->VulkanBuffer::_create_buffer(
        buffer_size,
        usage,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        this->buffer,
        this->buffer_memory
    );

    vkMapMemory(this->vulkan_init->getLogicalDevice(), this->buffer_memory, 0, buffer_size, 0, &this->buffer_mapped);
        memcpy(this->buffer_mapped, buffer_data, buffer_size);
}

uint32_t VulkanBuffer::find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(this->vulkan_init->getPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

VkCommandBuffer VulkanBuffer::begin_single_time_commands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = this->vulkan_init->getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(this->vulkan_init->getLogicalDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VulkanBuffer::end_single_time_commands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(this->vulkan_init->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(this->vulkan_init->getGraphicsQueue());

    vkFreeCommandBuffers(this->vulkan_init->getLogicalDevice(), this->vulkan_init->getCommandPool(), 1, &commandBuffer);
}
