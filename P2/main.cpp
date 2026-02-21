#include <glm/ext/scalar_uint_sized.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <random>

#include "include/engine/FpsLimiter.h"
#include "include/engine/Renderer.h"   

#include "include/math/Shape.h"
#include "include/math/Circle.h"
#include "include/math/Square.h"
#include "include/math/Triangle.h"

#include "include/DebugOptions.h"


using namespace glm;
using namespace std;

const char *getError()
{
    const char *errorDescription;
    glfwGetError(&errorDescription);
    return errorDescription;
}

inline void startUpGLFW()
{
    glewExperimental = true; // Needed for core profile
    if (!glfwInit())
    {
        throw getError();
    }
}

inline void startUpGLEW()
{
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw getError();
    }
}

inline GLFWwindow *setUp()
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
    GLFWwindow *window;                                            // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(1000, 1000, "Experiment", NULL, NULL);
    if (window == NULL)
    {
        cout << getError() << endl;
        glfwTerminate();
        throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    startUpGLEW();
    return window;
}

//temporary input
void processInput(GLFWwindow* window)
{
    static bool wWasPressed = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (!wWasPressed)
        {
            DebugOptions::get().wireframe = !DebugOptions::get().wireframe;
            std::cout << "Wireframe: " << (DebugOptions::get().wireframe ? "ON" : "OFF") << "\n";
            wWasPressed = true;
        }
    }
    else
    {
        wWasPressed = false;
    }

}

int main() {
    GLFWwindow* window = setUp();  // your function

    // Create renderer (pass window size)
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    Renderer<2> renderer(w, h);

    //////////////////////////////////////////////////////////////////// TEMP

    std::vector<Square<2>> squares;
    std::vector<Triangle<2>> triangles;

    const int gridSize = 8;
    const float cellSize = 1.5f;

    for (int x = -gridSize; x <= gridSize; ++x) {
        for (int y = -gridSize; y <= gridSize; ++y) {
            Vector<2> center{ x * cellSize, y * cellSize };
            squares.emplace_back(center, cellSize * 0.9f, cellSize * 0.9f);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDist(-8.0f, 8.0f);
    std::uniform_real_distribution<float> sizeDist(0.6f, 2.2f);

    const int numRandomTriangles = 15;

    for (int i = 0; i < numRandomTriangles; ++i) {
        Vector<2> p1{ posDist(gen), posDist(gen) };
        Vector<2> p2 = p1 + Vector<2>{ sizeDist(gen), sizeDist(gen) * 0.7f };
        Vector<2> p3 = p1 + Vector<2>{ sizeDist(gen) * -0.6f, sizeDist(gen) * 1.2f };

        triangles.emplace_back(p1, p2, p3);
    }

    Square<2> bigSquare({ 5.0f, 4.0f }, 3.5f, 3.5f);
    Triangle<2> bigTri({ -6.0f, 5.0f }, { -4.0f, 7.0f }, { -8.0f, 7.0f });
    Square<2> obstacle({ 0.0f, 6.0f }, 2.8f, 1.2f);     // long horizontal bar


    ///////////////////////////////////////////

    FrameLimiter limiter(60.0);

    while (!glfwWindowShouldClose(window)) {
        double dt = limiter.tick();

        //Update game logic here
        renderer.beginFrame();
        // wireframe
        processInput(window);

        // Draw shapes with colors
        for (const auto& sq : squares) {
            renderer.drawShape(&sq, glm::vec4(0.25f, 0.28f, 0.32f, 1.0f));
        }

        std::vector<glm::vec4> triColors = {
            {0.9f, 0.4f, 0.4f, 1.0f},   // red-ish
            {0.4f, 0.9f, 0.4f, 1.0f},   // green-ish
            {0.4f, 0.4f, 0.9f, 1.0f},   // blue-ish
            {0.9f, 0.9f, 0.4f, 1.0f},   // yellow
            {0.7f, 0.4f, 0.9f, 1.0f}    // purple
        };

        for (size_t i = 0; i < triangles.size(); ++i) {
            auto& t = triangles[i];
            glm::vec4 col = triColors[i % triColors.size()];
            renderer.drawShape(&t, col);
        }

        renderer.drawShape(&bigSquare,   glm::vec4(0.1f, 0.6f, 0.9f, 1.0f));  // cyan
        renderer.drawShape(&bigTri,      glm::vec4(0.9f, 0.1f, 0.5f, 1.0f));  // magenta
        renderer.drawShape(&obstacle,    glm::vec4(0.95f, 0.6f, 0.1f, 1.0f)); // orange-yellow

        renderer.endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();

        printf("dt: %.4f s  |  fps: %.1f\n", dt, 1.0 / dt);

        limiter.limit();
    }

    glfwTerminate();
    return 0;
}
