#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// We do NOT call any glm math functions.
#include <glm/glm.hpp>

#include "../math/Matrix.h"
#include "../math/Shape.h"

// Lightweight RGBA colour (replaces glm::vec4 for colours)
struct Color4 {
    float r, g, b, a;
    Color4(float r = 1, float g = 1, float b = 1, float a = 1)
        : r(r), g(g), b(b), a(a) {}
};

// Helper: build a column-major float[16] from our row-major Matrix<4,4>
// so we can hand it straight to glUniformMatrix4fv.
inline void matToColMajor(const Matrix<4,4>& m, float out[16])
{
    // Matrix<4,4>[row][col] -- OpenGL wants column-major: out[col*4 + row]
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            out[col * 4 + row] = m[row][col];
}

// Renderer<n> — 2-D (n=2) draw pipeline
template <int n>
class Renderer {
private:
    GLuint programID;
    GLuint vao;
    GLuint vbo;

    // View and projection stored as our own 4×4 matrices (column-major fed to GL)
    Matrix<4,4> view;
    Matrix<4,4> proj;

    float* m_vertexData  = nullptr;
    int m_vertexCount = 0;
    int m_vertexCap = 0;

public:
    Renderer(int width, int height);
    ~Renderer();

    // Pass our Matrix<4,4> instead of glm::mat4
    void setViewProj(const Matrix<4,4>& v, const Matrix<4,4>& p);

    void beginFrame();
    void drawShape(const Shape<n>* shape, const Color4& color);
    void endFrame();

private:
    void loadShaders();

    // Build an orthographic projection matrix from first principles.
    // Maps [left,right] x [bottom,top] x [near,far] → NDC.
    static Matrix<4,4> ortho(float l, float r, float b, float t,
                              float nearZ, float farZ);

    // Identity 4×4
    static Matrix<4,4> identity4();

    void pushFloat(float f);
};

#endif // RENDERER_H
