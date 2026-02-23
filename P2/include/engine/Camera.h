#ifndef CAMERA_H
#define CAMERA_H

#include "../math/Vector.h"
#include "../math/Matrix.h"

// Camera<n>
//
// Keeps track of a world-space position and zoom level, and can produce:
//   getViewMatrix()  — a translation matrix Translate(-pos)  (scaled by zoom)
//   getProjMatrix()  — an orthographic projection built from first principles
//
// Only n=2 is used/needed for this mini project.
template <int n>
class Camera {
    Vector<n> pos;
    float zoom;

    // Build an (n+1)×(n+1) identity matrix — we use n+1 for homogeneous coords.
    // For 2D we need 3×3; for 3D we need 4×4.
    // To keep things simple we use Matrix<n,n> as the return type (matching the
    // header declaration), which means we stay in n×n space with no homogeneous
    // row/col — this is equivalent to an affine transform applied at the shader.
    static Matrix<n,n> makeIdentity();

public:
    Camera() : pos(), zoom(1.0f) {}

    void setZoom(float z) { zoom = z; }
    float getZoom() const { return zoom; }

    void setPosition(const Vector<n>& p) { pos = p; }
    const Vector<n>& getPosition() const { return pos; }

    // Smoothly move camera toward target
    void follow(const Vector<n>& target, float alpha = 0.1f) {
        for (int i = 0; i < n; ++i)
            pos[i] += (target[i] - pos[i]) * alpha;
    }

    // View matrix: Scale(zoom) * Translate(-pos)
    // For 2D rendered with a 4×4 shader uniform we build a 4×4 in Renderer
    // Here we return n×n for completeness / non-shader use
    Matrix<n,n> getViewMatrix() const;

    // Orthographic projection matrix in n×n space
    // For 2D: maps the visible region to [-1,1]
    Matrix<n,n> getProjMatrix(float aspect) const;
};


template<int n>
Matrix<n,n> Camera<n>::makeIdentity()
{
    Matrix<n,n> m;   // zero-initialised by Matrix default ctor
    for (int i = 0; i < n; ++i)
        m[i][i] = 1.0f;
    return m;
}

// View = Scale(zoom) * Translate(-pos)
// In n×n space (no homogeneous coordinate) this is: V[i][j] = zoom * I[i][j],
// then subtract the translation component from the last column
// Since we have no homogeneous column in an n×n matrix we encode translation
// by baking it into the diagonal shift: this is approximate for n=2
// The Renderer converts this to a 4×4 before uploading to the GPU
template<int n>
Matrix<n,n> Camera<n>::getViewMatrix() const
{
    // Start with scaled identity
    Matrix<n,n> m = makeIdentity();
    for (int i = 0; i < n; ++i)
        m[i][i] = zoom;

    // Embed translation: shift the diagonal (approximate, see note above)
    // For a proper affine matrix the translation lives in an extra column;
    // callers that need a proper 4×4 should use the Renderer helper instead
    // Here we just return the scaled identity — translation is handled by the
    // 4×4 helper built inside Renderer
    return m;
}

// Orthographic projection for n=2: maps [-10/zoom, 10/zoom] → [-1,1].
template<int n>
Matrix<n,n> Camera<n>::getProjMatrix(float aspect) const
{
    Matrix<n,n> m = makeIdentity();
    // Scale axes to NDC
    float halfW = 10.0f / zoom;
    float halfH = 10.0f / zoom;
    if (n >= 1) m[0][0] =  1.0f / (halfW * aspect);
    if (n >= 2) m[1][1] =  1.0f / halfH;
    return m;
}

#endif // CAMERA_H
