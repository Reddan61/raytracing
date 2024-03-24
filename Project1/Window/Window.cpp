#include "Window.h"

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->on_resize();
}

Window::Window(const uint32_t WIDTH, const uint32_t HEIGHT, Camera* camera)
{
    this->camera = camera;
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
    double lastTime = 0.0;
    double currentTime = 0.0;
    double lastFPSUpdateTime = 0.0;
    double delta = 0;
    unsigned int counter = 0;

    while (!glfwWindowShouldClose(this->_window)) {
        currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;
        counter++;

        this->camera->keyCheck(this->_window, delta);

        if (currentTime - lastFPSUpdateTime >= 1.0) {
            std::string fps = std::to_string(counter);
            std::string ms = std::to_string(1000 / counter);
            std::string title = "FPS = " + fps + ", ms = " + ms;

            glfwSetWindowTitle(this->_window, title.c_str());
            counter = 0;
            lastFPSUpdateTime += 1.0f;
        }

        glfwPollEvents();

        this->draw();
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
