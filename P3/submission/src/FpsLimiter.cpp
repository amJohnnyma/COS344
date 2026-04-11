#include "../include/engine/FpsLimiter.h"

double FrameLimiter::tick()
{
    double now = glfwGetTime();
    double dt  = now - m_prev;
    m_prev = now;
    return dt;
}

void FrameLimiter::limit()
{
    while ((glfwGetTime() - m_prev) < m_target) {}
}
