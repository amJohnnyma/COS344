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
    float friction     = 0.985f;  // 0.98–0.995 feels nice for balls
    float restitution  = 0.82f;   // bounciness 

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

    // void applyGravity() { vel.y -= 9.81f * dt; }
};

#endif
