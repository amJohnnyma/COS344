#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include "../math/Vector.h"
using Vec2 = Vector<2>;
class PhysicsBody {
public:
    Vec2 pos;
    Vec2 vel;
    float radius       = 0.25f;   // typical mini-golf ball
    float mass         = 1.0f;
    float friction     = 0.985f;  // 0.98–0.995 feels nice for balls
    float restitution  = 0.82f;   // bounciness 

    PhysicsBody() {}

    void applyForce(const Vec2& force, float dt) {
        vel = vel + force * (dt / mass);
    }

    void update(float dt) {
        std::cout << "Phys update" << std::endl;
        pos = pos + vel * dt;
        vel = vel * friction;   // simple air/rolling drag
    }

    // void applyGravity() { vel.y -= 9.81f * dt; }
};

#endif
