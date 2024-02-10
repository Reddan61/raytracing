#include "Vulkan.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT debugMessenger, 
    const VkAllocationCallbacks* pAllocator
) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

Vulkan::Vulkan(Window* window)
{
    this->show_available_extensions();

    VkApplicationInfo app_info = this->create_app_info();

    VkInstanceCreateInfo instance_info = this->create_instance_info(window, app_info);

    VkResult result = vkCreateInstance(&instance_info, nullptr, &this->instance);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance!");
    }

    this->setup_debug_messegner();

    this->create_surface(window);

    this->pick_phys_device();

    this->create_logical_device();

    this->create_swap_chain(window);
}

Vulkan::~Vulkan()
{
    vkDestroySwapchainKHR(this->logical_device, this->swap_chain, nullptr);

    vkDestroyDevice(this->logical_device, nullptr);

    DestroyDebugUtilsMessengerEXT(this->instance, this->debug_messenger, nullptr);

    vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
    
    vkDestroyInstance(this->instance, nullptr);
}

VkApplicationInfo Vulkan::create_app_info()
{
    VkApplicationInfo appInfo{};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    return appInfo;
}

VkInstanceCreateInfo Vulkan::create_instance_info(Window* window, VkApplicationInfo& app_info)
{
    if (!this->check_validation_layers_support()) {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &app_info;

    auto extensions = window->get_required_from_vulkan_extensions();

    createInfo.enabledExtensionCount = extensions.first;
    createInfo.ppEnabledExtensionNames = extensions.second;

    createInfo.enabledLayerCount = this->validation_layers.size();
    createInfo.ppEnabledLayerNames = this->validation_layers.data();

    this->debug_create_instance_info = VkDebugUtilsMessengerCreateInfoEXT{};

    populateDebugMessengerCreateInfo(this->debug_create_instance_info);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&this->debug_create_instance_info;

    return createInfo;
}

void Vulkan::show_available_extensions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "vulkan available extensions:\n";

    for (const VkExtensionProperties& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }
}

void Vulkan::setup_debug_messegner()
{
    this->debug_messenger_create_info = VkDebugUtilsMessengerCreateInfoEXT{};
    populateDebugMessengerCreateInfo(this->debug_messenger_create_info);

    if (CreateDebugUtilsMessengerEXT(
        this->instance, 
        &this->debug_messenger_create_info, 
        nullptr, 
        &this->debug_messenger
    )
        != VK_SUCCESS
    ) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void Vulkan::pick_phys_device()
{
    uint32_t devices_count = 0;
    vkEnumeratePhysicalDevices(this->instance, &devices_count, nullptr);

    if (devices_count == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(devices_count);
    vkEnumeratePhysicalDevices(this->instance, &devices_count, devices.data());

    for (const auto& device : devices) {
        if (this->is_device_suitable(device)) {
            this->phys_device = device;
            break;
        }
    }

    if (this->phys_device == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

void Vulkan::create_logical_device()
{
    QueueFamilyIndices indices = this->find_queue_families(this->phys_device);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { 
        indices.graphics_family.value(), 
        indices.present_family.value() 
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};

        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(this->device_extensions.size());
    createInfo.ppEnabledExtensionNames = this->device_extensions.data();

    if (vkCreateDevice(this->phys_device, &createInfo, nullptr, &this->logical_device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(this->logical_device, indices.graphics_family.value(), 0, &this->graphics_queue);
    vkGetDeviceQueue(this->logical_device, indices.present_family.value(), 0, &this->present_queue);
}

void Vulkan::create_surface(Window* window)
{
    if (glfwCreateWindowSurface(this->instance, window->_window, nullptr, &this->surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}

void Vulkan::create_swap_chain(Window* window)
{
    SwapChainSupportDetails swapChainSupport = this->query_swap_chain_support(this->phys_device);

    VkSurfaceFormatKHR surfaceFormat = this->choose_swap_surface_format(swapChainSupport.formats);
    VkPresentModeKHR presentMode = this->choose_swap_present_mode(swapChainSupport.present_modes);
    VkExtent2D extent = this->choose_swap_extent(window, swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (
        swapChainSupport.capabilities.maxImageCount > 0 && 
        imageCount > swapChainSupport.capabilities.maxImageCount
    ) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = this->surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = this->find_queue_families(this->phys_device);
    uint32_t queueFamilyIndices[] = { indices.graphics_family.value(), indices.present_family.value() };

    if (indices.graphics_family != indices.present_family) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(this->logical_device, &createInfo, nullptr, &this->swap_chain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(this->logical_device, this->swap_chain, &imageCount, nullptr);
    this->swap_ñhain_images.resize(imageCount);
    vkGetSwapchainImagesKHR(this->logical_device, this->swap_chain, &imageCount, this->swap_ñhain_images.data());

    this->swap_chain_image_format = surfaceFormat.format;
    this->swap_chain_extent = extent;
}

bool Vulkan::is_device_suitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = this->find_queue_families(device);

    bool extensionsSupported = this->check_device_extensions_support(device);

    bool swapChainAdequate = false;

    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = this->query_swap_chain_support(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool Vulkan::check_device_extensions_support(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(
        this->device_extensions.begin(), 
        this->device_extensions.end()
    );

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool Vulkan::check_validation_layers_support()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : this->validation_layers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

Vulkan::SwapChainSupportDetails Vulkan::query_swap_chain_support(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.present_modes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.present_modes.data());
    }

    return details;
}

VkSurfaceFormatKHR Vulkan::choose_swap_surface_format(
    const std::vector<VkSurfaceFormatKHR>& availableFormats
) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR Vulkan::choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Vulkan::choose_swap_extent(Window* window, const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        int width, height;
        glfwGetFramebufferSize(window->_window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

Vulkan::QueueFamilyIndices Vulkan::find_queue_families(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    int i = 0;

    for (const auto& queue_family : queue_families) {
        
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family = i;

        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        
        if (presentSupport) {
            indices.present_family = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}
