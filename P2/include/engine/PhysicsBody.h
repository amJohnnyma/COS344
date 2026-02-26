#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include "../math/Vector.h"
template<int n>
class PhysicsBody {
public:
    Vector<n> pos;
    Vector<n> vel;
    float radius       = 0.25f;   // typical mini-golf ball
    float mass         = 1.0f;
    float friction     = 0.9885f;  // 0.98–0.995 feels nice for balls
    float restitution  = 0.99f;   // bounciness 

    PhysicsBody() {}
    PhysicsBody(const Vector<n>& initial_pos, const Vector<n>& initial_vel) : pos(initial_pos), vel(initial_vel) {}

    void applyForce(const Vector<n> force, float dt) {
        vel = vel + force * (dt / mass);
    }

    void update(float dt) {
        pos = pos + vel * dt;
        vel = vel * friction;   // simple air/rolling drag
    }
    void setVelocity(const Vector<n> v) { vel = v; }
    // Write a 2D result back into the n-D velocity (z unchanged)
    void setVel2D(const Vector<2>& v2) { vel[0] = v2[0]; vel[1] = v2[1]; }
    // layer-aware collision, not for geometry.
    Vector<2> pos2D() const { return Vector<2>{pos[0], pos[1]}; }
    Vector<2> vel2D() const { return Vector<2>{vel[0], vel[1]}; }

    // void applyGravity() { vel.y -= 9.81f * dt; }
};

#endif
