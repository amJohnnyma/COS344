#ifndef TRIANGULAR_PRISM_H
#define TRIANGULAR_PRISM_H

/*
   Cross section is a triangle
   Stores front and back face
   Decomposed into 8 triangles. 2 triangle caps + 3 rectangular side faces (2 triangles each)
   */

#include "Shape.h"
#include "Matrix.h"
#include "Vector.h"
#include "Triangle.h"
#include <cmath>
#include <iostream>

template <int n>
class TriangularPrism : public Shape<n>
{
    static_assert(n == 3, "Triangular prism only works in 3D");
    private:
    Vector<n> f1,f2,f3; // front cap
    Vector<n> b1,b2,b3; // back cap

    static const int MAX_CHILDREN = 256;
    Shape<n>* children[MAX_CHILDREN] = {};
    int childCount = 0;


    void recalcPosition();

    void rebuild();

    public:
    // depth (how far toward -Z)
    TriangularPrism(
            const Vector<n>& p1, const Vector<n>& p2,
            const Vector<n>& p3, float depth
            );
    TriangularPrism(const Vector<n>& f1, const Vector<n>& f2, const Vector<n>& f3,
            const Vector<n>& b1, const Vector<n>& b2, const Vector<n>& b3);

    TriangularPrism(const TriangularPrism<n>& o);

    ~TriangularPrism() override;

    void add(Shape<n>* child);

    void remove(Shape<n>* child);
    virtual TriangularPrism<n>& operator*=(const Matrix<n,n>& mat) override;

    /* Returns 8 triangles × 3 vertices × n floats = 24n floats. */
    virtual float* getPoints() const override;

    virtual int getNumPoints() const override ;

    virtual void print() const override;
    virtual void render(Renderer<n>& r) const override;

    virtual void setColor(float r, float g, float b, float a = 1.0f) override;
    virtual void rotate(Vector<n> angles,
            Vector<n> rotate_point = Vector<n>(),
            bool hasCentroid = false) override;
    /*
     * rotate3D — rotate by Euler angles (radians, XYZ intrinsic).
     *   angles[0] = rotation around X axis
     *   angles[1] = rotation around Y axis
     *   angles[2] = rotation around Z axis
     */
    void rotate3D(const Vector<n>& angles,
            Vector<n> pivot = Vector<n>(),
            bool hasPivot = false) override;

    virtual void applyTranslation(const Vector<n>& disp) override;

    virtual void updatePhysics(float dt) override;

    virtual void scale(float s) override;
};

#endif
