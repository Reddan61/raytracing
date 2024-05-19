#include "Window.h"

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->on_resize();
}

Window::Window(const uint32_t WIDTH, const uint32_t HEIGHT, Scene* scene)
{
    this->scene = scene;
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    this->_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);
    glfwSetWindowUserPointer(this->_window, this);
    glfwSetFramebufferSizeCallback(this->_window, framebufferResizeCallback);

    this->vulkan = new Vulkan(this->_window, this->scene);
}

Window::~Window()
{
    this->glfw_clean_up();
    delete this->vulkan;
    delete this->scene;
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

        this->scene->update(this->_window, delta);

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

    this->vulkan->stop();
}

void Window::glfw_clean_up()
{
    glfwDestroyWindow(this->_window);
    glfwTerminate();
}

void Window::draw()
{
    if (this->vulkan != nullptr) {
        this->vulkan->draw_frame(this->_window, this->scene);
    }
}

void Window::on_resize()
{
    this->vulkan->on_resize(true);
}
