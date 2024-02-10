#include "Window.h"

Window::Window(const uint32_t WIDTH, const uint32_t HEIGHT)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);

    Vulkan vulkan(this);
}

Window::~Window()
{
    this->glfw_clean_up();
}

void Window::run()
{
    while (!glfwWindowShouldClose(this->_window)) {
        glfwPollEvents();
    }
}

void Window::glfw_clean_up()
{
    glfwDestroyWindow(this->_window);
    glfwTerminate();
}

std::pair<uint32_t, const char**> Window::get_required_from_vulkan_extensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    const short custom_extenstions_count = 1;

    const char* custom_extenstions[custom_extenstions_count] = { 
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME 
    };

    uint32_t glfwExtensionCountWithCustom = glfwExtensionCount + custom_extenstions_count;

    const char** result = new const char*[glfwExtensionCountWithCustom];

    for (int i = 0; i < glfwExtensionCount; i++) {
        result[i] = glfwExtensions[i];
    }

    for (int i = 0; i < custom_extenstions_count; i++) {
        result[i + glfwExtensionCount] = custom_extenstions[i];
    }

    return std::pair<uint32_t, const char**>(glfwExtensionCountWithCustom, result);
}
