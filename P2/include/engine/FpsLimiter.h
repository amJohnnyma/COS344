#include <chrono>
#include <thread>
#include <cstdio>
#include <iostream>

class FrameLimiter {
public:
    using clock = std::chrono::steady_clock;
    using duration = std::chrono::duration<double>;

    explicit FrameLimiter(double fps)
        : target(1.0 / fps), prev(clock::now()) {}

    void setFPS(double fps) {
        target = duration(1.0 / fps);
    }

    // Call at the START of each frame. Returns delta time in seconds.
    double tick() {
        auto now = clock::now();
        double dt = duration(now - prev).count();
        prev = now;
        return dt;
    }

    // Call at the END of each frame, after all work is done.
    void limit() {
        auto frame_end = clock::now();
        auto elapsed = duration(frame_end - prev);
        if (elapsed < target) {
            std::this_thread::sleep_for(target - elapsed);
        }
    }

private:
    duration target;
    clock::time_point prev;
};
