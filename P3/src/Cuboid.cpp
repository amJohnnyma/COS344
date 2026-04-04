#include "../include/math/Cuboid.h"


    template <int n>
Cuboid<n>::Cuboid(const Vector<n>& centre, float hx, float hy, float hz)
{
    // front face (+Z side)
    tl  = Vector<n>{ centre[0]-hx, centre[1]+hy, centre[2]+hz };
    tr  = Vector<n>{ centre[0]+hx, centre[1]+hy, centre[2]+hz };
    bl  = Vector<n>{ centre[0]-hx, centre[1]-hy, centre[2]+hz };
    br  = Vector<n>{ centre[0]+hx, centre[1]-hy, centre[2]+hz };
    // back face (-Z side)
    tlb = Vector<n>{ centre[0]-hx, centre[1]+hy, centre[2]-hz };
    trb = Vector<n>{ centre[0]+hx, centre[1]+hy, centre[2]-hz };
    blb = Vector<n>{ centre[0]-hx, centre[1]-hy, centre[2]-hz };
    brb = Vector<n>{ centre[0]+hx, centre[1]-hy, centre[2]-hz };

    this->position = centre;
    rebuild();
}

    template <int n>
Cuboid<n>::Cuboid(const Vector<n>& tl,  const Vector<n>& tr,
        const Vector<n>& bl,  const Vector<n>& br,
        const Vector<n>& tlb, const Vector<n>& trb,
        const Vector<n>& blb, const Vector<n>& brb)
    : tl(tl), tr(tr), bl(bl), br(br),
    tlb(tlb), trb(trb), blb(blb), brb(brb)
{
    recalcPosition();
    rebuild();
}

    template <int n>
Cuboid<n>::Cuboid(const Cuboid<n>& o)
    : tl(o.tl), tr(o.tr), bl(o.bl), br(o.br),
    tlb(o.tlb), trb(o.trb), blb(o.blb), brb(o.brb)
{
    recalcPosition();
    rebuild();
}
    template <int n>
Cuboid<n>::~Cuboid()
{
    for (int i = 0; i < childCount; ++i) delete children[i];
}
    template <int n>
void Cuboid<n>::add(Shape<n>* child)
{
    if (childCount >= MAX_CHILDREN) {
        std::cerr << "ERROR: Too many children in Cuboid!\n";
        delete child;
        return;
    }
    if (child == nullptr) return;
    children[childCount++] = child;
}

    template <int n>
void Cuboid<n>::remove(Shape<n>* child)
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
Cuboid<n>& Cuboid<n>::operator*=(const Matrix<n,n>& mat)
{
    // Apply matrix to every corner (linear transform around the origin).
    auto applyMat = [&](Vector<n>& v) {
        Vector<n> old = v;
        for (int i = 0; i < n; ++i) {
            float s = 0.0f;
            for (int j = 0; j < n; ++j) s += mat[i][j] * old[j];
            v[i] = s;
        }
    };
    applyMat(tl); applyMat(tr); applyMat(bl); applyMat(br);
    applyMat(tlb); applyMat(trb); applyMat(blb); applyMat(brb);
    recalcPosition();
    rebuild();
    return *this;
}

/*  Returns 12 triangles × 3 vertices × n floats = 36n floats.
 *  Caller must delete[]. */
template <int n>
float* Cuboid<n>::getPoints() const
{
    const int totalFloats = 12 * 3 * n;
    float* pts = new float[totalFloats];
    int idx = 0;

    // Reuse the same triangle layout as rebuild().
    auto pushTri = [&](const Vector<n>& a, const Vector<n>& b, const Vector<n>& c) {
        for (int k = 0; k < n; ++k) pts[idx++] = a[k];
        for (int k = 0; k < n; ++k) pts[idx++] = b[k];
        for (int k = 0; k < n; ++k) pts[idx++] = c[k];
    };

    pushTri(tl,  bl,  br);  pushTri(tl,  br,  tr);   // front
    pushTri(trb, brb, blb); pushTri(trb, blb, tlb);  // back
    pushTri(tlb, blb, bl);  pushTri(tlb, bl,  tl);   // left
    pushTri(tr,  br,  brb); pushTri(tr,  brb, trb);  // right
    pushTri(tlb, tl,  tr);  pushTri(tlb, tr,  trb);  // top
    pushTri(bl,  blb, brb); pushTri(bl,  brb, br);   // bottom

    return pts;
}

template <int n>
int Cuboid<n>::getNumPoints() const { return 12 * 3; }  // 36 vertices

template <int n>
void Cuboid<n>::print() const
{
    std::cout << "Cuboid:\n";
    std::cout << "  Front TL: "; tl.print();
    std::cout << "  Front TR: "; tr.print();
    std::cout << "  Front BL: "; bl.print();
    std::cout << "  Front BR: "; br.print();
    std::cout << "  Back  TL: "; tlb.print();
    std::cout << "  Back  TR: "; trb.print();
    std::cout << "  Back  BL: "; blb.print();
    std::cout << "  Back  BR: "; brb.print();
}

template <int n>
void Cuboid<n>::render(Renderer<n>& r) const
{
    for (int i = 0; i < childCount; ++i) children[i]->render(r);
}

    template <int n>
