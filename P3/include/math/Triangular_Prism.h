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

    static const int MAX_CHILDREN = 64;
    Shape<n>* children[MAX_CHILDREN] = {};
    int childCount;

    // Rotation Helpers
    static Matrix<n,n> makeRotation(float aX, float aY, float aZ)
    {
        const float cx = std::cos(aX), sx = std::sin(aX);
        const float cy = std::cos(aY), sy = std::sin(aY);
        const float cz = std::cos(aZ), sz = std::sin(aZ);

        Matrix<n,n> Rx;
        Rx[0][0]=1; Rx[0][1]=0; Rx[0][2]=0;
        Rx[1][0]=0; Rx[1][1]=cx; Rx[1][2]=-sx;
        Rx[2][0]=0; Rx[2][1]=sx; Rx[2][2]=cx;


        Matrix<n,n> Ry;
        Ry[0][0]=cy; Ry[0][1]=0; Ry[0][2]=sy;
        Ry[1][0]=0; Ry[1][1]=1; Ry[1][2]=0;
        Ry[2][0]=-sy; Ry[2][1]=0; Ry[2][2]=cy;


        Matrix<n,n> Rz;
        Rz[0][0]=cz; Rz[0][1]=-sz; Rz[0][2]=0;
        Rz[1][0]=sz; Rz[1][1]=cz; Rz[1][2]=0;
        Rz[2][0]=0; Rz[2][1]=0; Rz[2][2]=1;

        return Rz * Ry * Rx;
    }


    static Vector<n> rotatePoint(
            const Vector<n>& p,
            const Vector<n>& pivot,
            const Matrix<n,n>& R)
    {
        Vector<n> local = p - pivot;
        Vector<n> rotated;
        for(int i =0; i < n; ++i)
        {
            float sum = 0.0f;
            for(int j =0; j < n; ++j) sum += R[i][j] * local[j];
            rotated[i] = sum;

        }
        return rotated + pivot;
    }

    void recalcPosition()
    {
        for (int k =0; k < n; ++k)
        {
            this->position[k] = (f1[k]+f2[k]+f3[k]+b1[k]+b2[k]+b3[k]) / 6.0f;
        }
    }

    void rebuild()
    {
        for (int i = 0; i < childCount; ++i) delete children[i];
        childCount = 0;

        add(new Triangle<n>(f1,f2,f3));
        add(new Triangle<n>(b1,b3,b2));

        add(new Triangle<n>(f1,b2,f2));
        add(new Triangle<n>(f1,b1,b2));

        add(new Triangle<n>(f2,b3,f3));
        add(new Triangle<n>(f2,b2,b3));

        add(new Triangle<n>(f3,b1,f1));
        add(new Triangle<n>(f3,b3,b1));
    }

    public:
    // depth (how far toward -Z)
    TriangularPrism(
            const Vector<n>& p1, const Vector<n>& p2,
            const Vector<n>& p3, float depth
            ) : f1(p1), f2(p2), f3(p3)
    {
        Vector<n> extrude;
        extrude[2] = -depth;

        b1 = f1 + extrude;
        b2 = f2 + extrude;
        b3 = f3 + extrude;

        recalcPosition();
        rebuild();
    }
    TriangularPrism(const Vector<n>& f1, const Vector<n>& f2, const Vector<n>& f3,
                    const Vector<n>& b1, const Vector<n>& b2, const Vector<n>& b3)
        : f1(f1), f2(f2), f3(f3), b1(b1), b2(b2), b3(b3)
    {
        recalcPosition();
        rebuild();
    }

    TriangularPrism(const TriangularPrism<n>& o)
        : f1(o.f1), f2(o.f2), f3(o.f3), b1(o.b1), b2(o.b2), b3(o.b3)
    {
        recalcPosition();
        rebuild();
    }

    ~TriangularPrism() override
    {
        for (int i = 0; i < childCount; ++i) delete children[i];
    }

    void add(Shape<n>* child)
    {
        if (childCount < MAX_CHILDREN) children[childCount++] = child;
    }

    void remove(Shape<n>* child)
    {
        for (int i =0; i < childCount; ++i)
        {
            if(children[i] == child)
            {
                for (int j =i; j < childCount-1; ++j) children[j] = children[j+1];
                --childCount;
                return;
            }
        }
    }
    virtual TriangularPrism<n>& operator*=(const Matrix<n,n>& mat) override
    {
        auto applyMat = [&](Vector<n>& v) {
            Vector<n> old = v;
            for (int i = 0; i < n; ++i) {
                float s = 0.0f;
                for (int j = 0; j < n; ++j) s += mat[i][j] * old[j];
                v[i] = s;
            }
        };
        applyMat(f1); applyMat(f2); applyMat(f3);
        applyMat(b1); applyMat(b2); applyMat(b3);
        recalcPosition();
        rebuild();
        return *this;
    }

    /* Returns 8 triangles × 3 vertices × n floats = 24n floats. */
    virtual float* getPoints() const override
    {
        const int totalFloats = 8 * 3 * n;
        float* pts = new float[totalFloats];
        int idx = 0;

        auto pushTri = [&](const Vector<n>& a, const Vector<n>& b, const Vector<n>& c) {
            for (int k = 0; k < n; ++k) pts[idx++] = a[k];
            for (int k = 0; k < n; ++k) pts[idx++] = b[k];
            for (int k = 0; k < n; ++k) pts[idx++] = c[k];
        };

        pushTri(f1, f2, f3);
        pushTri(b1, b3, b2);
        pushTri(f1, f2, b2); pushTri(f1, b2, b1);
        pushTri(f2, f3, b3); pushTri(f2, b3, b2);
        pushTri(f3, f1, b1); pushTri(f3, b1, b3);

        return pts;
    }

    virtual int getNumPoints() const override { return 8 * 3; }

    virtual void print() const override
    {
        std::cout << "TriangularPrism:\n";
        std::cout << "  Front f1: "; f1.print();
        std::cout << "  Front f2: "; f2.print();
        std::cout << "  Front f3: "; f3.print();
        std::cout << "  Back  b1: "; b1.print();
        std::cout << "  Back  b2: "; b2.print();
        std::cout << "  Back  b3: "; b3.print();
    }
 virtual void render(Renderer<n>& r) const override
    {
        for (int i = 0; i < childCount; ++i) children[i]->render(r);
    }

    virtual void setColor(float r, float g, float b, float a = 1.0f) override
    {
        Shape<n>::setColor(r, g, b, a);
        for (int i = 0; i < childCount; ++i) children[i]->setColor(r, g, b, a);
    }
    /*
     * Single-angle rotate (Shape<n> interface) — maps to Z rotation only.
     * Use rotate3D for full 3-D control.
     */
    virtual void rotate(Vector<n> angles,
                        Vector<n> rotate_point = Vector<n>(),
                        bool hasCentroid = false) override
    {
        rotate3D(angles, rotate_point, hasCentroid);
    }
 /*
     * rotate3D — rotate by Euler angles (radians, XYZ intrinsic).
     *   angles[0] = rotation around X axis
     *   angles[1] = rotation around Y axis
     *   angles[2] = rotation around Z axis
     */
    void rotate3D(const Vector<n>& angles,
                  Vector<n> pivot = Vector<n>(),
                  bool hasPivot = false)
    {
        if (!hasPivot) pivot = this->position;

        Matrix<n,n> R = makeRotation(angles[0], angles[1], angles[2]);

        f1 = rotatePoint(f1, pivot, R);
        f2 = rotatePoint(f2, pivot, R);
        f3 = rotatePoint(f3, pivot, R);
        b1 = rotatePoint(b1, pivot, R);
        b2 = rotatePoint(b2, pivot, R);
        b3 = rotatePoint(b3, pivot, R);

        recalcPosition();
        rebuild();
    }
    virtual void applyTranslation(const Vector<n>& disp) override
    {
        f1 = f1 + disp; f2 = f2 + disp; f3 = f3 + disp;
        b1 = b1 + disp; b2 = b2 + disp; b3 = b3 + disp;

        if (!this->physicsBodyActive())
            this->position = this->position + disp;

        for (int i = 0; i < childCount; ++i) children[i]->applyTranslation(disp);
    }

    virtual void updatePhysics(float dt) override
    {
        Shape<n>::updatePhysics(dt);
        for (int i = 0; i < childCount; ++i) children[i]->updatePhysics(dt);
    }

    virtual void scale(float s) override
    {
        Vector<n> c = this->position;
        auto sc = [&](Vector<n>& v){ v = c + (v - c) * s; };
        sc(f1); sc(f2); sc(f3);
        sc(b1); sc(b2); sc(b3);
        rebuild();
        this->setColor(this->color[0], this->color[1],
                       this->color[2], this->color[3]);
    }
};

#endif
