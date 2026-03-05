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
    delete[] m_vertexData;
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

    m_vertexCount = 0;

    glUseProgram(programID);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);


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
template <int n>
void Renderer<n>::drawDebugGrid(int extent)
{
    GLint locColor = glGetUniformLocation(programID, "uColor");

    // grid lines
    glUniform4f(locColor, 0.4f, 0.4f, 0.4f, 1.0f);

    for (int i = -extent; i <= extent; i++)
    {
        m_vertexCount = 0;
        // vertical line as thin quad
        float x = (float)i - 0.5f;
        pushFloat(x - 0.02f); pushFloat(-(float)extent);
        pushFloat(x + 0.02f); pushFloat(-(float)extent);
        pushFloat(x + 0.02f); pushFloat( (float)extent);
        pushFloat(x - 0.02f); pushFloat(-(float)extent);
        pushFloat(x + 0.02f); pushFloat( (float)extent);
        pushFloat(x - 0.02f); pushFloat( (float)extent);
        glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float), m_vertexData, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        m_vertexCount = 0;
        // horizontal line as thin quad
        float y = (float)i + 0.5f;
        pushFloat(-(float)extent); pushFloat(y - 0.02f);
        pushFloat( (float)extent); pushFloat(y - 0.02f);
        pushFloat( (float)extent); pushFloat(y + 0.02f);
        pushFloat(-(float)extent); pushFloat(y - 0.02f);
        pushFloat( (float)extent); pushFloat(y + 0.02f);
        pushFloat(-(float)extent); pushFloat(y + 0.02f);
        glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float), m_vertexData, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // mark 0,0 with a red square
    glUniform4f(locColor, 1.0f, 0.0f, 0.0f, 1.0f);
    m_vertexCount = 0;
    pushFloat(-0.15f); pushFloat(-0.15f);
    pushFloat( 0.15f); pushFloat(-0.15f);
    pushFloat( 0.15f); pushFloat( 0.15f);
    pushFloat(-0.15f); pushFloat(-0.15f);
    pushFloat( 0.15f); pushFloat( 0.15f);
    pushFloat(-0.15f); pushFloat( 0.15f);
    glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float), m_vertexData, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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

    m_vertexCount = 0;
    // Build a xy-only buffer
    if (DebugOptions::get().wireframe)
    {
        // GL_LINES expects explicit pairs: for each triangle (i, i+1, i+2)
        // emit 3 edges — each as two vertices
        for (int i = 0; i < numPoints; i += 3)
        {
            int tri[3] = { i, i+1, i+2 };
            for (int e = 0; e < 3; ++e)
            {
                int a = tri[e];
                int b = tri[(e + 1) % 3];
                pushFloat(points[a * n + 0]);
                pushFloat(points[a * n + 1]);
                pushFloat(points[b * n + 0]);
                pushFloat(points[b * n + 1]);
            }
        }
    }
    else
    {
        for (int i = 0; i < numPoints; ++i) {
            pushFloat(points[i * n + 0]);
            pushFloat(points[i * n + 1]);
        }
    }

    delete[] points;

    GLint locColor = glGetUniformLocation(programID, "uColor");
    glUniform4f(locColor, color.r, color.g, color.b, color.a);

    glBufferData(GL_ARRAY_BUFFER,
                 m_vertexCount * sizeof(float),
                 m_vertexData,
                 GL_DYNAMIC_DRAW);

    if (DebugOptions::get().wireframe)
        glDrawArrays(GL_LINES, 0, (numPoints/3)*6);
    else
        glDrawArrays(GL_TRIANGLES,0,numPoints);

}

// endFrame — handles any deferred batch data (currently unused — each shape
// is drawn immediately in drawShape), then resets GL state.
template <int n>
void Renderer<n>::endFrame()
{

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(1.0f);
    glBindVertexArray(0);
}

template <int n>
void Renderer<n>::pushFloat(float f)
{
    if (m_vertexCount >= m_vertexCap) {
        int newCap = m_vertexCap == 0 ? 1024 : m_vertexCap * 2;
        float* newBuf = new float[newCap];
        for (int i = 0; i < m_vertexCount; i++) newBuf[i] = m_vertexData[i];
        delete[] m_vertexData;
        m_vertexData = newBuf;
        m_vertexCap  = newCap;
    }
    m_vertexData[m_vertexCount++] = f;
}

template class Renderer<2>;
template class Renderer<3>;
