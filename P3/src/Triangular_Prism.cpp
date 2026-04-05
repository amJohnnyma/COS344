#include "../include/math/Triangular_Prism.h"


// depth (how far toward -Z)
    template <int n>
TriangularPrism<n>::TriangularPrism(
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
    template <int n>
TriangularPrism<n>::TriangularPrism(const Vector<n>& f1, const Vector<n>& f2, const Vector<n>& f3,
        const Vector<n>& b1, const Vector<n>& b2, const Vector<n>& b3)
    : f1(f1), f2(f2), f3(f3), b1(b1), b2(b2), b3(b3)
{
    recalcPosition();
    rebuild();
}

    template <int n>
TriangularPrism<n>::TriangularPrism(const TriangularPrism<n>& o)
    : f1(o.f1), f2(o.f2), f3(o.f3), b1(o.b1), b2(o.b2), b3(o.b3)
{
    recalcPosition();
    rebuild();
}

    template <int n>
TriangularPrism<n>::~TriangularPrism()
{
    for (int i = 0; i < childCount; ++i) {
        delete children[i];
        children[i] = nullptr;
    }
    childCount = 0;
}

    template <int n>
void TriangularPrism<n>::add(Shape<n>* child)
{
    if (childCount >= MAX_CHILDREN) {
        std::cerr << "ERROR: Too many children in TriangularPrism!\n";
        delete child;
        return;
    }
    if (child == nullptr) return;
    children[childCount++] = child;
}

    template <int n>
void TriangularPrism<n>::remove(Shape<n>* child)
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
    template <int n>
TriangularPrism<n>& TriangularPrism<n>::operator*=(const Matrix<n,n>& mat)
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
template <int n>
float* TriangularPrism<n>::getPoints() const
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

template <int n>
int TriangularPrism<n>::getNumPoints() const { return 8 * 3; }

template <int n>
void TriangularPrism<n>::print() const
{
    std::cout << "TriangularPrism:\n";
    std::cout << "  Front f1: "; f1.print();
    std::cout << "  Front f2: "; f2.print();
    std::cout << "  Front f3: "; f3.print();
    std::cout << "  Back  b1: "; b1.print();
    std::cout << "  Back  b2: "; b2.print();
    std::cout << "  Back  b3: "; b3.print();
}
template <int n>
void TriangularPrism<n>::render(Renderer<n>& r) const
{
    for (int i = 0; i < childCount; ++i) children[i]->render(r);
}

    template <int n>
void TriangularPrism<n>::setColor(float r, float g, float b, float a)
{
    Shape<n>::setColor(r, g, b, a);
    for (int i = 0; i < childCount; ++i) children[i]->setColor(r, g, b, a);
}
/*
 * Single-angle rotate (Shape<n> interface) — maps to Z rotation only.
 * Use rotate3D for full 3-D control.
 */
    template <int n>
void TriangularPrism<n>::rotate(Vector<n> angles,
        Vector<n> rotate_point,
        bool hasCentroid)
{
    rotate3D(angles, rotate_point, hasCentroid);
}
/*
 * rotate3D — rotate by Euler angles (radians, XYZ intrinsic).
 *   angles[0] = rotation around X axis
 *   angles[1] = rotation around Y axis
 *   angles[2] = rotation around Z axis
 */
    template <int n>
void TriangularPrism<n>::rotate3D(const Vector<n>& angles,
        Vector<n> pivot,
        bool hasPivot)
{
    if (!hasPivot) pivot = this->position;

    Matrix<n,n> R = Matrix<n,n>::makeRotation(angles[0], angles[1], angles[2]);

    f1 = Matrix<n,n>::rotatePoint(f1, pivot, R);
    f2 = Matrix<n,n>::rotatePoint(f2, pivot, R);
    f3 = Matrix<n,n>::rotatePoint(f3, pivot, R);
    b1 = Matrix<n,n>::rotatePoint(b1, pivot, R);
    b2 = Matrix<n,n>::rotatePoint(b2, pivot, R);
    b3 = Matrix<n,n>::rotatePoint(b3, pivot, R);

    recalcPosition();
    rebuild();
}

template<int n>
void TriangularPrism<n>::rotateAroundAxis(const Vector<n>& axis, float angle, Vector<n> pivot)
{
    Matrix<n,n> R = Matrix<n,n>::makeRotationAroundAxis(axis, angle);
    f1 = Matrix<n,n>::rotatePoint(f1, pivot, R);
    f2 = Matrix<n,n>::rotatePoint(f2, pivot, R);
    f3 = Matrix<n,n>::rotatePoint(f3, pivot, R);
    b1 = Matrix<n,n>::rotatePoint(b1, pivot, R);
    b2 = Matrix<n,n>::rotatePoint(b2, pivot, R);
    b3 = Matrix<n,n>::rotatePoint(b3, pivot, R);


    recalcPosition();
    rebuild();
}
    template <int n>
void TriangularPrism<n>::applyTranslation(const Vector<n>& disp)
{
    f1 = f1 + disp; f2 = f2 + disp; f3 = f3 + disp;
    b1 = b1 + disp; b2 = b2 + disp; b3 = b3 + disp;

    if (!this->physicsBodyActive())
        this->position = this->position + disp;

    for (int i = 0; i < childCount; ++i) children[i]->applyTranslation(disp);
}

    template <int n>
void TriangularPrism<n>::updatePhysics(float dt)
{
    Shape<n>::updatePhysics(dt);
    for (int i = 0; i < childCount; ++i) children[i]->updatePhysics(dt);
}

    template <int n>
void TriangularPrism<n>::scale(float s)
{
    Vector<n> c = this->position;
    auto sc = [&](Vector<n>& v){ v = c + (v - c) * s; };
    sc(f1); sc(f2); sc(f3);
    sc(b1); sc(b2); sc(b3);
    rebuild();
    this->setColor(this->color[0], this->color[1],
            this->color[2], this->color[3]);
}

    template <int n>
void TriangularPrism<n>::recalcPosition()
{
    for (int k =0; k < n; ++k)
    {
        this->position[k] = (f1[k]+f2[k]+f3[k]+b1[k]+b2[k]+b3[k]) / 6.0f;
    }
}

    template <int n>
void TriangularPrism<n>::rebuild()
{
    for (int i = 0; i < childCount; ++i)
    { delete children[i];
        children[i] = nullptr; }
    childCount = 0;

    add(new Triangle<n>(f1,f2,f3));
    add(new Triangle<n>(b1,b3,b2));

    add(new Triangle<n>(f1,b2,f2));
    add(new Triangle<n>(f1,b1,b2));

    add(new Triangle<n>(f2,b3,f3));
    add(new Triangle<n>(f2,b2,b3));

    add(new Triangle<n>(f3,b1,f1));
    add(new Triangle<n>(f3,b3,b1));

    setColor(this->color[0], this->color[1], this->color[2], this->color[3]);
}


template class TriangularPrism<3>;

