#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"
#include "Vector.h"
#include <cmath>

template <int n>
class Circle : public Shape<n> {
private:
    Vector<n> center;
    float radius;
    int segments;   // e.g. 32 for a smooth circle
public:
    Circle(const Vector<n>& c, float r, int segs = 32);
    Circle(const Circle<n>&);
    virtual Circle<n>& operator*=(const Matrix<n,n>&);
    virtual Circle<n>* operator*(const Matrix<n,n>&) const;

    // Returns (segments * 3 * n) floats — triangle-fan decomposed into triangles.
    // Each triangle: center, p_i, p_{i+1}.
    virtual float* getPoints() const;
    virtual int getNumPoints() const { return segments * 3; }

    virtual void print() const{
        std::cout << "Circle center:" << std::endl;
        center.print();
        std::cout << "radius=" << radius
                  << " segments=" << segments << std::endl;
    }
};

#endif /*CIRCLE_H*/
