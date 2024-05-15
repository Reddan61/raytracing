#include "Vulkan.h"

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }
    
    size_t fileSize = (size_t)file.tellg();

    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

Vulkan::Vulkan(Window* window)
{
    this->vulkan_init = new VulkanInit(window);

    //graphics
    this->create_screen_images();
    this->create_screen_image_views();
    this->create_screen_image_sampler();
    this->create_graphics_descriptor_set_layout();
    this->create_graphics_descriptor_pool();
    this->create_graphics_descriptor_sets();
    this->create_graphics_pipe_line();
    
    //compute
    this->create_scene_buffer(window);
    this->create_spheres_buffer(window);
    this->create_triangles_buffer(window);
    this->create_bvhs_buffers(window);
    this->create_point_lights_buffer(window);
    this->create_compute_descriptor_set_layout();
    this->create_compute_descriptor_pool();
    this->create_compute_descriptor_sets(window);
    this->create_compute_pipe_line();

    this->create_command_buffers();
    this->create_compute_command_buffers();

    this->create_sync_objects();
}

Vulkan::~Vulkan()
{
    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), this->bvhs_origins_buffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), this->bvhs_origins_buffer_memory, nullptr); 
    
    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), this->bvhs_leaves_buffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), this->bvhs_leaves_buffer_memory, nullptr);

    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), this->triangles_buffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), this->triangles_buffer_memory, nullptr);

    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), this->point_lights_buffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), this->point_lights_buffer_memory, nullptr);

    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), this->scene_buffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), this->scene_buffer_memory, nullptr);

    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), this->spheres_buffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), this->spheres_buffer_memory, nullptr);

    for (size_t i = 0; i < this->MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyImage(this->vulkan_init->getLogicalDevice(), this->screen_images[i], nullptr);
        vkFreeMemory(this->vulkan_init->getLogicalDevice(), this->screen_image_memories[i], nullptr);
    }
    
    for (size_t i = 0; i < this->MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySampler(this->vulkan_init->getLogicalDevice(), this->screen_image_samplers[i], nullptr);
        vkDestroyImageView(this->vulkan_init->getLogicalDevice(), this->screen_image_views[i], nullptr);
    }

    vkDestroyDescriptorPool(this->vulkan_init->getLogicalDevice(), this->graphics_descriptor_pool, nullptr);
    vkDestroyDescriptorSetLayout(this->vulkan_init->getLogicalDevice(), this->graphics_descriptor_set_layout, nullptr);
    
    vkDestroyDescriptorPool(this->vulkan_init->getLogicalDevice(), this->compute_descriptor_pool, nullptr);
    vkDestroyDescriptorSetLayout(this->vulkan_init->getLogicalDevice(), this->compute_descriptor_set_layout, nullptr);

    vkDestroyPipeline(this->vulkan_init->getLogicalDevice(), this->graphics_pipeline, nullptr);
    vkDestroyPipelineLayout(this->vulkan_init->getLogicalDevice(), this->graphics_pipeline_layout, nullptr);

    vkDestroyPipeline(this->vulkan_init->getLogicalDevice(), this->compute_pipeline, nullptr);
    vkDestroyPipelineLayout(this->vulkan_init->getLogicalDevice(), this->compute_pipeline_layout, nullptr);
    
    for (size_t i = 0; i < this->MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(this->vulkan_init->getLogicalDevice(), this->image_available_semaphores[i], nullptr);
        vkDestroySemaphore(this->vulkan_init->getLogicalDevice(), this->render_finished_semaphores[i], nullptr);
        vkDestroySemaphore(this->vulkan_init->getLogicalDevice(), this->compute_finished_smaphores[i], nullptr);
        vkDestroyFence(this->vulkan_init->getLogicalDevice(), this->in_flight_fences[i], nullptr);
        vkDestroyFence(this->vulkan_init->getLogicalDevice(), this->compute_in_flight_fences[i], nullptr);
    }

    delete this->vulkan_init;
}

