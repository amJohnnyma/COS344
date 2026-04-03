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

#include "include/DebugOptions.h"
#include "include/math/Vector.h"
#include "include/engine/Input.h"
#include "include/engine/SceneCreator.h"

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

auto& input = InputManager::get_instance();

void processInput(GLFWwindow* window, FrameLimiter& limiter, double& targetFps, bool& shouldRestart, SceneCreator<3>& sceneC)
{

    // slected must have colour changed to a pastel version
    if(input.is_key_pressed(GLFW_KEY_1))
    {
        //select golf ball
        sceneC.getActive()->selectGolfBall();
    }
    if(input.is_key_pressed(GLFW_KEY_2))
    {
        //select obstacle
        sceneC.getActive()->selectObstacle();
    }
    if(input.is_key_pressed(GLFW_KEY_3))
    {
        //select other type of obstacle
        sceneC.getActive()->selectObstacle();
    }
    if(input.is_key_pressed(GLFW_KEY_4))
    {
        //select golf hole
        sceneC.getActive()->selectHole();
    }
    if(input.is_key_pressed(GLFW_KEY_0))
    {
        //deselect current selection
        sceneC.getActive()->deselectObject();
    }

    //transformations, apply a force to the object
    if(input.is_key_pressed(GLFW_KEY_W))
    {
        // move up
        sceneC.getActive()->moveSelected(Vector<3>{0.f,0.5f,0.f});
    }
    if(input.is_key_pressed(GLFW_KEY_S))
    {
        // move down
        sceneC.getActive()->moveSelected(Vector<3>{0.f,-0.5f,0.f});
    }
    if(input.is_key_pressed(GLFW_KEY_A))
    {
        // move left
        sceneC.getActive()->moveSelected(Vector<3>{-0.5f,0.f,0.f});
    }
    if(input.is_key_pressed(GLFW_KEY_D))
    {
        // move right
        sceneC.getActive()->moveSelected(Vector<3>{0.5f,0.f,0.f});
    }
    if(input.is_key_pressed(GLFW_KEY_EQUAL))
    {
        //scale up
        sceneC.getActive()->scaleSelected(1.1);
    }
    if(input.is_key_pressed(GLFW_KEY_MINUS))
    {
        //scale down
        sceneC.getActive()->scaleSelected(0.9);
    }
    if(input.is_key_pressed(GLFW_KEY_E))
    {
        // rotate CW
        sceneC.getActive()->rotateSelected(-15);
    }
    if(input.is_key_pressed(GLFW_KEY_Q))
    {
        // rotate CCW
        sceneC.getActive()->rotateSelected(15);
    }

    // Wireframe (ENTER)
    if (input.is_key_pressed(GLFW_KEY_ENTER))
    {
        DebugOptions::get().wireframe = !DebugOptions::get().wireframe;
        std::cout << "Wireframe: " << (DebugOptions::get().wireframe ? "ON" : "OFF") << "\n";
    }

    // Grid (G)
    if (input.is_key_pressed(GLFW_KEY_G))
    {
        DebugOptions::get().showGrid = !DebugOptions::get().showGrid;
        std::cout << "Grid: " << (DebugOptions::get().showGrid ? "ON" : "OFF") << "\n";
    }

    // Pause (P)
    if (input.is_key_pressed(GLFW_KEY_P))
    {
        sceneC.togglePause();
        std::cout << (sceneC.isPaused() ? "PAUSED" : "RESUMED") << "\n";
    }

    // Restart (R)
    if (input.is_key_pressed(GLFW_KEY_R))
    {
        shouldRestart = true;
        sceneC.setPause(false);
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

    bool shouldRestart = false;
    SceneCreator<3> sceneC;
    sceneC.loadScenes("pTest");
    
    while (!glfwWindowShouldClose(window))
    {
        double dt = limiter.tick();

        processInput(window, limiter, targetFps, shouldRestart, sceneC);

        if (shouldRestart) {
           // scene.initScene();
            shouldRestart = false;
            sceneC = SceneCreator<3>();
            sceneC.loadScenes("pTest");
        }

        renderer.beginFrame();
        sceneC.update(dt, renderer);

        if (DebugOptions::get().showGrid)
            renderer.drawDebugGrid(10);

        renderer.endFrame();

        InputManager::get_instance().update();
        glfwSwapBuffers(window);
        glfwPollEvents();

        printf("dt: %.4f s  |  fps: %.1f  |  target: %.0f  |  %s\n",
               dt, 1.0 / dt, targetFps, sceneC.isPaused() ? "PAUSED" : "");

        limiter.limit();
    }
sceneC.saveScenes("DemoSave");

    glfwTerminate();
    return 0;
}
