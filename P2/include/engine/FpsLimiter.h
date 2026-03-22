#ifndef FPS_LIMITER_H
#define FPS_LIMITER_h


#include <GLFW/glfw3.h>

class FrameLimiter {
public:
    explicit FrameLimiter(double fps)
        : m_target(1.0 / fps), m_prev(glfwGetTime()) {}

    void setFPS(double fps) { m_target = 1.0 / fps; }

    double tick() {
        double now = glfwGetTime();
        double dt  = now - m_prev;
        m_prev = now;
        return dt;
    }

    void limit() {
        double elapsed = glfwGetTime() - m_prev;
        // Busy-wait -> not nice :(
        while ((glfwGetTime() - m_prev) < m_target) {}
    }

private:
    double m_target;
    double m_prev;
};

#endif