void Vulkan::create_graphics_pipe_line()
{
    auto vertShaderCode = readFile("./Vulkan/shaders/vert.spv");
    auto fragShaderCode = readFile("./Vulkan/shaders/frag.spv");

    VkShaderModule vertShaderModule = this->create_shader_module(vertShaderCode);
    VkShaderModule fragShaderModule = this->create_shader_module(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &this->graphics_descriptor_set_layout;

    if (vkCreatePipelineLayout(this->vulkan_init->getLogicalDevice(), &pipelineLayoutInfo, nullptr, &this->graphics_pipeline_layout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = this->graphics_pipeline_layout;
    pipelineInfo.renderPass = this->vulkan_init->getRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(this->vulkan_init->getLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->graphics_pipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(this->vulkan_init->getLogicalDevice(), fragShaderModule, nullptr);
    vkDestroyShaderModule(this->vulkan_init->getLogicalDevice(), vertShaderModule, nullptr);
}

void Vulkan::create_command_buffers()
{
    this->command_buffers.resize(this->MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = this->vulkan_init->getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)this->command_buffers.size();

    if (vkAllocateCommandBuffers(this->vulkan_init->getLogicalDevice(), &allocInfo, this->command_buffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}

void Vulkan::create_compute_command_buffers()
{
    this->compute_command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = this->vulkan_init->getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)this->compute_command_buffers.size();

    if (vkAllocateCommandBuffers(this->vulkan_init->getLogicalDevice(), &allocInfo, this->compute_command_buffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate compute command buffers!");
    }
}

void Vulkan::create_compute_descriptor_pool()
{
    std::array<VkDescriptorPoolSize, 7> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[3].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[3].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);    
    poolSizes[4].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[4].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);  
    poolSizes[5].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[5].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);   
    poolSizes[6].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[6].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(this->vulkan_init->getLogicalDevice(), &poolInfo, nullptr, &this->compute_descriptor_pool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool!");
    }
}

void Vulkan::record_graphics_command_buffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    this->transition_image_layout_with_buffer(
        commandBuffer,
        this->screen_images[this->current_frame],
        VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = this->vulkan_init->getRenderPass();
    renderPassInfo.framebuffer = this->vulkan_init->getSwapChainFrameBuffers()[imageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = this->vulkan_init->getSwapChainExtent();

    VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphics_pipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)this->vulkan_init->getSwapChainExtent().width;
        viewport.height = (float)this->vulkan_init->getSwapChainExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = this->vulkan_init->getSwapChainExtent();
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphics_pipeline_layout, 0, 1, &this->graphics_descriptor_sets[this->current_frame], 0, nullptr);
        
        vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    vkCmdEndRenderPass(commandBuffer);

    this->transition_image_layout_with_buffer(
        commandBuffer,
        this->screen_images[this->current_frame],
        VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL
    );

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}

void Vulkan::record_compute_command_buffer(VkCommandBuffer commandBuffer)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording compute command buffer!");
    }

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, this->compute_pipeline);

    vkCmdBindDescriptorSets(
        commandBuffer, 
        VK_PIPELINE_BIND_POINT_COMPUTE,
        this->compute_pipeline_layout, 
        0, 1, 
        &this->compute_descriptor_sets[current_frame], 0, 
        nullptr
    );

    vkCmdDispatch(
        commandBuffer, 
        static_cast<uint32_t>(this->vulkan_init->getSwapChainExtent().width / 8), 
        static_cast<uint32_t>(this->vulkan_init->getSwapChainExtent().height / 8), 
        1
    );

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record compute command buffer!");
    }
}


