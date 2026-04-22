#include "../include/math/Shape.h"

template<int n>
void Shape<n>::updatePhysics(float dt)
{
    if (!hasPhysics) return;

    Vector<n> old_pos = position;
    physicsBody.update(dt);
    position = physicsBody.pos;
    Vector<n> displacement = position - old_pos;
    applyTranslation(displacement);
}

template<int n>
void Shape<n>::setPosition(Vector<n> pos)
{
    Vector<n> delta = pos - position;
    applyTranslation(delta);
    position = pos;
}

template class Shape<2>;
template class Shape<3>;
