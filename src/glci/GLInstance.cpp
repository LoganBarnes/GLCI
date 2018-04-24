#include "GLInstance.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#define DEBUG_PRINT(msg) std::cout << "DEBUG: " << (msg) << std::endl
//#define DEBUG_PRINT(msg) {}

gl::GLInstance::GLInstance()
{
    // set error callback before any glfw calls
    glfwSetErrorCallback([](int error, const char *description) {
        std::cerr << "ERROR: (" << error << ") " << description << std::endl;
    });

    glfw_ = std::shared_ptr<int>(new int(glfwInit()), [](auto p) {
        glfwTerminate(); // safe even if glfwInit() fails
        delete p;
        DEBUG_PRINT("GLFW terminated");
    });

    if (*glfw_ == 0) {
        throw std::runtime_error("GLFW init failed");
    }
    DEBUG_PRINT("GLFW initialized");

    // On some Linux systems, creating contexts via both the native and EGL APIs
    // in a single process will cause the application to segfault.
    // Stick to one API or the other on Linux for now.
    // http://www.glfw.org/docs/latest/bug.html#_bug000001
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

    // Make sure we have a good OpenGL Profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    window_ = std::shared_ptr<GLFWwindow>(glfwCreateWindow(640, 480, "OpenGL window", nullptr, nullptr), [](auto p) {
        if (p) {
            glfwDestroyWindow(p);
            DEBUG_PRINT("GLFW window destroyed");
        }
    });

    if (window_ == nullptr) {
        throw std::runtime_error("GLFW window creation failed");
    }
    DEBUG_PRINT("GLFW window created");

    glfwMakeContextCurrent(window_.get());
    glfwSwapInterval(1);

    if (gl3wInit()) {
        throw std::runtime_error("failed to initialize OpenGL");
    }

    if (!gl3wIsSupported(4, 5)) {
        std::cerr << "OpenGL 4.5 not supported" << std::endl;
        if (!gl3wIsSupported(4, 1)) {
            std::cerr << "OpenGL 4.1 not supported" << std::endl;
            if (!gl3wIsSupported(3, 2)) {
                std::cerr << "OpenGL 3.2 not supported" << std::endl;
            }
        }
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION) << " GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION)
              << std::endl;
}

gl::GLInstance::~GLInstance() = default;
