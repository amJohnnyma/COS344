#ifndef CAMERA_H
#define CAMERA_H

#include "../math/Vector.h"
#include "../math/Matrix.h"

template <int n>
class Camera {
    Vector<n> pos;
    float zoom;

    static Matrix<n,n> makeIdentity();

public:
    Camera() : pos(), zoom(1.0f) {}

    void setZoom(float z) { zoom = z; }
    float getZoom() const { return zoom; }

    void setPosition(const Vector<n>& p) { pos = p; }
    const Vector<n>& getPosition() const { return pos; }

    void follow(const Vector<n>& target, float alpha = 0.1f) {
        for (int i = 0; i < n; ++i)
            pos[i] += (target[i] - pos[i]) * alpha;
    }

    Matrix<n,n> getViewMatrix() const;
    Matrix<n,n> getProjMatrix(float aspect) const;
};

#endif // CAMERA_H
