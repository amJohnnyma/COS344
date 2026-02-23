#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// glm/glm.hpp is in the allowed list — included only for GL type compatibility.
// No glm math functions (ortho, mat4 arithmetic, etc.) are called.
#include <glm/glm.hpp>

#include "include/engine/FpsLimiter.h"
#include "include/engine/Renderer.h"    // uses Color4, Matrix<4,4>

#include "include/math/Shape.h"
#include "include/math/Circle.h"
#include "include/math/Square.h"
#include "include/math/Triangle.h"

#include "include/DebugOptions.h"

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

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Mini-Golf", NULL, NULL);
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
    srand(42);   // reproducible pseudo-random layout

    GLFWwindow* window = setUp();

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    Renderer<2> renderer(w, h);

    // Build view (identity) and proj (our custom ortho) and pass to renderer.
    // These replace the old glm::ortho / glm::mat4(1) calls.
    Matrix<4,4> viewMat = buildIdentity4();
    Matrix<4,4> projMat = buildOrtho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
    renderer.setViewProj(viewMat, projMat);

    // Scene geometry — created from first principles using Shape classes
    // Background
    Square<2> background = Square<2>(Vector<2>{0.0f,0.0f}, 13, 18);
    Square<2> courseBorder = Square<2>(Vector<2>{0.0f,0.0f}, 12, 17);
    Square<2> courseGround = Square<2>(Vector<2>{0.0f,0.0f}, 11, 16);

    //obstacles
    // water feature
    Square<2> waterFeature = Square<2>(Vector<2>{0.0f,0.0f}, 11, 1.5);
    Square<2> waterBridge1 = Square<2>(Vector<2>{0.0f, -3.0f}, 1.5, 3);
    Square<2> waterBridge2 = Square<2>(Vector<2>{0.0f, 3.0f}, 1.5, 3);

    /*
    // Grid of background squares
    const int   gridSize = 8;
    const float cellSize = 1.5f;

    std::vector<Square<2>> squares;
    for (int x = -gridSize; x <= gridSize; ++x) {
        for (int y = -gridSize; y <= gridSize; ++y) {
            Vector<2> center{ static_cast<float>(x) * cellSize,
                              static_cast<float>(y) * cellSize };
            squares.emplace_back(center, cellSize * 0.9f, cellSize * 0.9f);
        }
    }

    // Random triangles — use our randF instead of std::uniform_real_distribution
    const int numTriangles = 15;
    std::vector<Triangle<2>> triangles;
    triangles.reserve(numTriangles);
    for (int i = 0; i < numTriangles; ++i) {
        Vector<2> p1{ randF(-8.0f, 8.0f), randF(-8.0f, 8.0f) };
        Vector<2> p2 = p1 + Vector<2>{ randF(0.6f, 2.2f), randF(0.6f, 2.2f) * 0.7f };
        Vector<2> p3 = p1 + Vector<2>{ randF(0.6f, 2.2f) * -0.6f,
                                        randF(0.6f, 2.2f) * 1.2f };
        triangles.emplace_back(p1, p2, p3);
    }

    // Named obstacle shapes
    Square<2>   bigSquare({ 5.0f,  4.0f }, 3.5f, 3.5f);
    Triangle<2> bigTri(  { -6.0f, 5.0f }, { -4.0f, 7.0f }, { -8.0f, 7.0f });
    Square<2>   obstacle({ 0.0f,  6.0f }, 2.8f, 1.2f);

    */
    // A circle (golf ball placeholder)
    Circle<2> ball({ 0.0f, 0.0f }, 0.35f, 32);

    // Colour palette — using Color4 (replaces glm::vec4)
    // Neutrals
    Color4 colWhite      ( 1.00f, 1.00f, 1.00f, 1.0f );
    Color4 colBlack      ( 0.00f, 0.00f, 0.00f, 1.0f );
    Color4 colGray       ( 0.50f, 0.50f, 0.50f, 1.0f );
    Color4 colLightGray  ( 0.80f, 0.80f, 0.80f, 1.0f );
    Color4 colDarkGray   ( 0.20f, 0.20f, 0.20f, 1.0f );
    Color4 colGrid       ( 0.25f, 0.28f, 0.32f, 1.0f );

    // Reds / Pinks
    Color4 colRed        ( 0.90f, 0.10f, 0.10f, 1.0f );
    Color4 colCrimson    ( 0.70f, 0.05f, 0.10f, 1.0f );
    Color4 colPink       ( 0.95f, 0.55f, 0.70f, 1.0f );
    Color4 colMagenta    ( 0.90f, 0.10f, 0.50f, 1.0f );
    Color4 colRose       ( 0.95f, 0.30f, 0.45f, 1.0f );
    Color4 colSalmon     ( 0.95f, 0.55f, 0.45f, 1.0f );

    // Oranges / Yellows
    Color4 colOrange     ( 0.95f, 0.60f, 0.10f, 1.0f );
    Color4 colAmber      ( 1.00f, 0.75f, 0.00f, 1.0f );
    Color4 colYellow     ( 0.95f, 0.95f, 0.10f, 1.0f );
    Color4 colGold       ( 1.00f, 0.84f, 0.10f, 1.0f );
    Color4 colLime       ( 0.75f, 0.95f, 0.10f, 1.0f );
    Color4 colPeach      ( 1.00f, 0.75f, 0.55f, 1.0f );

    // Greens
    Color4 colGreen      ( 0.10f, 0.80f, 0.20f, 1.0f );
    Color4 colDarkGreen  ( 0.05f, 0.45f, 0.10f, 1.0f );
    Color4 colMint       ( 0.40f, 0.95f, 0.70f, 1.0f );
    Color4 colOlive      ( 0.50f, 0.55f, 0.10f, 1.0f );
    Color4 colTeal       ( 0.10f, 0.65f, 0.60f, 1.0f );
    Color4 colSage       ( 0.55f, 0.70f, 0.50f, 1.0f );

    // Blues
    Color4 colBlue       ( 0.10f, 0.30f, 0.90f, 1.0f );
    Color4 colNavy       ( 0.05f, 0.10f, 0.45f, 1.0f );
    Color4 colSkyBlue    ( 0.45f, 0.75f, 0.95f, 1.0f );
    Color4 colCyan       ( 0.10f, 0.60f, 0.90f, 1.0f );
    Color4 colIce        ( 0.75f, 0.90f, 1.00f, 1.0f );
    Color4 colCobalt     ( 0.15f, 0.35f, 0.75f, 1.0f );

    // Purples
    Color4 colPurple     ( 0.55f, 0.10f, 0.85f, 1.0f );
    Color4 colViolet     ( 0.70f, 0.30f, 0.95f, 1.0f );
    Color4 colIndigo     ( 0.30f, 0.10f, 0.60f, 1.0f );
    Color4 colLavender   ( 0.75f, 0.65f, 0.95f, 1.0f );
    Color4 colPlum       ( 0.50f, 0.15f, 0.45f, 1.0f );
    Color4 colOrchid     ( 0.80f, 0.45f, 0.85f, 1.0f );

    // Browns / Earth tones
    Color4 colBrown      ( 0.50f, 0.28f, 0.10f, 1.0f );
    Color4 colSienna     ( 0.65f, 0.30f, 0.10f, 1.0f );
    Color4 colTan        ( 0.80f, 0.68f, 0.50f, 1.0f );
    Color4 colSand       ( 0.90f, 0.82f, 0.62f, 1.0f );
    Color4 colChocolate  ( 0.35f, 0.18f, 0.05f, 1.0f );

    // Semi-transparent variants (useful for overlays / debug)
    Color4 colRedFaint   ( 0.90f, 0.10f, 0.10f, 0.4f );
    Color4 colGreenFaint ( 0.10f, 0.80f, 0.20f, 0.4f );
    Color4 colBlueFaint  ( 0.10f, 0.30f, 0.90f, 0.4f );
    Color4 colBlackFaint ( 0.00f, 0.00f, 0.00f, 0.5f );

    // Main loop
    FrameLimiter limiter(60.0);

    while (!glfwWindowShouldClose(window))
    {
        double dt = limiter.tick();

        renderer.beginFrame();
        processInput(window);

        // Background grid
      //  for (const auto& sq : squares)
    //        renderer.drawShape(&sq, colGrid);

        // Random coloured triangles
  //      for (int i = 0; i < static_cast<int>(triangles.size()); ++i)
//            renderer.drawShape(&triangles[i], triColors[i % 5]);

        // Named obstacles
        //renderer.drawShape(&bigSquare, colCyan);
        //renderer.drawShape(&bigTri,    colMagenta);
        //renderer.drawShape(&obstacle,  colOrange);

        renderer.drawShape(&background, colGrid);
        renderer.drawShape(&courseBorder, colBrown);
        renderer.drawShape(&courseGround, colGreen);
        renderer.drawShape(&waterFeature, colBlue);
        renderer.drawShape(&waterBridge1, colChocolate);
        renderer.drawShape(&waterBridge2, colChocolate);

        // Ball (circle)
        renderer.drawShape(&ball, colWhite);

        renderer.endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();

        printf("dt: %.4f s  |  fps: %.1f\n", dt, 1.0 / dt);

        limiter.limit();
    }

    glfwTerminate();
    return 0;
}
