#ifndef SHAPE_H
#define SHAPE_H

#include "Matrix.h"
#include "Vector.h"
#include <iostream>
#include "../engine/PhysicsBody.h"

template <int n> class Renderer;


//component
template <int n>
class Shape {
    protected:
        Vector<4> color = Vector<4>{1.0f,1.0f,1.0f,1.0f};
        Vector<2> position; // the center position
        PhysicsBody physicsBody;

    public:
        virtual ~Shape() = default;
        virtual Shape& operator*=(const Matrix<n,n>&) = 0;
        virtual float* getPoints() const = 0;   // caller must delete[]
        virtual int getNumPoints() const = 0;
        virtual void print() const = 0;
        virtual void render(Renderer<n>& r) const = 0;

        void addPhysicsBody() {physicsBody.pos = position; physicsBody.vel = Vector<2>{0.1f, 0.f};}
        void updatePhysics(float dt) {
            physicsBody.update(dt); 
            position = physicsBody.pos;
            updatePosition();
        }

        void setPosition(Vector<2> pos) {position = pos;}

        virtual void updatePosition() = 0;

        virtual void setColor(float r, float g, float b, float a = 1.0f)
        {
            color[0]=r;color[1]=g;color[2]=b;color[3]=a;
        }

        virtual void rotate(float theta, Vector<n> rotate_point = Vector<n>(), bool hasCentroid = false) = 0;
};

#endif /*SHAPE_H*/
