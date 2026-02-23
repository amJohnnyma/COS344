#ifndef SHAPE_H
#define SHAPE_H

#include "Matrix.h"
#include "Vector.h"
#include <iostream>

template <int n> class Renderer;


//component
template <int n>
class Shape {
    protected:
        Vector<4> color = Vector<4>{1.0f,1.0f,1.0f,1.0f};
    public:
        virtual ~Shape() = default;
        virtual Shape& operator*=(const Matrix<n,n>&) = 0;
        virtual float* getPoints() const = 0;   // caller must delete[]
        virtual int getNumPoints() const = 0;
        virtual void print() const = 0;
        virtual void render(Renderer<n>& r) const = 0;

        virtual void setColor(float r, float g, float b, float a = 1.0f)
        {
            color[0]=r;color[1]=g;color[2]=b;color[3]=a;
        }
};

#endif /*SHAPE_H*/
