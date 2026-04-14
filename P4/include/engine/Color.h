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
    const Color4 White     { 0.70f, 0.70f, 0.70f, 1.0f };
    const Color4 Magenta   { 1.00f, 0.00f, 1.00f, 1.0f };
    const Color4 DarkGray  { 0.25f, 0.25f, 0.25f, 1.0f };

    namespace Pastel {
        const Color4 Grid      { 0.625f, 0.640f, 0.660f, 1.0f };
        const Color4 Brown     { 0.750f, 0.640f, 0.550f, 1.0f };
        const Color4 Green     { 0.550f, 0.900f, 0.600f, 1.0f };
        const Color4 Blue      { 0.550f, 0.650f, 0.950f, 1.0f };
        const Color4 Chocolate { 0.675f, 0.590f, 0.525f, 1.0f };
        const Color4 Amber     { 1.000f, 0.875f, 0.500f, 1.0f };
        const Color4 White     { 1.000f, 1.000f, 1.000f, 1.0f };
        const Color4 Magenta   { 1.000f, 0.500f, 1.000f, 1.0f };
        const Color4 DarkGray  { 0.625f, 0.625f, 0.625f, 1.0f };
    }
}

#endif
