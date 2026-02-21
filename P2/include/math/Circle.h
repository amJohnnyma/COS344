#ifndef CIRCLE_H
#define CIRCLE_H
#include "Shape.h"
#include "Vector.h"
#include <vector>  

template <int n>
class Circle : public Shape<n> {
private:
    Vector<n> center;
    float radius;
    int segments;  // e.g., 32 for smooth circle
public:
    Circle(const Vector<n>& c, float r, int segs = 32);
    Circle(const Circle<n>&);
    virtual Circle<n>& operator*=(const Matrix<n,n>&);
    virtual Circle<n>* operator*(const Matrix<n,n>&) const;
    virtual float* getPoints() const override;  // Generates ~segments+1 points (triangle fan)
    virtual int getNumPoints() const override { return segments + 1; }
};
#endif
