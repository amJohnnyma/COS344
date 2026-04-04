#ifndef CYLINDER_H
#define CYLINDER_H

/*
 * Cylinder.h — n=3
 *
 * A cylinder approximated with `segments` rectangular side faces (each split into 2 triangles)
 * plus two circular disc caps.
 */

#include "Shape.h"
#include "Matrix.h"
#include "Vector.h"
#include "Triangle.h"
#include <cmath>
#include <iostream>

template <int n>
class Cylinder : public Shape<n>
{
    static_assert(n == 3, "Cylinder only works in 3-D (n=3).");

private:
    Vector<n> topCentre;
    Vector<n> botCentre;
    float radius;
    int segments;

    static const int MAX_CHILDREN = 512;
    Shape<n>* children[MAX_CHILDREN] = {};
    int childCount = 0;

    void recalcPosition();
    Vector<n> rimPoint(const Vector<n>& centre, int i) const;
    void rebuild();

public:
    Cylinder(const Vector<n>& top_centre,
             const Vector<n>& bot_centre,
             float r,
             int segs = 32);

    static Cylinder<n> upright(const Vector<n>& base_centre,
                               float r, float height, int segs = 32);

    Cylinder(const Cylinder<n>& o);
    ~Cylinder() override;

    void add(Shape<n>* child);
    void remove(Shape<n>* child);

    virtual Cylinder<n>& operator*=(const Matrix<n,n>& mat) override;

    virtual float* getPoints() const override;
    virtual int getNumPoints() const override;
    virtual void print() const override;
    virtual void render(Renderer<n>& r) const override;
    virtual void setColor(float r, float g, float b, float a = 1.0f) override;

    virtual void rotate(Vector<n> angles,
                        Vector<n> rotate_point = Vector<n>(),
                        bool hasCentroid = false) override;

    /* Full 3D Euler rotation using Matrix::makeRotation */
    void rotate3D(const Vector<n>& angles,
                  Vector<n> pivot = Vector<n>(),
                  bool hasPivot = false) override;

    virtual void applyTranslation(const Vector<n>& disp) override;
    virtual void updatePhysics(float dt) override;
    virtual void scale(float s) override;

};

#endif /* CYLINDER_H */
