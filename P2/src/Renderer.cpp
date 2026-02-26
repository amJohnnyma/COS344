#include "../include/engine/Renderer.h"
#include "../include/shader.hpp"
#include "../include/DebugOptions.h"
#include <iostream>
#include <cmath>    // for 2*pi in circle generation (not from glm)

// Static helpers

template <int n>
Matrix<4,4> Renderer<n>::identity4()
{
    Matrix<4,4> m;   // default-constructed to all zeros
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
    return m;
}

// Standard orthographic projection built from first principles.
// Maps x ∈ [l,r], y ∈ [b,t], z ∈ [nearZ,farZ] → NDC [-1,1]³.
template <int n>
Matrix<4,4> Renderer<n>::ortho(float l, float r, float b, float t,
                                float nearZ, float farZ)
{
    Matrix<4,4> m;   // zeros

    m[0][0] =  2.0f / (r - l);
    m[1][1] =  2.0f / (t - b);
    m[2][2] = -2.0f / (farZ - nearZ);
    m[3][3] =  1.0f;

    m[0][3] = -(r + l) / (r - l);
    m[1][3] = -(t + b) / (t - b);
    m[2][3] = -(farZ + nearZ) / (farZ - nearZ);

    return m;
}

// Constructor
template <int n>
Renderer<n>::Renderer(int width, int height)
    : programID(0), vao(0), vbo(0),
      view(identity4()),
      proj(ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f))
{
    loadShaders();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Attribute 0: position — 2 floats per vertex (x, y) for 2-D rendering
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

template <int n>
Renderer<n>::~Renderer()
{
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

template <int n>
void Renderer<n>::loadShaders()
{
    programID = LoadShaders(
        "shaders/vertex/vertex.glsl",
        "shaders/fragment/fragment.glsl"
    );
    if (programID == 0)
        std::cerr << "Shader loading failed!\n";
}

template <int n>
void Renderer<n>::setViewProj(const Matrix<4,4>& v, const Matrix<4,4>& p)
{
    view = v;
    proj = p;
}

template <int n>
void Renderer<n>::beginFrame()
{
    glClearColor(0.12f, 0.15f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    vertexData.clear();

    glUseProgram(programID);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    if (DebugOptions::get().wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Upload view/proj uniforms — convert our row-major matrix to
    // the column-major flat array that OpenGL expects.
    float colMajorView[16];
    float colMajorProj[16];
    matToColMajor(view, colMajorView);
    matToColMajor(proj, colMajorProj);

    GLint locView = glGetUniformLocation(programID, "uView");
    GLint locProj = glGetUniformLocation(programID, "uProj");
    glUniformMatrix4fv(locView, 1, GL_FALSE, colMajorView);
    glUniformMatrix4fv(locProj, 1, GL_FALSE, colMajorProj);
}

// drawShape — uploads and draws a single shape immediately.
template <int n>
void Renderer<n>::drawShape(const Shape<n>* shape, const Color4& color)
{
    if (!shape) return;

    float* points    = shape->getPoints();
    int    numPoints = shape->getNumPoints();

    if (numPoints < 3) {
        delete[] points;
        return;
    }
    // Build a xy-only buffer
    std::vector<float> xy;
    xy.reserve(numPoints * 2);
    for (int i = 0; i < numPoints; ++i) {
        xy.push_back(points[i * n + 0]);   // x
        xy.push_back(points[i * n + 1]);   // y
        // z (and higher) intentionally ignored — z is a logical layer, not depth // for P2 atleast
    }

    delete[] points;

    GLint locColor = glGetUniformLocation(programID, "uColor");
    glUniform4f(locColor, color.r, color.g, color.b, color.a);

    glBufferData(GL_ARRAY_BUFFER,
                 xy.size() * sizeof(float),
                 xy.data(),
                 GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, numPoints);

}

// endFrame — handles any deferred batch data (currently unused — each shape
// is drawn immediately in drawShape), then resets GL state.
template <int n>
void Renderer<n>::endFrame()
{
    if (!vertexData.empty()) {
        std::cout << "Drawing " << (vertexData.size() / 2) << " deferred vertices\n";

        glBufferData(GL_ARRAY_BUFFER,
                     vertexData.size() * sizeof(float),
                     vertexData.data(),
                     GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0,
                     static_cast<GLsizei>(vertexData.size() / 2));
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(1.0f);
    glBindVertexArray(0);
}

template class Renderer<2>;
template class Renderer<3>;
