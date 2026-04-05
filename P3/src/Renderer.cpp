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
    proj(identity4())
{
    loadShaders();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Attribute 0: position — 2 floats per vertex (x, y) for 2-D rendering
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            3 * sizeof(float), (void*)0);

    glBindVertexArray(0);


    // Enable depth testing once (harmless for n=2)
    glEnable(GL_DEPTH_TEST);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    float identity[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    GLint locModel = glGetUniformLocation(programID, "uModel");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, identity);


glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}
template <int n>
void Renderer<n>::drawDebugGrid(int extent)
{
    GLint locColor = glGetUniformLocation(programID, "uColor");

    auto drawGridPlane = [&](int axis) {
        // axis: 0 = XY plane (z=0), 1 = XZ plane (y=0), 2 = YZ plane (x=0)
        for (int i = -extent; i <= extent; i++)
        {
            m_vertexCount = 0;
            float a = (float)i - 0.5f;
            float e = (float)extent;

            // Line A (varies along first axis, fixed on second)
            // Line B (fixed on first axis, varies along second)
            auto push3 = [&](float x, float y, float z) {
                pushFloat(x); pushFloat(y); pushFloat(z);
            };

            auto pushLine = [&](bool horizontal) {
                m_vertexCount = 0;
                if (axis == 0) { // XY plane, z=0
                    if (!horizontal) {
                        float x = a;
                        push3(x-0.02f, -e, 0); push3(x+0.02f, -e, 0); push3(x+0.02f, e, 0);
                        push3(x-0.02f, -e, 0); push3(x+0.02f,  e, 0); push3(x-0.02f, e, 0);
                    } else {
                        float y = (float)i + 0.5f;
                        push3(-e, y-0.02f, 0); push3(e, y-0.02f, 0); push3(e, y+0.02f, 0);
                        push3(-e, y-0.02f, 0); push3(e, y+0.02f, 0); push3(-e, y+0.02f, 0);
                    }
                } else if (axis == 1) { // XZ plane, y=0
                    if (!horizontal) {
                        float x = a;
                        push3(x-0.02f, 0, -e); push3(x+0.02f, 0, -e); push3(x+0.02f, 0, e);
                        push3(x-0.02f, 0, -e); push3(x+0.02f, 0,  e); push3(x-0.02f, 0, e);
                    } else {
                        float z = (float)i + 0.5f;
                        push3(-e, 0, z-0.02f); push3(e, 0, z-0.02f); push3(e, 0, z+0.02f);
                        push3(-e, 0, z-0.02f); push3(e, 0, z+0.02f); push3(-e, 0, z+0.02f);
                    }
                } else { // YZ plane, x=0
                    if (!horizontal) {
                        float y = a;
                        push3(0, y-0.02f, -e); push3(0, y+0.02f, -e); push3(0, y+0.02f, e);
                        push3(0, y-0.02f, -e); push3(0, y+0.02f,  e); push3(0, y-0.02f, e);
                    } else {
                        float z = (float)i + 0.5f;
                        push3(0, -e, z-0.02f); push3(0, e, z-0.02f); push3(0, e, z+0.02f);
                        push3(0, -e, z-0.02f); push3(0, e, z+0.02f); push3(0, -e, z+0.02f);
                    }
                }
                glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float), m_vertexData, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            };

            pushLine(false);
            pushLine(true);
        }
    };

    // XY plane — grey
    glUniform4f(locColor, 0.4f, 0.4f, 0.4f, 1.0f);
    drawGridPlane(0);

    // XZ plane — slightly blue-tinted grey
    glUniform4f(locColor, 0.3f, 0.3f, 0.5f, 1.0f);
    drawGridPlane(1);

    // YZ plane — slightly green-tinted grey
    glUniform4f(locColor, 0.3f, 0.5f, 0.3f, 1.0f);
    drawGridPlane(2);

    // Mark origin with a red cube (one quad per face)
    glUniform4f(locColor, 1.0f, 0.0f, 0.0f, 1.0f);
    const float h = 0.15f;
    auto pushQuad = [&](
        float x0,float y0,float z0,
        float x1,float y1,float z1,
        float x2,float y2,float z2,
        float x3,float y3,float z3)
    {
        m_vertexCount = 0;
        pushFloat(x0);pushFloat(y0);pushFloat(z0);
        pushFloat(x1);pushFloat(y1);pushFloat(z1);
        pushFloat(x2);pushFloat(y2);pushFloat(z2);
        pushFloat(x0);pushFloat(y0);pushFloat(z0);
        pushFloat(x2);pushFloat(y2);pushFloat(z2);
        pushFloat(x3);pushFloat(y3);pushFloat(z3);
        glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float), m_vertexData, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    };

    // +Z / -Z faces
    pushQuad(-h,-h, h,  h,-h, h,  h, h, h,  -h, h, h);
    pushQuad(-h,-h,-h,  h,-h,-h,  h, h,-h,  -h, h,-h);
    // +Y / -Y faces
    pushQuad(-h, h,-h,  h, h,-h,  h, h, h,  -h, h, h);
    pushQuad(-h,-h,-h,  h,-h,-h,  h,-h, h,  -h,-h, h);
    // +X / -X faces
    pushQuad( h,-h,-h,  h, h,-h,  h, h, h,  h,-h, h);
    pushQuad(-h,-h,-h, -h, h,-h, -h, h, h, -h,-h, h);
}
template <int n>
void Renderer<n>::drawFaceNormals(const Shape<3>* shape, float arrowLength, float headSize)
{
    if constexpr (n != 3) return;
    if (!shape) return;

    float* points = shape->getPoints();
    int numPoints = shape->getNumPoints();

    if (numPoints < 3) {
        delete[] points;
        return;
    }

    GLint locColor = glGetUniformLocation(programID, "uColor");
    glUniform4f(locColor, 0.0f, 1.0f, 0.0f, 1.0f);   // bright green normals

    for (int i = 0; i < numPoints; i += 3)
    {
        // Get three vertices (x,y,z)
        float ax = points[i*3 + 0],     ay = points[i*3 + 1],     az = points[i*3 + 2];
        float bx = points[(i+1)*3 + 0], by = points[(i+1)*3 + 1], bz = points[(i+1)*3 + 2];
        float cx = points[(i+2)*3 + 0], cy = points[(i+2)*3 + 1], cz = points[(i+2)*3 + 2];

        // Face center
        float centerX = (ax + bx + cx) / 3.0f;
        float centerY = (ay + by + cy) / 3.0f;
        float centerZ = (az + bz + cz) / 3.0f;

        // Edge vectors
        float e1x = bx - ax, e1y = by - ay, e1z = bz - az;
        float e2x = cx - ax, e2y = cy - ay, e2z = cz - az;

        // Cross product (right-hand rule)
        float nx = e1y * e2z - e1z * e2y;
        float ny = e1z * e2x - e1x * e2z;
        float nz = e1x * e2y - e1y * e2x;

        // Normalize manually
        float len = std::sqrt(nx*nx + ny*ny + nz*nz);
        if (len < 0.0001f) continue;        // degenerate triangle

        nx /= len;
        ny /= len;
        nz /= len;

        // Tip of the arrow
        float tipX = centerX + nx * arrowLength;
        float tipY = centerY + ny * arrowLength;
        float tipZ = centerZ + nz * arrowLength;

        m_vertexCount = 0;

        // 1. Shaft (line from center to tip)
        pushFloat(centerX); pushFloat(centerY); pushFloat(centerZ);
        pushFloat(tipX);    pushFloat(tipY);    pushFloat(tipZ);

        // 2. Arrow head - two short lines forming a V
        // Create two perpendicular directions in the plane perpendicular to normal
        // Simple fallback perpendicular vectors
        float px, py, pz;
        if (std::abs(nz) > 0.1f) {
            px = ny; py = -nx; pz = 0.0f;   // perpendicular to normal
        } else {
            px = 0.0f; py = nz; pz = -ny;
        }

        float plen = std::sqrt(px*px + py*py + pz*pz);
        if (plen > 0.0001f) {
            px /= plen;
            py /= plen;
            pz /= plen;
        } else {
            px = 1.0f; py = 0.0f; pz = 0.0f;
        }

        // Second perpendicular = normal × first perpendicular
        float qx = ny * pz - nz * py;
        float qy = nz * px - nx * pz;
        float qz = nx * py - ny * px;

        float scale = headSize * 0.6f;

        // Head point 1
        float h1x = tipX - nx * headSize + px * scale;
        float h1y = tipY - ny * headSize + py * scale;
        float h1z = tipZ - nz * headSize + pz * scale;

        // Head point 2
        float h2x = tipX - nx * headSize - px * scale;
        float h2y = tipY - ny * headSize - py * scale;
        float h2z = tipZ - nz * headSize - pz * scale;

        // Draw head lines: tip → h1 and tip → h2
        pushFloat(tipX); pushFloat(tipY); pushFloat(tipZ);
        pushFloat(h1x);  pushFloat(h1y);  pushFloat(h1z);

        pushFloat(tipX); pushFloat(tipY); pushFloat(tipZ);
        pushFloat(h2x);  pushFloat(h2y);  pushFloat(h2z);

        // Upload and draw the 3 lines (6 vertices)
        glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float), m_vertexData, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINES, 0, 6);
    }

    delete[] points;
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
                // vertex A (x, y, z)
                pushFloat(points[a * n + 0]);
                pushFloat(points[a * n + 1]);
                if constexpr (n == 3) 
                {
                    pushFloat(points[a * n + 2]);
                }
                else                  pushFloat(-10.0f);

                // vertex B (x, y, z)
                pushFloat(points[b * n + 0]);
                pushFloat(points[b * n + 1]);
                if constexpr (n == 3) pushFloat(points[b * n + 2]);
                else                  pushFloat(-10.0f);
            }
        }
    }
    else
    {
        for (int i = 0; i < numPoints; ++i) {
            pushFloat(points[i * n + 0]);
            pushFloat(points[i * n + 1]);
            if constexpr (n == 3) {
                pushFloat(points[i * n + 2]);
            } else {
                pushFloat(0.0f);
            }
        }
    }

    delete[] points;

    GLint locColor = glGetUniformLocation(programID, "uColor");
    glUniform4f(locColor, color.r, color.g, color.b, color.a);

    glBufferData(GL_ARRAY_BUFFER,
            m_vertexCount * sizeof(float),
            m_vertexData,
            GL_DYNAMIC_DRAW);
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);
glFrontFace(GL_CCW);

    if (DebugOptions::get().wireframe)
    {
        glDrawArrays(GL_LINES, 0, (numPoints/3)*6);

        if constexpr (n==3)
            drawFaceNormals(shape, 1.5f, 0.4f);
    }
    else
        glDrawArrays(GL_TRIANGLES,0,numPoints);
glDisable(GL_CULL_FACE);


}

    template <int n>
Matrix<4,4> Renderer<n>::perspective(float fovY, float aspect, float nearZ, float farZ)
{
    float f = 1.0f / std::tan(fovY * 0.5f);
    Matrix<4,4> m;   // zeros
    m[0][0] = f / aspect;
    m[1][1] = f;
    m[2][2] = (farZ + nearZ) / (nearZ - farZ);
    m[2][3] = (2.0f * farZ * nearZ) / (nearZ - farZ);
    m[3][2] = -1.0f;
    return m;
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

template class Renderer<3>;
