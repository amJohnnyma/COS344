#ifndef SHAPE_H
#define SHAPE_H

#include "Matrix.h"
#include "Vector.h"
#include <iostream>
#include "../engine/PhysicsBody.h"

template <int n> class Renderer;

enum PhysicsType
{
    NONE = 0,
    WATER,
    WALL,
    BALL,
    BACKGROUND,
    GROUND
};

//component
template <int n>
class Shape {
    protected:
        Vector<4> color = Vector<4>{1.0f,1.0f,1.0f,1.0f};
        Vector<n> position;
        PhysicsBody<n> physicsBody;
        bool hasPhysics = false;
        PhysicsType type = NONE;

    public:
        virtual ~Shape() = default;
        virtual Shape& operator*=(const Matrix<n,n>&) = 0;
        virtual float* getPoints() const = 0;
        virtual int getNumPoints() const = 0;
        virtual void print() const = 0;
        virtual void render(Renderer<n>& r) const = 0;

        void setPhysicsType(const PhysicsType t) { type = t; }
        PhysicsType getPhysicsType()             { return type; }
        const PhysicsType getPhysicsType() const { return type; }
        PhysicsBody<n>& getPhysicsBody()         { return physicsBody; }
        const PhysicsBody<n>& getPhysicsBody() const { return physicsBody; }
        bool physicsBodyActive()                 { return hasPhysics; }
        void setVelocity(const Vector<n>& v)     { physicsBody.setVelocity(v); }
        Vector<n> getVelocity()                  { return physicsBody.vel; }
        void setPhysicsBodyRadius(float r)       { physicsBody.radius = r; }

        virtual void enablePhysics(const Vector<n>& initial_vel = Vector<n>())
        {
            physicsBody = PhysicsBody(position, initial_vel);
            hasPhysics = true;
        }

        virtual void updatePhysics(float dt);
        virtual void applyTranslation(const Vector<n>& disp) = 0;
        void setPosition(Vector<n> pos);
        Vector<n> getPosition() { return position; }

        virtual void setColor(float r, float g, float b, float a = 1.0f)
        {
            color[0]=r; color[1]=g; color[2]=b; color[3]=a;
        }

        virtual void rotate(float theta, Vector<n> rotate_point = Vector<n>(), bool hasCentroid = false) = 0;
        virtual void scale(float s) = 0;
};

#endif /*SHAPE_H*/
