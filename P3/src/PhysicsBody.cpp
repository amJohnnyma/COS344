#include "../include/engine/PhysicsBody.h"

template<int n>
void PhysicsBody<n>::applyForce(const Vector<n> force, float dt)
{
    vel = vel + force * (dt / mass);
}

template<int n>
void PhysicsBody<n>::update(float dt)
{
    pos = pos + vel * dt;
    vel = vel * friction;
}

template class PhysicsBody<2>;
template class PhysicsBody<3>;
