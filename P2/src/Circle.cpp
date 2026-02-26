#include "../include/math/Circle.h"
#include <cmath>

template<int n>
Circle<n>::Circle(const Vector<n>& c, float r, int segs)
    : center(c), radius(r), segments(segs)
{
    float step = 2.0f * M_PI / segs;
    for (int i = 0; i < segs; i++) {
        float theta0 = step * i;
        float theta1 = step * (i + 1);

        Vector<n> p1, p2;
        p1[0] = center[0] + radius * std::cos(theta0);
        p1[1] = center[1] + radius * std::sin(theta0);
        p2[0] = center[0] + radius * std::cos(theta1);
        p2[1] = center[1] + radius * std::sin(theta1);

        children.push_back(new Triangle<n>(center, p1, p2));
    }

    this->position = c;
}

// Copy constructor
template<int n>
Circle<n>::Circle(const Circle<n>& other)
    : center(other.center), radius(other.radius), segments(other.segments)
{
}

// operator*= — transform the center; radius is unchanged (assumes uniform scale
// or no scale — for a proper ellipse the caller should adjust).
// For a pure rotation/translation matrix this is correct.
template<int n>
Circle<n>& Circle<n>::operator*=(const Matrix<n,n>& mat)
{
    // Transform center point: new_center[i] = sum_j mat[i][j] * center[j]
    Vector<n> old = center;
    for (int i = 0; i < n; ++i) {
        float sum = 0.0f;
        for (int j = 0; j < n; ++j)
            sum += mat[i][j] * old[j];
        center[i] = sum;
    }
    return *this;
}


// getPoints — decompose the disc into `segments` triangles (triangle fan)
// expressed as individual triangles so GL_TRIANGLES can be used directly.
//
// Each triangle i:
//   vertex 0 — center
//   vertex 1 — point on circumference at angle theta_i
//   vertex 2 — point on circumference at angle theta_{i+1}
//
// We only output the x,y components (n=2) for the renderer.
// For n > 2 we copy higher components from center.
//
// Returns (segments * 3 * n) floats — caller must delete[].
template<int n>
float* Circle<n>::getPoints() const
{
    const int totalFloats = segments * 3 * n;
    float* pts = new float[totalFloats];
    int idx = 0;

    const float twoPi = 2.0f * 3.14159265358979323846f;

    for (int i = 0; i < segments; ++i) {
        float theta0 = (twoPi * i)       / segments;
        float theta1 = (twoPi * (i + 1)) / segments;

        // Vertex 0 — center
        for (int k = 0; k < n; ++k)
            pts[idx++] = center[k];

        // Vertex 1 — circumference at theta0
        pts[idx++] = center[0] + radius * std::cos(theta0);    // x
        if (n >= 2) pts[idx++] = center[1] + radius * std::sin(theta0);  // y
        for (int k = 2; k < n; ++k) pts[idx++] = center[k];   // z, w, ...

        // Vertex 2 — circumference at theta1
        pts[idx++] = center[0] + radius * std::cos(theta1);    // x
        if (n >= 2) pts[idx++] = center[1] + radius * std::sin(theta1);  // y
        for (int k = 2; k < n; ++k) pts[idx++] = center[k];
    }

    return pts;
}

template <int n>
void Circle<n>::rotate(float theta, Vector<n> rotate_point, bool hasCentroid)
{
    Vector<n> pivot;

    if(hasCentroid)
    {
        pivot = rotate_point;
    }
    else {
        pivot = center;
    }

    // only 2D
    if constexpr (n == 2)
    {
        const float c = std::cos(theta);
        const float s = std::sin(theta);
        const float Cx = pivot[0];
        const float Cy = pivot[1];

        const float dx = center[0] - Cx;
        const float dy = center[1] - Cy;

        center[0] = Cx + dx * c - dy * s;
        center[1] = Cy + dx * s + dy * c;

        for (Shape<n>* child : children)
        {
            child->rotate(theta, pivot, true);
        }
    }
}


template class Circle<2>;
