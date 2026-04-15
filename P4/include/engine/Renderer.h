#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// We do NOT call any glm math functions.
#include <glm/glm.hpp>

#include "../math/Matrix.h"
#include "../math/Shape.h"

#include "Color.h"


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

    // temp
    GLint uLightPos = -1;
    GLint uLightColor = -1;
    GLint uLightRadius = -1;
GLuint m_checkerTexture;
    GLint uUseTextureLoc;
    GLint uTextureLoc;

public:
    Renderer(int width, int height);
    ~Renderer();

    // Pass our Matrix<4,4> instead of glm::mat4
    void setViewProj(const Matrix<4,4>& v, const Matrix<4,4>& p);

    void beginFrame();
    void drawShape(const Shape<n>* shape, const Color4& color);
    void endFrame();

    void drawDebugGrid(int extent);

void beginStencilMask();
void beginStencilCutout();
void endStencilMask();
void endStencilCutout();
void updatePointLight(const Vector<n>& pos,const Vector<n>& col,const float& radius);
void setPointLightPos(const Vector<n>& pos);
void setPointLightCol(const Vector<n>& col);
void setPointLightRad(const float& rad);
void setUseTexture(bool use) { 
        glUseProgram(programID);
        glUniform1i(uUseTextureLoc, use ? 1 : 0); 
    }
private:
    void loadShaders();

    // Build an orthographic projection matrix from first principles.
    // Maps [left,right] x [bottom,top] x [near,far] → NDC.
    static Matrix<4,4> ortho(float l, float r, float b, float t,
                              float nearZ, float farZ);

    // Identity 4×4
    static Matrix<4,4> identity4();
    Matrix<4,4> perspective(float fovY, float aspect, float nearZ, float farZ);

    void pushFloat(float f);
void drawFaceNormals(const Shape<3>* shape, float arrowLength = 0.3f, float headSize = 0.08f);


GLuint createCheckerboardTexture();
};


#endif // RENDERER_H
