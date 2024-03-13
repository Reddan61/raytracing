#include "Window.h"

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->on_resize();
}

Window::Window(const uint32_t WIDTH, const uint32_t HEIGHT)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    this->_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);
    glfwSetWindowUserPointer(this->_window, this);
    glfwSetFramebufferSizeCallback(this->_window, framebufferResizeCallback);

    this->vulkan = new Vulkan(this);
}

Window::~Window()
{
    this->glfw_clean_up();
    delete this->vulkan;
}

void Window::run()
{
    while (!glfwWindowShouldClose(this->_window)) {
        glfwPollEvents();
        this->draw();
        this->calculate_fps();
    }

    vkDeviceWaitIdle(this->vulkan->logical_device);
}

void Window::glfw_clean_up()
{
    glfwDestroyWindow(this->_window);
    glfwTerminate();
}

void Window::draw()
{
    if (this->vulkan != nullptr) {
        this->vulkan->draw_frame(this);
    }
}

void Window::on_resize()
{
    this->vulkan->set_framebuffer_resized(true);
}

void Window::calculate_fps()
{
    this->current_time = glfwGetTime();
    double delta = this->current_time - this->last_time;

    if (delta >= 1) {
        int framerate = std::max(1, int(this->num_frames / delta));
        std::stringstream title;

        title << "Running at " << framerate << " fps.";

        glfwSetWindowTitle(this->_window, title.str().c_str());

        this->last_time = this->current_time;
        this->num_frames = -1;
        this->frame_time = float(1000.0 / framerate);
    }

    ++this->num_frames;
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
