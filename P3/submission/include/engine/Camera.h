#ifndef CAMERA_H
#define CAMERA_H

#include "../math/Matrix.h"
#include "../math/Vector.h"
#include <cmath>

struct Camera
{
    // Position and orientation
    Vector<3> position;
    float yaw   = 0.0f;   // rotation around Y (radians)
    float pitch = 0.0f;   // rotation around X (radians)

    // Projection settings
    float fovY   = M_PI / 3.0f;
    float aspect = 1.0f;
    float nearZ  = 0.1f;
    float farZ   = 100.0f;

    Camera()
        : position(new float[3]{0.f, 0.f, 20.f})
        {}

    // Orbit around a target point at a given radius
    void orbitAround(const Vector<3>& target, float radius)
    {
        position[0] = target[0] + radius * std::sin(yaw) * std::cos(pitch);
        position[1] = target[1] + radius * std::sin(pitch);
        position[2] = target[2] + radius * std::cos(yaw) * std::cos(pitch);
    }

    Matrix<4,4> getView() const
    {
        float cy = std::cos(yaw),   sy = std::sin(yaw);
        float cp = std::cos(pitch), sp = std::sin(pitch);

        // Row-major view = Rotation^T * Translation
        Matrix<4,4> v;
        // Right vector
        v[0][0] = cy;        v[0][1] = 0;   v[0][2] = -sy;
        // Up vector  
        v[1][0] = sy*sp;     v[1][1] = cp;  v[1][2] = cy*sp;
        // Forward vector (negated for OpenGL -Z forward)
        v[2][0] = sy*cp;     v[2][1] = -sp; v[2][2] = cy*cp;
        // Translation (dot of rotation rows with -position)
        v[0][3] = -(v[0][0]*position[0] + v[0][1]*position[1] + v[0][2]*position[2]);
        v[1][3] = -(v[1][0]*position[0] + v[1][1]*position[1] + v[1][2]*position[2]);
        v[2][3] = -(v[2][0]*position[0] + v[2][1]*position[1] + v[2][2]*position[2]);
        v[3][3] = 1.0f;
        return v;
    }

    Matrix<4,4> getProj() const
    {
        float f = 1.0f / std::tan(fovY * 0.5f);
        Matrix<4,4> m;
        m[0][0] = f / aspect;
        m[1][1] = f;
        m[2][2] = (farZ + nearZ) / (nearZ - farZ);
        m[2][3] = (2.0f * farZ * nearZ) / (nearZ - farZ);
        m[3][2] = -1.0f;
        return m;
    }
    // Forward vector (direction the camera is looking)
    Vector<3> getForward() const
    {
        Vector<3> f(new float[3]{
                std::sin(yaw) * std::cos(pitch),
                -std::sin(pitch),
                std::cos(yaw) * std::cos(pitch)  // note: negative removed, handled by view
                });
        return f;
    }

    // Right vector (perpendicular to forward, on the XZ plane)
    Vector<3> getRight() const
    {
        Vector<3> r(new float[3]{
                std::cos(yaw),
                0.0f,
                -std::sin(yaw)
                });
        return r;
    }

    // Up vector (world up — good enough for most cases)
    Vector<3> getUp() const
    {
        return Vector<3>{0.0f, 1.0f, 0.0f};
    }
};

#endif