void Vulkan::create_sync_objects()
{
    this->image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    this->render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    this->compute_finished_smaphores.resize(MAX_FRAMES_IN_FLIGHT);
    this->in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
    this->compute_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(this->vulkan_init->getLogicalDevice(), &semaphoreInfo, nullptr, &this->image_available_semaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(this->vulkan_init->getLogicalDevice(), &semaphoreInfo, nullptr, &this->render_finished_semaphores[i]) != VK_SUCCESS ||
            vkCreateFence(this->vulkan_init->getLogicalDevice(), &fenceInfo, nullptr, &this->in_flight_fences[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create graphics synchronization objects for a frame!");
        }
        if (vkCreateSemaphore(this->vulkan_init->getLogicalDevice(), &semaphoreInfo, nullptr, &this->compute_finished_smaphores[i]) != VK_SUCCESS ||
            vkCreateFence(this->vulkan_init->getLogicalDevice(), &fenceInfo, nullptr, &this->compute_in_flight_fences[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create compute synchronization objects for a frame!");
        }
    }
}

void Vulkan::draw_frame(Window *window)
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    // Compute submission        
    vkWaitForFences(this->vulkan_init->getLogicalDevice(), 1, &compute_in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);

    this->update_scene_buffer(window);

    vkResetFences(this->vulkan_init->getLogicalDevice(), 1, &compute_in_flight_fences[current_frame]);

    vkResetCommandBuffer(this->compute_command_buffers[current_frame], /*VkCommandBufferResetFlagBits*/ 0);
    this->record_compute_command_buffer(this->compute_command_buffers[current_frame]);

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &this->compute_command_buffers[current_frame];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &compute_finished_smaphores[current_frame];

    if (vkQueueSubmit(this->vulkan_init->getComputeQueue(), 1, &submitInfo, this->compute_in_flight_fences[current_frame]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit compute command buffer!");
    };

    // Graphics submission
    vkWaitForFences(this->vulkan_init->getLogicalDevice(), 1, &this->in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(this->vulkan_init->getLogicalDevice(), this->vulkan_init->getSwapChain(), UINT64_MAX, this->image_available_semaphores[current_frame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        this->vulkan_init->recreate_swapchain(window);
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(this->vulkan_init->getLogicalDevice(), 1, &this->in_flight_fences[current_frame]);

    vkResetCommandBuffer(this->command_buffers[current_frame], /*VkCommandBufferResetFlagBits*/ 0);
    this->record_graphics_command_buffer(this->command_buffers[current_frame], imageIndex);

    VkSemaphore waitSemaphores[] = { 
        this->compute_finished_smaphores[current_frame],
        this->image_available_semaphores[current_frame] 
    }; 

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submitInfo.waitSemaphoreCount = 2;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &this->command_buffers[current_frame];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &this->render_finished_semaphores[current_frame];

    if (vkQueueSubmit(this->vulkan_init->getGraphicsQueue(), 1, &submitInfo, this->in_flight_fences[current_frame]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &this->render_finished_semaphores[current_frame];

    VkSwapchainKHR swapChains[] = { this->vulkan_init->getSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(this->vulkan_init->getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || this->framebuffer_resized) {
        this->framebuffer_resized = false;
        this->vulkan_init->recreate_swapchain(window);
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    this->current_frame = (this->current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

VkShaderModule Vulkan::create_shader_module(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(this->vulkan_init->getLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module!");
    }

    return shaderModule;
}

void Vulkan::create_scene_buffer(Window* window)
{
    auto scene = window->scene->getSceneBuffer();
    VkDeviceSize bufferSize = window->scene->getSceneBufferSize();

    //VkBuffer stagingBuffer;
    //VkDeviceMemory stagingBufferMemory;

    //this->create_buffer(
    //    bufferSize,
    //    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    //    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    //    stagingBuffer,
    //    stagingBufferMemory
    //);

    this->create_buffer(
        bufferSize,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        this->scene_buffer,
        this->scene_buffer_memory
    );

    vkMapMemory(this->vulkan_init->getLogicalDevice(), this->scene_buffer_memory, 0, bufferSize, 0, &this->scene_buffer_mapped);
}

void Vulkan::update_scene_buffer(Window* window)
{
    auto scene = window->scene->getSceneBuffer();
    auto scene_size = window->scene->getSceneBufferSize();

    memcpy(this->scene_buffer_mapped, &scene, scene_size);
}

void Vulkan::create_spheres_buffer(Window* window)
{
    auto spheres = window->scene->getSpheresBuffer();

    VkDeviceSize bufferSize = window->scene->getSpheresBufferSize();

    this->create_buffer(
        bufferSize,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        this->spheres_buffer,
        this->spheres_buffer_memory
    );

    void* data;
    vkMapMemory(this->vulkan_init->getLogicalDevice(), this->spheres_buffer_memory, 0, bufferSize, 0, &data);
        memcpy(data, spheres.data(), bufferSize);
    vkUnmapMemory(this->vulkan_init->getLogicalDevice(), this->spheres_buffer_memory);
}

void Vulkan::create_triangles_buffer(Window* window)
{
    auto triangles = window->scene->getTrianglesBuffer();

    VkDeviceSize bufferSize = window->scene->getTrianglesBufferSize();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    this->create_buffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory
    );

    void* data;
    vkMapMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, triangles.data(), bufferSize);
    vkUnmapMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory);

    this->create_buffer(
        bufferSize,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        this->triangles_buffer,
        this->triangles_buffer_memory
    );

    this->copy_buffer(stagingBuffer, this->triangles_buffer, bufferSize);

    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory, nullptr);
}

void Vulkan::create_bvhs_buffers(Window* window)
{
    auto bvh = window->scene->getSceneBVHsBuffer();

    VkBuffer stagingBuffer1;
    VkDeviceMemory stagingBufferMemory1;

    this->create_buffer(
        bvh.origin_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer1,
        stagingBufferMemory1
    );

    void* data;
    vkMapMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory1, 0, bvh.origin_size, 0, &data);
        memcpy(data, bvh.origins.data(), bvh.origin_size);
    vkUnmapMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory1);

    this->create_buffer(
        bvh.origin_size,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        this->bvhs_origins_buffer,
        this->bvhs_origins_buffer_memory
    );  


    this->copy_buffer(stagingBuffer1, this->bvhs_origins_buffer, bvh.origin_size);

    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), stagingBuffer1, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory1, nullptr);

    VkBuffer stagingBuffer2;
    VkDeviceMemory stagingBufferMemory2;

    this->create_buffer(
        bvh.leaves_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer2,
        stagingBufferMemory2
    );

    void* data1;
    vkMapMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory2, 0, bvh.leaves_size, 0, &data1);
        memcpy(data1, bvh.leaves.data(), bvh.leaves_size);
    vkUnmapMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory2);

    this->create_buffer(
        bvh.leaves_size,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        this->bvhs_leaves_buffer,
        this->bvhs_leaves_buffer_memory
    );

    this->copy_buffer(stagingBuffer2, this->bvhs_leaves_buffer, bvh.leaves_size);

    vkDestroyBuffer(this->vulkan_init->getLogicalDevice(), stagingBuffer2, nullptr);
    vkFreeMemory(this->vulkan_init->getLogicalDevice(), stagingBufferMemory2, nullptr);
}

void Vulkan::create_point_lights_buffer(Window* window)
{
    auto pointLights = window->scene->getPointLightsBuffer();

    VkDeviceSize bufferSize = window->scene->getPointLightsBufferSize();

    this->create_buffer(
        bufferSize,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        this->point_lights_buffer,
        this->point_lights_buffer_memory
    );

    void* data;
    vkMapMemory(this->vulkan_init->getLogicalDevice(), this->point_lights_buffer_memory, 0, bufferSize, 0, &data);
        memcpy(data, pointLights.data(), bufferSize);
    vkUnmapMemory(this->vulkan_init->getLogicalDevice(), this->point_lights_buffer_memory);
}

void Vulkan::set_framebuffer_resized(bool isResized)
{
    this->framebuffer_resized = isResized;
}

uint32_t Vulkan::find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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

void Vulkan::copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = this->begin_single_time_commands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
  
    this->end_single_time_commands(commandBuffer);
}

void Vulkan::create_graphics_descriptor_pool()
{
    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(this->vulkan_init->getLogicalDevice(), &poolInfo, nullptr, &this->graphics_descriptor_pool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create graphics descriptor pool!");
    }
}

void Vulkan::create_graphics_descriptor_set_layout()
{
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 0;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 1> bindings = { 
        samplerLayoutBinding 
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(this->vulkan_init->getLogicalDevice(), &layoutInfo, nullptr, &this->graphics_descriptor_set_layout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create graphics descriptor set layout!");
    }
}

void Vulkan::create_graphics_descriptor_sets()
{
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, this->graphics_descriptor_set_layout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = this->graphics_descriptor_pool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    this->graphics_descriptor_sets.resize(MAX_FRAMES_IN_FLIGHT);

    if (vkAllocateDescriptorSets(this->vulkan_init->getLogicalDevice(), &allocInfo, this->graphics_descriptor_sets.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate graphics descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = this->screen_image_views[i];
        imageInfo.sampler = this->screen_image_samplers[i];

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = this->graphics_descriptor_sets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(
            this->vulkan_init->getLogicalDevice(), 
            static_cast<uint32_t>(descriptorWrites.size()),
            descriptorWrites.data(), 
            0,
            nullptr
        );
    }
}

void Vulkan::create_compute_pipe_line()
{
    auto computeShaderCode = readFile("./Vulkan/shaders/compute.spv");

    VkShaderModule computeShaderModule = this->create_shader_module(computeShaderCode);

    VkPipelineShaderStageCreateInfo computeShaderStageInfo{};
    computeShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    computeShaderStageInfo.module = computeShaderModule;
    computeShaderStageInfo.pName = "main";

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &this->compute_descriptor_set_layout;

    if (vkCreatePipelineLayout(this->vulkan_init->getLogicalDevice(), &pipelineLayoutInfo, nullptr, &this->compute_pipeline_layout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute pipeline layout!");
    }

    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.layout = this->compute_pipeline_layout;
    pipelineInfo.stage = computeShaderStageInfo;

    if (vkCreateComputePipelines(this->vulkan_init->getLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->compute_pipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute pipeline!");
    }

    vkDestroyShaderModule(this->vulkan_init->getLogicalDevice(), computeShaderModule, nullptr);
}

void Vulkan::create_image(
    uint32_t width, uint32_t height, 
    VkFormat format, VkImageTiling tiling, 
    VkImageUsageFlags usage, VkMemoryPropertyFlags properties, 
    VkImage& image, VkDeviceMemory& imageMemory
)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(width);
    imageInfo.extent.height = static_cast<uint32_t>(height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    if (vkCreateImage(this->vulkan_init->getLogicalDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(this->vulkan_init->getLogicalDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = this->find_memory_type(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(this->vulkan_init->getLogicalDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate image memory!");
    }

    vkBindImageMemory(this->vulkan_init->getLogicalDevice(), image, imageMemory, 0);
}

void Vulkan::create_screen_images()
{
    this->screen_images.resize(this->MAX_FRAMES_IN_FLIGHT);
    this->screen_image_memories.resize(this->MAX_FRAMES_IN_FLIGHT);

    for (int i = 0; i < this->MAX_FRAMES_IN_FLIGHT; i++) {
        this->create_image(
            this->vulkan_init->getSwapChainExtent().width, 
            this->vulkan_init->getSwapChainExtent().height,
            VK_FORMAT_R8G8B8A8_UNORM, 
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            this->screen_images[i], this->screen_image_memories[i]
        );

        this->transition_image_layout(
            this->screen_images[i],
            VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL
        );
    }
}

void Vulkan::create_screen_image_sampler()
{
    this->screen_image_samplers.resize(this->MAX_FRAMES_IN_FLIGHT);

    for (int i = 0; i < this->MAX_FRAMES_IN_FLIGHT; i++) {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(this->vulkan_init->getLogicalDevice(), &samplerInfo, nullptr, &this->screen_image_samplers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create texture sampler!");
        }
    }
}

void Vulkan::create_screen_image_views()
{
    this->screen_image_views.resize(this->MAX_FRAMES_IN_FLIGHT);

    for (int i = 0; i < this->MAX_FRAMES_IN_FLIGHT; i++) {
        this->screen_image_views[i] = this->vulkan_init->create_image_view(this->screen_images[i], VK_FORMAT_R8G8B8A8_UNORM);
    }
}

void Vulkan::transition_image_layout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = this->begin_single_time_commands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
    }
    else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    this->end_single_time_commands(commandBuffer);
}

void Vulkan::transition_image_layout_with_buffer(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_GENERAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_GENERAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );
}


void Vulkan::copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = this->begin_single_time_commands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    this->end_single_time_commands(commandBuffer);
}

VkCommandBuffer Vulkan::begin_single_time_commands()
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

void Vulkan::end_single_time_commands(VkCommandBuffer commandBuffer)
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

void Vulkan::create_compute_descriptor_set_layout()
{
    std::array<VkDescriptorSetLayoutBinding, 7> layoutBindings{};

    layoutBindings[0].binding = 0;
    layoutBindings[0].descriptorCount = 1;
    layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    layoutBindings[0].pImmutableSamplers = nullptr;
    layoutBindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    layoutBindings[1].binding = 1;
    layoutBindings[1].descriptorCount = 1;
    layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layoutBindings[1].pImmutableSamplers = nullptr;
    layoutBindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    layoutBindings[2].binding = 2;
    layoutBindings[2].descriptorCount = 1;
    layoutBindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBindings[2].pImmutableSamplers = nullptr;
    layoutBindings[2].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    layoutBindings[3].binding = 3;
    layoutBindings[3].descriptorCount = 1;
    layoutBindings[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layoutBindings[3].pImmutableSamplers = nullptr;
    layoutBindings[3].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;    
    
    layoutBindings[4].binding = 4;
    layoutBindings[4].descriptorCount = 1;
    layoutBindings[4].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layoutBindings[4].pImmutableSamplers = nullptr;
    layoutBindings[4].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    layoutBindings[5].binding = 5;
    layoutBindings[5].descriptorCount = 1;
    layoutBindings[5].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layoutBindings[5].pImmutableSamplers = nullptr;
    layoutBindings[5].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    layoutBindings[6].binding = 6;
    layoutBindings[6].descriptorCount = 1;
    layoutBindings[6].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layoutBindings[6].pImmutableSamplers = nullptr;
    layoutBindings[6].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = layoutBindings.size();
    layoutInfo.pBindings = layoutBindings.data();

    if (vkCreateDescriptorSetLayout(this->vulkan_init->getLogicalDevice(), &layoutInfo, nullptr, &this->compute_descriptor_set_layout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute descriptor set layout!");
    }
}

void Vulkan::create_compute_descriptor_sets(Window *window) {
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, this->compute_descriptor_set_layout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = this->compute_descriptor_pool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    this->compute_descriptor_sets.resize(MAX_FRAMES_IN_FLIGHT);

    if (vkAllocateDescriptorSets(this->vulkan_init->getLogicalDevice(), &allocInfo, this->compute_descriptor_sets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        std::array<VkWriteDescriptorSet, 7> descriptorWrites{};

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
        imageInfo.imageView = this->screen_image_views[i];

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = this->compute_descriptor_sets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &imageInfo;

        VkDescriptorBufferInfo spheresBufferInfo{};
        spheresBufferInfo.buffer = this->spheres_buffer;
        spheresBufferInfo.offset = 0;
        spheresBufferInfo.range = window->scene->getSpheresBufferSize();

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = this->compute_descriptor_sets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pBufferInfo = &spheresBufferInfo;

        VkDescriptorBufferInfo sceneBufferInfo{};
        sceneBufferInfo.buffer = this->scene_buffer;
        sceneBufferInfo.offset = 0;
        sceneBufferInfo.range = window->scene->getSceneBufferSize();

        descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[2].dstSet = this->compute_descriptor_sets[i];
        descriptorWrites[2].dstBinding = 2;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pBufferInfo = &sceneBufferInfo;

        VkDescriptorBufferInfo pointLightsBufferInfo{};
        pointLightsBufferInfo.buffer = this->point_lights_buffer;
        pointLightsBufferInfo.offset = 0;
        pointLightsBufferInfo.range = window->scene->getPointLightsBufferSize();

        descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[3].dstSet = this->compute_descriptor_sets[i];
        descriptorWrites[3].dstBinding = 3;
        descriptorWrites[3].dstArrayElement = 0;
        descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWrites[3].descriptorCount = 1;
        descriptorWrites[3].pBufferInfo = &pointLightsBufferInfo;

        VkDescriptorBufferInfo trianglesBufferInfo{};
        trianglesBufferInfo.buffer = this->triangles_buffer;
        trianglesBufferInfo.offset = 0;
        trianglesBufferInfo.range = window->scene->getTrianglesBufferSize();

        descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[4].dstSet = this->compute_descriptor_sets[i];
        descriptorWrites[4].dstBinding = 4;
        descriptorWrites[4].dstArrayElement = 0;
        descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWrites[4].descriptorCount = 1;
        descriptorWrites[4].pBufferInfo = &trianglesBufferInfo; 

        auto bvh = window->scene->getSceneBVHsBuffer();
        
        VkDescriptorBufferInfo bvhOriginsBufferInfo{};
        bvhOriginsBufferInfo.buffer = this->bvhs_origins_buffer;
        bvhOriginsBufferInfo.offset = 0;
        bvhOriginsBufferInfo.range = bvh.origin_size;

        descriptorWrites[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[5].dstSet = this->compute_descriptor_sets[i];
        descriptorWrites[5].dstBinding = 5;
        descriptorWrites[5].dstArrayElement = 0;
        descriptorWrites[5].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWrites[5].descriptorCount = 1;
        descriptorWrites[5].pBufferInfo = &bvhOriginsBufferInfo;  
        
        VkDescriptorBufferInfo bvhLeavesBufferInfo{};
        bvhLeavesBufferInfo.buffer = this->bvhs_leaves_buffer;
        bvhLeavesBufferInfo.offset = 0;
        bvhLeavesBufferInfo.range = bvh.leaves_size;

        descriptorWrites[6].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[6].dstSet = this->compute_descriptor_sets[i];
        descriptorWrites[6].dstBinding = 6;
        descriptorWrites[6].dstArrayElement = 0;
        descriptorWrites[6].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWrites[6].descriptorCount = 1;
        descriptorWrites[6].pBufferInfo = &bvhLeavesBufferInfo;

        vkUpdateDescriptorSets(
            this->vulkan_init->getLogicalDevice(),
            static_cast<uint32_t>(descriptorWrites.size()),
            descriptorWrites.data(),
            0,
            nullptr
        );

        vkUpdateDescriptorSets(
            this->vulkan_init->getLogicalDevice(), 
            static_cast<uint32_t>(descriptorWrites.size()), 
            descriptorWrites.data(), 0, nullptr
        );
    }
}

void Vulkan::create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
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
