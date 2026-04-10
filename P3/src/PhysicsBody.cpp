#include "../include/engine/PhysicsBody.h"

template<int n>
void PhysicsBody<n>::applyForce(const Vector<n> force, float dt)
{
    vel = vel + force * (dt / mass);
}

template<int n>
void PhysicsBody<n>::update(float dt)
{
    if constexpr (n == 3)
    {
        if(!grounded)
            vel[1] -= 9.8f * dt; 
    }
    pos = pos + vel * dt;
    vel = vel * friction;

    grounded = false;
}

template class PhysicsBody<2>;
template class PhysicsBody<3>;
