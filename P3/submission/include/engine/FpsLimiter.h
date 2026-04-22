#ifndef FPS_LIMITER_H
#define FPS_LIMITER_H

#include <GLFW/glfw3.h>

class FrameLimiter {
public:
    explicit FrameLimiter(double fps)
        : m_target(1.0 / fps), m_prev(glfwGetTime()) {}

    void setFPS(double fps) { m_target = 1.0 / fps; }

    double tick();
    void limit();

private:
    double m_target;
    double m_prev;
};

#endif
