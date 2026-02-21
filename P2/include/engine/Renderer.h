#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // ortho, lookAt, etc.
#include "../math/Shape.h"
#include <vector>

template <int n>
class Renderer {
private:
    GLuint programID;
    GLuint vao;
    GLuint vbo;

    glm::mat4 view;
    glm::mat4 proj;

    // Temporary storage  collected every frame
    std::vector<float> vertexData;

public:
    Renderer(int width, int height);
    ~Renderer();

    void setViewProj(const glm::mat4& v, const glm::mat4& p);

    void beginFrame();
    void drawShape(const Shape<n>* shape, const glm::vec4& color = glm::vec4(1,1,1,1));
    void endFrame();

private:
    void loadShaders();
};

#endif
