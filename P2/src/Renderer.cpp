#include "../include/engine/Renderer.h"
#include "../include/shader.hpp"
#include "../include/DebugOptions.h"
#include <iostream>

template <int n>
Renderer<n>::Renderer(int width, int height)
    : programID(0), vao(0), vbo(0)
{
    loadShaders();

    // Create VAO + VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Position attribute (only vec2 for 2D right now)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    // Default ortho projection (adjust later for camera zoom/follow)
    setViewProj(
        glm::mat4(1.0f),                               // identity view for now
        glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f)  // simple 20×20 world units
    );
}

template <int n>
Renderer<n>::~Renderer() {
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

template <int n>
void Renderer<n>::loadShaders() {
    programID = LoadShaders(
        "shaders/vertex/vertex.glsl",
        "shaders/fragment/fragment.glsl"
    );
    if (programID == 0) {
        std::cerr << "Shader loading failed!\n";
    }
}

template <int n>
void Renderer<n>::setViewProj(const glm::mat4& v, const glm::mat4& p) {
    view = v;
    proj = p;
}

template <int n>
void Renderer<n>::beginFrame() {
    glClearColor(0.12f, 0.15f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    vertexData.clear();

    glUseProgram(programID);
    glBindVertexArray(vao);
    if (DebugOptions::get().wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Upload uniforms (same for all shapes this frame)
    GLint locView = glGetUniformLocation(programID, "uView");
    GLint locProj = glGetUniformLocation(programID, "uProj");
    glUniformMatrix4fv(locView, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(locProj, 1, GL_FALSE, &proj[0][0]);
}

template <int n>
void Renderer<n>::drawShape(const Shape<n>* shape, const glm::vec4& color) {
    if (!shape) return;

    float* points = shape->getPoints();
    int numPoints = shape->getNumPoints();

    if (numPoints < 3) {
        delete[] points;
        return;
    }

    // color set before drawuig
    GLint locColor = glGetUniformLocation(programID, "uColor");
    glUniform4f(locColor, 1, color[0], color[1], color[2]);

    // upload just this shape
    glBufferData(GL_ARRAY_BUFFER, 
            numPoints * 2 * sizeof(float),
            points,
            GL_DYNAMIC_DRAW);
    
    delete[] points;

    //draw this shape
    glDrawArrays(GL_TRIANGLES, 0, numPoints);

    
}

template <int n>
void Renderer<n>::endFrame() {
    if (vertexData.empty()) {
        glBindVertexArray(0);
        return;
    }
std::cout << "Drawing " << (vertexData.size()/2) << " vertices\n";
    // Upload all collected data at once
    glBufferData(GL_ARRAY_BUFFER,
                 vertexData.size() * sizeof(float),
                 vertexData.data(),
                 GL_DYNAMIC_DRAW);

    // Draw everything as triangles (need to adjust per shape later)
    // triangulated shapes or fans
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexData.size() / 2));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(1.0f);

    glBindVertexArray(0);
}

template class Renderer<2>;
