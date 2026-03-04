#ifndef COLOR_H
#define COLOR_H

// Lightweight RGBA colour (replaces glm::vec4 for colours)
struct Color4 {
    float r, g, b, a;
    Color4(float r = 1, float g = 1, float b = 1, float a = 1)
        : r(r), g(g), b(b), a(a) {}
};
namespace Colors {
    const Color4 Grid      { 0.25f, 0.28f, 0.32f, 1.0f };
    const Color4 Brown     { 0.50f, 0.28f, 0.10f, 1.0f };
    const Color4 Green     { 0.10f, 0.80f, 0.20f, 1.0f };
    const Color4 Blue      { 0.10f, 0.30f, 0.90f, 1.0f };
    const Color4 Chocolate { 0.35f, 0.18f, 0.05f, 1.0f };
    const Color4 Amber     { 1.00f, 0.75f, 0.00f, 1.0f };
    const Color4 White     { 1.00f, 1.00f, 1.00f, 1.0f };
    const Color4 Magenta   { 1.00f, 0.00f, 1.00f, 1.0f };
    const Color4 DarkGray  { 0.25f, 0.25f, 0.25f, 1.0f };
}

#endif
