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


// Temporary input handling (W key toggles wireframe)
void processInput(GLFWwindow* window)
{
    static bool wWasPressed = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (!wWasPressed) {
            DebugOptions::get().wireframe = !DebugOptions::get().wireframe;
            std::cout << "Wireframe: "
                      << (DebugOptions::get().wireframe ? "ON" : "OFF") << "\n";
            wWasPressed = true;
        }
    } else {
        wWasPressed = false;
    }
}

// main
int main()
{
    srand(42);

    GLFWwindow* window = setUp();

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    Renderer<2> renderer(w, h);

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

    Square<2> background(Vector<2>{0.0f, 0.0f}, 13, 18);
    background.setColor(colGrid.r, colGrid.g, colGrid.b);

    Square<2> courseBorder(Vector<2>{0.0f, 0.0f}, 12, 17);
    courseBorder.setColor(colBrown.r, colBrown.g, colBrown.b);

    Square<2> courseGround(Vector<2>{0.0f, 0.0f}, 11, 16);
    courseGround.setColor(colGreen.r, colGreen.g, colGreen.b);

    Square<2> waterFeature(Vector<2>{0.0f, 0.0f}, 11, 1.5);
    waterFeature.setColor(colBlue.r, colBlue.g, colBlue.b);

    Square<2> waterBridge1(Vector<2>{0.0f, -3.0f}, 1.5, 3);
    waterBridge1.setColor(colChocolate.r, colChocolate.g, colChocolate.b);

    Square<2> waterBridge2(Vector<2>{0.0f, 3.0f}, 1.5, 3);
    waterBridge2.setColor(colChocolate.r, colChocolate.g, colChocolate.b);

    Square<2> reboundObstacle1(Vector<2>{-7, -4}, 1, 2);
    reboundObstacle1.setColor(colAmber.r, colAmber.g, colAmber.b);
    reboundObstacle1.rotate(-45.f);

    Square<2> reboundObstacle2(Vector<2>{-7, 4}, 1, 2);
    reboundObstacle2.setColor(colAmber.r, colAmber.g, colAmber.b);
    reboundObstacle2.rotate(45.f);

    Square<2> midCourseWall(Vector<2>{1.5f, 0.f}, 1.f, 13.f);
    midCourseWall.setColor(colBrown.r, colBrown.g, colBrown.b);

    Circle<2> ball(Vector<2>{7.0f, -3.0f}, 0.3f, 32);
    ball.setColor(colWhite.r, colWhite.g, colWhite.b);

    Square<2> startArea(Vector<2>{7.f, -3.f}, 3.f, 1.f);
    startArea.setColor(colMagenta.r, colMagenta.g, colMagenta.b);


    courseGround.add(&waterFeature);
    courseGround.add(&waterBridge1);
    courseGround.add(&waterBridge2);
    courseGround.add(&reboundObstacle1);
    courseGround.add(&reboundObstacle2);
    courseGround.add(&midCourseWall);
    courseGround.add(&startArea);
    courseGround.add(&ball);

    courseBorder.add(&courseGround);
    background.add(&courseBorder);
    //ball.enablePhysics(Vector<2>{-10.f, 0.f});
    background.enablePhysics(Vector<2>{-10.f, 0.f});
    ball.enablePhysics(Vector<2>{-5.f, 0.f});


    FrameLimiter limiter(60.0);


    while (!glfwWindowShouldClose(window))
    {
        double dt = limiter.tick();

        renderer.beginFrame();
        processInput(window);
       // background.rotate(1 / 60.0f);
        ball.rotate(1/60.0f);
        background.updatePhysics(dt);

        background.render(renderer);
        

        renderer.endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();

        printf("dt: %.4f s  |  fps: %.1f\n", dt, 1.0 / dt);

        limiter.limit();
    }

    glfwTerminate();
    return 0;
}
