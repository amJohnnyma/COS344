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
        bool hasPhysics = false;

    public:
        virtual ~Shape() = default;
        virtual Shape& operator*=(const Matrix<n,n>&) = 0;
        virtual float* getPoints() const = 0;   // caller must delete[]
        virtual int getNumPoints() const = 0;
        virtual void print() const = 0;
        virtual void render(Renderer<n>& r) const = 0;

        void setVelocity(const Vector<2>& v) {physicsBody.setVelocity(v);}
        void enablePhysics(const Vector<2>& initial_vel = Vector<2>{0.f,0.f})
        {
            physicsBody = PhysicsBody(position, initial_vel);
            hasPhysics = true;
        }
        virtual void updatePhysics(float dt)
        {
            if(hasPhysics)
            {

                // then update this 
                Vec2 old_pos = position;
                physicsBody.update(dt);
                position = physicsBody.pos;
                Vector<2> displacement = position - old_pos;
                applyTranslation(displacement);

                // update children

            }


        }

        virtual void applyTranslation(const Vector<n>& disp) = 0;
        

        void setPosition(Vector<2> pos) {position = pos;}


        virtual void setColor(float r, float g, float b, float a = 1.0f)
        {
            color[0]=r;color[1]=g;color[2]=b;color[3]=a;
        }

        virtual void rotate(float theta, Vector<n> rotate_point = Vector<n>(), bool hasCentroid = false) = 0;
};

#endif /*SHAPE_H*/
