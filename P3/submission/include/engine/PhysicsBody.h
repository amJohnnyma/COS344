#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include "../math/Vector.h"

template<int n>
class PhysicsBody {
public:
    Vector<n> pos;
    Vector<n> vel;
    float radius      = 0.25f;
    float mass        = 1.0f;
    float friction    = 0.9885f;
    float restitution = 0.99f;
    bool grounded = false;
    bool isCutout = false;
    bool hasBeenCutout = false;

    PhysicsBody() {}
    PhysicsBody(const Vector<n>& initial_pos, const Vector<n>& initial_vel)
        : pos(initial_pos), vel(initial_vel) {}

    void applyForce(const Vector<n> force, float dt);
    void update(float dt);

    void setVelocity(const Vector<n> v) { vel = v; }
    void setVel2D(const Vector<2>& v2)  { vel[0] = v2[0]; vel[1] = v2[1]; }
    Vector<2> pos2D() const { return Vector<2>{pos[0], pos[1]}; }
    Vector<2> vel2D() const { return Vector<2>{vel[0], vel[1]}; }
    Vector<n> getVelocity()             { return vel; }
};

#endif
