#pragma once

#include <GL/gl3w.h>
#include <memory>

struct GLFWwindow;

namespace gl {

class GLInstance
{
public:
    GLInstance();
    ~GLInstance();

    GLInstance(GLInstance &) = delete;
    GLInstance(const GLInstance &) = delete;
    GLInstance &operator=(GLInstance &) = delete;
    GLInstance &operator=(const GLInstance &) = delete;

private:
    std::shared_ptr<int> glfw_ = nullptr;
    std::shared_ptr<GLFWwindow> window_ = nullptr;
};

} // namespace gl
