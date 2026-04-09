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
#include "include/engine/Camera.h"

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
float windmillSpeed = 0.f;

void processInput(GLFWwindow* window, FrameLimiter& limiter, double& targetFps, bool& shouldRestart, SceneCreator<3>& sceneC, Camera& cam)
{

    // previous scene
    if(input.is_key_pressed(GLFW_KEY_1))
    {
        sceneC.selectPreviousScene();
        
    }
    // next scene
    if(input.is_key_pressed(GLFW_KEY_2))
    {

        sceneC.selectNextScene();
    }


    //transformations, apply a force to the object
    if(input.is_key_down(GLFW_KEY_W))
    {
        // rotate x
        sceneC.getActive()->rotateScene(Vector<3>{0.1f,0.f,0.f});
    }
    if(input.is_key_down(GLFW_KEY_S))
    {
        // rotate -x
        sceneC.getActive()->rotateScene(Vector<3>{-0.1f,0.f,0.f});
    }
    if(input.is_key_down(GLFW_KEY_A))
    {
        // rotate y
        sceneC.getActive()->rotateScene(Vector<3>{0.f,0.1f,0.f});
    }
    if(input.is_key_down(GLFW_KEY_D))
    {
        // rotate -y
        sceneC.getActive()->rotateScene(Vector<3>{0.f,-0.1f,0.f});
    }

    if(input.is_key_down(GLFW_KEY_E))
    {
        // rotate z
        sceneC.getActive()->rotateScene(Vector<3>{0.f,0.f,0.1f});
    }
    if(input.is_key_down(GLFW_KEY_Q))
    {
        // rotate -z
        sceneC.getActive()->rotateScene(Vector<3>{0.f,0.f,-0.1f});
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

    //rotate windmill (EQUAL)

    if (input.is_key_down(GLFW_KEY_EQUAL))
    {
        windmillSpeed += 0.005;

    windmillSpeed = std::copysign(std::min(std::abs(windmillSpeed), 0.9f),windmillSpeed);
    }
    //rotate windmill (MINUS)
    if (input.is_key_down(GLFW_KEY_MINUS))
    {
        windmillSpeed -= 0.005;

    windmillSpeed = std::copysign(std::min(std::abs(windmillSpeed), 0.9f),windmillSpeed);
    }

    /*
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
    */

    //camera movement
    float camSpeed = 0.2f;

    Vector<3> forward = cam.getForward();
    Vector<3> right   = cam.getRight();
    Vector<3> up      = cam.getUp();

    // Forward/back
    if (input.is_key_down(GLFW_KEY_I)) {
        cam.position[0] -= forward[0] * camSpeed;
        cam.position[1] -= forward[1] * camSpeed;
        cam.position[2] -= forward[2] * camSpeed;
    }
    if (input.is_key_down(GLFW_KEY_K)) {
        cam.position[0] += forward[0] * camSpeed;
        cam.position[1] += forward[1] * camSpeed;
        cam.position[2] += forward[2] * camSpeed;
    }

    // Strafe left/right
    if (input.is_key_down(GLFW_KEY_J)) {
        cam.position[0] -= right[0] * camSpeed;
        cam.position[1] -= right[1] * camSpeed;
        cam.position[2] -= right[2] * camSpeed;
    }
    if (input.is_key_down(GLFW_KEY_L)) {
        cam.position[0] += right[0] * camSpeed;
        cam.position[1] += right[1] * camSpeed;
        cam.position[2] += right[2] * camSpeed;
    }

    // Up/down (world up, not camera up — feels more natural)
    if (input.is_key_down(GLFW_KEY_U)) cam.position[1] += camSpeed;
    if (input.is_key_down(GLFW_KEY_O)) cam.position[1] -= camSpeed;
                                                                      // Camera rotation
    if (input.is_key_down(GLFW_KEY_LEFT))  cam.yaw   += 0.02f;   // look left
    if (input.is_key_down(GLFW_KEY_RIGHT)) cam.yaw   -= 0.02f;   // look right
    if (input.is_key_down(GLFW_KEY_UP))    cam.pitch += 0.02f;   // look up
    if (input.is_key_down(GLFW_KEY_DOWN))  cam.pitch -= 0.02f;   // look down

    // Clamp pitch so camera doesn't flip upside down
    if (cam.pitch >  1.5f) cam.pitch =  1.5f;
    if (cam.pitch < -1.5f) cam.pitch = -1.5f;
}
int main()
{
    srand(42);

    GLFWwindow* window = setUp();
    InputManager::get_instance().register_window(window);

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    Renderer<3> renderer(w, h);
    Camera cam;
    cam.fovY   = M_PI / 2.0f;
    cam.aspect = (float)w / h;
    cam.nearZ  = 0.1f;
    cam.farZ   = 100.0f;
    cam.position = Vector<3>{0.f,0.f,20.f};
    renderer.setViewProj(cam.getView(), cam.getProj());

    double targetFps = 60.0;
    FrameLimiter limiter(targetFps);
    double count = 0; // when count is 1 then display fps (Each second)



    bool shouldRestart = false;
    SceneCreator<3> sceneC;
    sceneC.loadScenes("test");



    while (!glfwWindowShouldClose(window))
    {
        double dt = limiter.tick();

        processInput(window, limiter, targetFps, shouldRestart, sceneC, cam);

        if (shouldRestart) {
            // scene.initScene();
            shouldRestart = false;
            sceneC = SceneCreator<3>();
            sceneC.loadScenes("test");
        }
        //cam.yaw += (float)dt * 0.5f;          // auto-spin — remove when done testing
        //cam.orbitAround(Vector<3>{0,0,0}, 20.0f);
        renderer.setViewProj(cam.getView(), cam.getProj());

        renderer.beginFrame();
        sceneC.update(dt, renderer);

        sceneC.getActive()->rotateWindmillBlade(windmillSpeed);

        if (DebugOptions::get().showGrid)
            renderer.drawDebugGrid(10);

        renderer.endFrame();

        InputManager::get_instance().update();
        glfwSwapBuffers(window);
        glfwPollEvents();

        count += dt;
        if(count > 1)
        {
            printf("dt: %.4f s  |  fps: %.1f  |  target: %.0f  |  %s\n",
                    dt, 1.0 / dt, targetFps, sceneC.isPaused() ? "PAUSED" : "");
            count = 0;
        }


        limiter.limit();
    }
    sceneC.saveScenes("DemoSave");

    glfwTerminate();
    return 0;
}
