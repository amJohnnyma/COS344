
#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "Matrix.h"
#include "Vector.h"
#include "Triangle.h"
#include <cmath>
#include <iostream>

template <int n>
class Sphere: public Shape<n>
{
    static_assert(n == 3, "Cone only works in 3-D (n=3).");

private:
    float radius;
    int segments;          // approximation quality (e.g. 32)
    Vector<n> center;

    static const int MAX_CHILDREN = 1024;
    Shape<n>* children[MAX_CHILDREN] = {};
    int childCount = 0;

    // currently a sphere constructed at any rotation looks the same so add
    // a matrix which rotates points
    Matrix<n,n> rotation = Matrix<n,n>::identity(); 

    void recalcPosition();
    void rebuild();

public:
    Sphere(const Vector<n>& center, float r,
            int segs = 32);


    Sphere(const Sphere<n>& o);
    ~Sphere() override;

    void add(Shape<n>* child);
    void remove(Shape<n>* child);

    Sphere<n>& operator*=(const Matrix<n,n>& mat) override;

    float* getPoints() const override;
    int getNumPoints() const override;
    void print() const override;
    void render(Renderer<n>& r) const override;
    void setColor(float r, float g, float b, float a = 1.0f) override;

    void rotate(Vector<n> angles, Vector<n> rotate_point = Vector<n>(), bool hasCentroid = false) override;

    /* Full 3D rotation using Matrix::makeRotation and rotatePoint */
    void rotate3D(const Vector<n>& angles,
                  Vector<n> pivot = Vector<n>(),
                  bool hasPivot = false) override;

    void applyTranslation(const Vector<n>& disp) override;
    void updatePhysics(float dt) override;
    void scale(float s) override;
    virtual void rotateAroundAxis(const Vector<n>& axis, float angle, Vector<n> pivot) override;

private:
    // Helper for rotation using the new Matrix API
    static Vector<n> rotatePoint(const Vector<n>& point, const Vector<n>& pivot, const Matrix<n,n>& R);
};

#endif 
