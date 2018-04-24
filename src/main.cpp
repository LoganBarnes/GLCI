#include "glci/GLInstance.hpp"
#include <chrono>
#include <thread>

int main()
{
    gl::GLInstance gl;

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, 640, 480);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);
}
