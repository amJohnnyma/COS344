#ifndef CONE_H
#define CONE_H

#include "Shape.h"
#include "Matrix.h"
#include "Vector.h"
#include "Triangle.h"
#include <cmath>
#include <iostream>

template <int n>
class Cone : public Shape<n>
{
    static_assert(n == 3, "Cone only works in 3-D (n=3).");

private:
    Vector<n> apex;        // tip of the cone
    Vector<n> baseCentre;  // centre of the circular base
    float radius;
    int segments;          // approximation quality (e.g. 32)

    static const int MAX_CHILDREN = 256;
    Shape<n>* children[MAX_CHILDREN] = {};
    int childCount = 0;

    void recalcPosition();
    Vector<n> rimPoint(int i) const;
    void rebuild();

public:
    Cone(const Vector<n>& apex_pt,
         const Vector<n>& base_centre,
         float r,
         int segs = 32);

    static Cone<n> upright(const Vector<n>& base_centre, float r, float height, int segs = 32);

    Cone(const Cone<n>& o);
    ~Cone() override;

    void add(Shape<n>* child);
    void remove(Shape<n>* child);

    Cone<n>& operator*=(const Matrix<n,n>& mat) override;

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
