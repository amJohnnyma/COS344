#ifndef CUBOID_H
#define CUBOID_H

/*
 * Cuboid.h  —  axis-aligned rectangular prism, n=3
 *
 * Decomposed into 12 triangles (2 per face × 6 faces).
 * Rotation is performed by first-principles 3-D rotation matrices:
 *
 *   R = Rz(angZ) * Ry(angY) * Rx(angX)          (intrinsic XYZ Euler)
 *
 * A translate-rotate-translate scheme moves the pivot to the origin,
 * applies R, then restores it — implemented entirely with your
 * existing Matrix<3,3> and Vector<3> classes.
 *
 * Allowed includes: cmath, iostream, stdio.h, stdlib.h, iomanip, sstream,
 *   GL/glew.h, GLFW/glfw3.h, glm/glm.hpp  (plus your own headers).
 */

#include "Shape.h"
#include "Matrix.h"
#include "Vector.h"
#include "Triangle.h"
#include <cmath>
#include <iostream>

template <int n>
class Cuboid : public Shape<n>
{
    static_assert(n == 3, "Cuboid only works in 3-D (n=3).");

private:
    /*
     * Store all 8 corners.  Naming convention (right-hand, Y-up):
     *
     *   tl = top-left-front     tr = top-right-front
     *   bl = bot-left-front     br = bot-right-front
     *   tlb= top-left-back      trb= top-right-back
     *   blb= bot-left-back      brb= bot-right-back
     */
    Vector<n> tl, tr, bl, br;      // front face  (+Z)
    Vector<n> tlb, trb, blb, brb;  // back  face  (-Z)

    static const int MAX_CHILDREN = 100000;
    Shape<n>* children[MAX_CHILDREN] = {};
    int childCount = 0;
    int m_splitLevel = 0;



    /* Recompute the centroid-position from the 8 corners. */
    void recalcPosition();

    /* Destroy old triangle children and rebuild from current corners. */
    void rebuild();

public:

    /*  Build from centre + half-extents (hx, hy, hz). */
    Cuboid(const Vector<n>& centre, float hx, float hy, float hz);

    /*  Build directly from 8 corners (order: tl,tr,bl,br, tlb,trb,blb,brb). */
    Cuboid(const Vector<n>& tl,  const Vector<n>& tr,
           const Vector<n>& bl,  const Vector<n>& br,
           const Vector<n>& tlb, const Vector<n>& trb,
           const Vector<n>& blb, const Vector<n>& brb);

    Cuboid(const Cuboid<n>& o);

    ~Cuboid() override;


    void add(Shape<n>* child);

    void remove(Shape<n>* child);


    virtual Cuboid<n>& operator*=(const Matrix<n,n>& mat) override;

    /*  Returns 12 triangles × 3 vertices × n floats = 36n floats.
     *  Caller must delete[]. */
    virtual float* getPoints() const override;

    virtual int getNumPoints() const override; // 36 vertices

    virtual void print() const override;

    virtual void render(Renderer<n>& r) const override;

    virtual void setColor(float r, float g, float b, float a = 1.0f) override;

    virtual void rotate(Vector<n> angles,
                        Vector<n> rotate_point = Vector<n>(),
                        bool hasCentroid = false) override;

    void rotate3D(const Vector<n>& angles,
                  Vector<n> pivot = Vector<n>(),
                  bool hasPivot = false) override;

    virtual void applyTranslation(const Vector<n>& disp) override;

    virtual void updatePhysics(float dt) override;

    virtual void scale(float s) override;
    virtual void rotateAroundAxis(const Vector<n>& axis, float angle, Vector<n> pivot) override;

    void splitVertices(int levels);

    void applySplits(int levels);
};

#endif /* CUBOID_H */
