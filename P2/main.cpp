#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// glm/glm.hpp is in the allowed list. included only for GL type compatibility.
// No glm math functions (ortho, mat4 arithmetic, etc.) are called.
#include <glm/glm.hpp>

#include "include/engine/FpsLimiter.h"
#include "include/engine/Renderer.h"    // uses Color4, Matrix<4,4>

#include "include/math/Shape.h"
#include "include/math/Circle.h"
#include "include/math/Square.h"
#include "include/math/Triangle.h"

#include "include/DebugOptions.h"
#include "include/math/Vector.h"
#include "include/engine/Physics.h"
#include "include/engine/Input.h"

#include <cmath>   // for random placement helpers

// Simple LCG pseudo-random in [-range, range] — avoids <random> (not in the
// allowed list). Uses stdlib rand().
static float randF(float lo, float hi)
{
    float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return lo + t * (hi - lo);
}

// GLFW / GLEW startup helpers
const char* getError()
{
    const char* errorDescription;
    glfwGetError(&errorDescription);
    return errorDescription;
}

inline void startUpGLFW()
{
    if (!glfwInit())
        throw getError();
}

inline void startUpGLEW()
{
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw getError();
    }
}

inline GLFWwindow* setUp()
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Mini-Golf (u23536030)", NULL, NULL);
    if (!window) {
        std::cout << getError() << std::endl;
        glfwTerminate();
        throw "Failed to open GLFW window.";
    }
    glfwMakeContextCurrent(window);
    startUpGLEW();
    return window;
}

// Build a 4×4 orthographic projection matrix from first principles.
// Maps x∈[l,r], y∈[b,t], z∈[nearZ,farZ] → NDC [-1,1]³.
static Matrix<4,4> buildOrtho(float l, float r, float b, float t,
                               float nearZ, float farZ)
{
    Matrix<4,4> m;   // default-ctor: all zeros

    m[0][0] =  2.0f / (r - l);
    m[1][1] =  2.0f / (t - b);
    m[2][2] = -2.0f / (farZ - nearZ);
    m[3][3] =  1.0f;

    // Translation components (last column in row-major storage)
    m[0][3] = -(r + l) / (r - l);
    m[1][3] = -(t + b) / (t - b);
    m[2][3] = -(farZ + nearZ) / (farZ - nearZ);

    return m;
}

// Build a 4×4 identity matrix.
static Matrix<4,4> buildIdentity4()
{
    Matrix<4,4> m;
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
    return m;
}

static bool paused = false;
auto& input = InputManager::get_instance();