void Cuboid<n>::setColor(float r, float g, float b, float a)
{
    Shape<n>::setColor(r, g, b, a);
    for (int i = 0; i < childCount; ++i) children[i]->setColor(r, g, b, a);
}

/*
 * rotate — full 3-D rotation by Euler angles (radians, XYZ intrinsic).
 *
 * Parameters:
 *   angles       — Vector<n> where [0]=angX, [1]=angY, [2]=angZ
 *   rotate_point — pivot (default: shape centroid)
 *   hasCentroid  — if true use rotate_point as-is, else use centroid
 *
 * Legacy single-angle signature is kept for Shape<n> compatibility;
 * it maps to a Z-rotation only (matches 2-D behaviour).
 */
    template <int n>
void Cuboid<n>::rotate(Vector<n> angles,
        Vector<n> rotate_point,
        bool hasCentroid)
{
    // Single-float override → treat as Z-rotation only.
    rotate3D(angles, rotate_point, hasCentroid);
}

    template <int n>
void Cuboid<n>::rotate3D(const Vector<n>& angles,
        Vector<n> pivot,
        bool hasPivot)
{
    if (!hasPivot) {
        // Use centroid as pivot.
        pivot = this->position;
    }

    Matrix<n,n> R = Matrix<n,n>::makeRotation(angles[0], angles[1], angles[2]);

    tl  = Matrix<n,n>::rotatePoint(tl,  pivot, R);
    tr  = Matrix<n,n>::rotatePoint(tr,  pivot, R);
    bl  = Matrix<n,n>::rotatePoint(bl,  pivot, R);
    br  = Matrix<n,n>::rotatePoint(br,  pivot, R);
    tlb = Matrix<n,n>::rotatePoint(tlb, pivot, R);
    trb = Matrix<n,n>::rotatePoint(trb, pivot, R);
    blb = Matrix<n,n>::rotatePoint(blb, pivot, R);
    brb = Matrix<n,n>::rotatePoint(brb, pivot, R);

    recalcPosition();
    rebuild();
}

    template <int n>
void Cuboid<n>::applyTranslation(const Vector<n>& disp)
{
    tl  = tl  + disp; tr  = tr  + disp;
    bl  = bl  + disp; br  = br  + disp;
    tlb = tlb + disp; trb = trb + disp;
    blb = blb + disp; brb = brb + disp;

    if (!this->physicsBodyActive())
        this->position = this->position + disp;

    for (int i = 0; i < childCount; ++i) children[i]->applyTranslation(disp);
}

    template <int n>
void Cuboid<n>::updatePhysics(float dt)
{
    Shape<n>::updatePhysics(dt);
    for (int i = 0; i < childCount; ++i) children[i]->updatePhysics(dt);
}

    template <int n>
void Cuboid<n>::scale(float s)
{
    Vector<n> c = this->position;
    auto sc = [&](Vector<n>& v){ v = c + (v - c) * s; };
    sc(tl); sc(tr); sc(bl); sc(br);
    sc(tlb); sc(trb); sc(blb); sc(brb);
    rebuild();
    this->setColor(this->color[0], this->color[1],
            this->color[2], this->color[3]);
}

    /* Recompute the centroid-position from the 8 corners. */
    template <int n>
    void Cuboid<n>::recalcPosition()
    {
        for (int k = 0; k < n; ++k)
            this->position[k] = (tl[k]+tr[k]+bl[k]+br[k]+
                                  tlb[k]+trb[k]+blb[k]+brb[k]) * 0.125f;
    }

    /* Destroy old triangle children and rebuild from current corners. */
template <int n>
    void Cuboid<n>::rebuild()
    {
        for (int i = 0; i < childCount; ++i) delete children[i];
        childCount = 0;

        /*  6 faces × 2 triangles = 12 triangles
         *
         *  Winding: counter-clockwise when viewed from outside (right-hand).
         *
         *  Front  (+Z): tl, bl, br  /  tl, br, tr
         *  Back   (-Z): trb, brb, blb / trb, blb, tlb
         *  Left   (-X): tlb, blb, bl  /  tlb, bl, tl
         *  Right  (+X): tr, br, brb   /  tr, brb, trb
         *  Top    (+Y): tlb, tl, tr   /  tlb, tr, trb
         *  Bottom (-Y): bl, blb, brb  /  bl, brb, br
         */
        add(new Triangle<n>(tl,  bl,  br));   // front
        add(new Triangle<n>(tl,  br,  tr));
        add(new Triangle<n>(trb, brb, blb));  // back
        add(new Triangle<n>(trb, blb, tlb));
        add(new Triangle<n>(tlb, blb, bl));   // left
        add(new Triangle<n>(tlb, bl,  tl));
        add(new Triangle<n>(tr,  br,  brb));  // right
        add(new Triangle<n>(tr,  brb, trb));
        add(new Triangle<n>(tlb, tl,  tr));   // top
        add(new Triangle<n>(tlb, tr,  trb));
        add(new Triangle<n>(bl,  blb, brb));  // bottom
        add(new Triangle<n>(bl,  brb, br));

        setColor(this->color[0], this->color[1], this->color[2]);
    }


template class Cuboid<3>;