void processInput(GLFWwindow* window, FrameLimiter& limiter, double& targetFps, bool& shouldRestart)
{

    // Wireframe (W)
    if (input.is_key_pressed(GLFW_KEY_W))
    {
        DebugOptions::get().wireframe = !DebugOptions::get().wireframe;
        std::cout << "Wireframe: " << (DebugOptions::get().wireframe ? "ON" : "OFF") << "\n";
    }

    // Pause (P)
    if (input.is_key_pressed(GLFW_KEY_P))
    {
        paused = !paused;
        std::cout << (paused ? "PAUSED" : "RESUMED") << "\n";
    }

    // Restart (R)
    if (input.is_key_pressed(GLFW_KEY_R))
    {
        shouldRestart = true;
        paused        = false;
        std::cout << "RESTARTING...\n";
    }

    // Increase FPS (UP)
    if (input.is_key_pressed(GLFW_KEY_UP))
    {
        targetFps = std::min(targetFps + 10.0, 240.0);
        limiter.setFPS(targetFps);
        std::cout << "FPS target: " << targetFps << "\n";
    }

    // Decrease FPS (DOWN)
    if (input.is_key_pressed(GLFW_KEY_DOWN))
    {
        targetFps = std::max(targetFps - 10.0, 10.0);
        limiter.setFPS(targetFps);
        std::cout << "FPS target: " << targetFps << "\n";
    }
}
int main()
{
    srand(42);

    GLFWwindow* window = setUp();
    InputManager::get_instance().register_window(window);

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    Renderer<3> renderer(w, h);

    double targetFps = 60.0;
    FrameLimiter limiter(targetFps);


    Matrix<4,4> viewMat = buildIdentity4();
    Matrix<4,4> projMat = buildOrtho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
    renderer.setViewProj(viewMat, projMat);

    Color4 colGrid        ( 0.25f, 0.28f, 0.32f, 1.0f );
    Color4 colBrown       ( 0.50f, 0.28f, 0.10f, 1.0f );
    Color4 colGreen       ( 0.10f, 0.80f, 0.20f, 1.0f );
    Color4 colBlue        ( 0.10f, 0.30f, 0.90f, 1.0f );
    Color4 colChocolate   ( 0.35f, 0.18f, 0.05f, 1.0f );
    Color4 colAmber       ( 1.00f, 0.75f, 0.00f, 1.0f );
    Color4 colWhite       ( 1.00f, 1.00f, 1.00f, 1.0f );
    Color4 colMagenta( 1.00f, 0.00f, 1.00f, 1.0f );

    Square<3> background(Vector<3>{0.0f, 0.0f, 0.f}, 13, 18);
    background.setColor(colGrid.r, colGrid.g, colGrid.b);

    Square<3> reboundObstacle1(Vector<3>{-7, -4, 1.f}, 1, 2);
    reboundObstacle1.setColor(colAmber.r, colAmber.g, colAmber.b);
    reboundObstacle1.rotate(-45.f);
    Square<3> reboundObstacle2(Vector<3>{-7, 4, 1.f}, 1, 2);
    reboundObstacle2.setColor(colAmber.r, colAmber.g, colAmber.b);
    reboundObstacle2.rotate(45.f);
    Circle<3> ball(Vector<3>{7.0f, -3.0f, 1.f}, 0.3f, 32);
    ball.setColor(colWhite.r, colWhite.g, colWhite.b);

    Circle<3> ball1(Vector<3>{-6.0f, -3.0f, 1.f}, 0.3f, 32);
    ball1.setColor(colGrid.r, colGreen.g, colGreen.b);

    const float wallThick = 1.0f;
    const float halfW = 9.0f;   // half of width  18
    const float halfH = 6.5f;   // half of height 13

    Square<3> wallLeft  (Vector<3>{-(halfW + wallThick * 0.5f),  0.0f, 1.f}, 13.0f + wallThick * 2, wallThick);
    Square<3> wallRight (Vector<3>{+(halfW + wallThick * 0.5f),  0.0f, 1.f}, 13.0f + wallThick * 2, wallThick);
    Square<3> wallBottom(Vector<3>{0.0f, -(halfH + wallThick * 0.5f), 1.f}, wallThick, 18.0f);
    Square<3> wallTop   (Vector<3>{0.0f, +(halfH + wallThick * 0.5f), 1.f}, wallThick, 18.0f);

    wallLeft.setColor(colAmber.r, colAmber.g, colAmber.b);
    wallRight.setColor(colAmber.r,colAmber.g,colAmber.b);
    wallBottom.setColor(colAmber.r,colAmber.g,colAmber.b);
    wallTop.setColor(colAmber.r,colAmber.g,colAmber.b);

    Square<3> river(Vector<3>{0.f,0.f,-1.f}, 13.f, 2.f);
    river.setColor(colBlue.r, colBlue.g, colBlue.b);

    background.add(&reboundObstacle1);
    background.add(&reboundObstacle2);
    background.add(&wallLeft);
    background.add(&wallRight);
    background.add(&wallBottom);
    background.add(&wallTop);
    background.add(&river);
    background.add(&ball);
    background.add(&ball1);

    //background.enablePhysics(Vector<2>{-10.f, 0.f});
    ball.enablePhysics(Vector<3>{30.f, 0.f, 0.f});
    ball1.enablePhysics(Vector<3>{-30.f, 0.f, 0.f});

    std::vector<Shape<3>*> scene;
    scene.push_back(&background);
    scene.push_back(&ball);
    scene.push_back(&ball1);
    scene.push_back(&reboundObstacle1);
    scene.push_back(&reboundObstacle2);
    scene.push_back(&wallLeft);         
    scene.push_back(&wallRight);        
    scene.push_back(&wallBottom);       
    scene.push_back(&wallTop);
    scene.push_back(&river);

    // Lambda to (re)initialize scene state
    auto initScene = [&]() {
        ball.setPosition(Vector<3>{7.0f, -3.0f, 1.f});
        ball.enablePhysics(Vector<3>{-20.f, 0.f, 0.f});
        ball1.setPosition(Vector<3>{-6.0f, -3.0f, 1.f});
        ball1.enablePhysics(Vector<3>{20.f, 0.f, 0.f});
    };

    PhysicsEngine physics;
    bool shouldRestart = false;

    std::cout << "Controls:\n"
              << "  W        - Toggle wireframe\n"
              << "  P        - Pause / Resume\n"
              << "  R        - Restart\n"
              << "  UP/DOWN  - Increase / Decrease FPS target\n";

    while (!glfwWindowShouldClose(window))
    {
        double dt = limiter.tick();

        processInput(window, limiter, targetFps, shouldRestart);

        if (shouldRestart) {
            initScene();
            physics = PhysicsEngine{};
            shouldRestart = false;
        }

        renderer.beginFrame();

        if (!paused)
            physics.update(scene, dt);

        background.render(renderer);
        renderer.endFrame();

        InputManager::get_instance().update();
        glfwSwapBuffers(window);
        glfwPollEvents();

        printf("dt: %.4f s  |  fps: %.1f  |  target: %.0f  |  %s\n",
               dt, 1.0 / dt, targetFps, paused ? "PAUSED" : "");

        limiter.limit();
    }

    glfwTerminate();
    return 0;
}
