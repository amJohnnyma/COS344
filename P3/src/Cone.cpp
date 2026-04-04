#include "../include/math/Cone.h"


template <int n>
void Cone<n>::recalcPosition()
{
    // centroid = midpoint of apex and base centre
    for (int k = 0; k < n; ++k)
        this->position[k] = (apex[k] + baseCentre[k]) * 0.5f;
}

template <int n>
Vector<n> Cone<n>::rimPoint(int i) const
{
    const float twoPi = 2.0f * 3.14159265358979323846f;
    float t = twoPi * i / segments;

    // Cone axis (unit)
    Vector<n> axis = apex - baseCentre;
    float mag = axis.magnitude();
    if (mag < 1e-12f) {
        return baseCentre;
    }

    Vector<n> axisN = axis * (1.0f / mag);

    // Choose a world-up reference that is not collinear with axisN
    Vector<n> worldUp{};
    if (std::abs(axisN[1]) < 0.99f) {
        worldUp[1] = 1.0f;
    } else {
        worldUp[0] = 1.0f;
    }

    // right = axisN × worldUp
    Vector<n> right = axisN.crossProduct(worldUp);
    float rightMag = right.magnitude();
    if (rightMag > 1e-12f)
        right = right * (1.0f / rightMag);

    // up = axisN × right
    Vector<n> up = axisN.crossProduct(right);

    // rim point
    Vector<n> rim = baseCentre 
                  + right * (radius * std::cos(t))
                  + up   * (radius * std::sin(t));

    return rim;
}

template <int n>
void Cone<n>::rebuild()
{
    for (int i = 0; i < childCount; ++i)
        delete children[i];

    childCount = 0;

    for (int i = 0; i < segments; ++i) {
        Vector<n> p0 = rimPoint(i);
        Vector<n> p1 = rimPoint((i + 1) % segments);   // wrap around

        // Lateral face: apex → p0 → p1
        add(new Triangle<n>(apex, p0, p1));

        // Base disc: baseCentre → p1 → p0 (reversed winding)
        add(new Triangle<n>(baseCentre, p1, p0));
    }

    setColor(this->color[0], this->color[1], this->color[2]);
}


template <int n>
Cone<n>::Cone(const Vector<n>& apex_pt,
              const Vector<n>& base_centre,
              float r,
              int segs)
    : apex(apex_pt), baseCentre(base_centre), radius(r), segments(segs)
{
    recalcPosition();
    rebuild();
}

template <int n>
Cone<n> Cone<n>::upright(const Vector<n>& base_centre, float r, float height, int segs)
{
    Vector<n> ap = base_centre;
    ap[1] += height;
    return Cone<n>(ap, base_centre, r, segs);
}

template <int n>
Cone<n>::Cone(const Cone<n>& o)
    : apex(o.apex), baseCentre(o.baseCentre), radius(o.radius), segments(o.segments)
{
    recalcPosition();
    rebuild();
}

template <int n>
Cone<n>::~Cone()
{
    for (int i = 0; i < childCount; ++i)
        delete children[i];
}



    template <int n>
void Cone<n>::add(Shape<n>* child)
{
    if (childCount >= MAX_CHILDREN) {
        std::cerr << "ERROR: Too many children in Cone!\n";
        delete child;
        return;
    }
    if (child == nullptr) return;
    children[childCount++] = child;
}

    template <int n>
void Cone<n>::remove(Shape<n>* child)
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
Cone<n>& Cone<n>::operator*=(const Matrix<n,n>& mat)
{
    auto applyMat = [&](Vector<n>& v) {
        Vector<n> old = v;
        for (int i = 0; i < n; ++i) {
            float s = 0.0f;
            for (int j = 0; j < n; ++j)
                s += mat[i][j] * old[j];
            v[i] = s;
        }
    };

    applyMat(apex);
    applyMat(baseCentre);

    recalcPosition();
    rebuild();
    return *this;
}

template <int n>
float* Cone<n>::getPoints() const
{
    const int totalFloats = 2 * segments * 3 * n;
    float* pts = new float[totalFloats];
    int idx = 0;

    for (int i = 0; i < segments; ++i) {
        Vector<n> p0 = rimPoint(i);
        Vector<n> p1 = rimPoint((i + 1) % segments);

        // lateral triangle
        for (int k = 0; k < n; ++k) pts[idx++] = apex[k];
        for (int k = 0; k < n; ++k) pts[idx++] = p0[k];
        for (int k = 0; k < n; ++k) pts[idx++] = p1[k];

        // base triangle
        for (int k = 0; k < n; ++k) pts[idx++] = baseCentre[k];
        for (int k = 0; k < n; ++k) pts[idx++] = p1[k];
        for (int k = 0; k < n; ++k) pts[idx++] = p0[k];
    }
    return pts;
}

template <int n>
int Cone<n>::getNumPoints() const
{
    return 2 * segments * 3;
}

template <int n>
void Cone<n>::print() const
{
    std::cout << "Cone:\n";
    std::cout << " Apex: "; apex.print();
    std::cout << " BaseCentre: "; baseCentre.print();
    std::cout << " radius=" << radius << " segments=" << segments << "\n";
}

template <int n>
void Cone<n>::render(Renderer<n>& r) const
{
    for (int i = 0; i < childCount; ++i)
        children[i]->render(r);
}

template <int n>
void Cone<n>::setColor(float r, float g, float b, float a)
{
    Shape<n>::setColor(r, g, b, a);
    for (int i = 0; i < childCount; ++i)
        children[i]->setColor(r, g, b, a);
}

template <int n>
void Cone<n>::rotate(Vector<n> angles, Vector<n> rotate_point, bool hasCentroid)
{
    rotate3D(angles, rotate_point, hasCentroid);
}


template <int n>
Vector<n> Cone<n>::rotatePoint(const Vector<n>& point, const Vector<n>& pivot, const Matrix<n,n>& R)
{
    Vector<n> translated = point - pivot;
    Vector<n> rotated = R * translated;           // Uses Matrix::operator*(Vector)
    return rotated + pivot;
}

template <int n>
void Cone<n>::rotate3D(const Vector<n>& angles,
                       Vector<n> pivot,
                       bool hasPivot)
{
    if (!hasPivot)
        pivot = this->position;

    // Use Matrix<n,n>::makeRotation as requested
    Matrix<n,n> R = Matrix<n,n>::makeRotation(angles[0], angles[1], angles[2]);

    apex      = rotatePoint(apex,      pivot, R);
    baseCentre = rotatePoint(baseCentre, pivot, R);

    recalcPosition();
    rebuild();
}

template <int n>
void Cone<n>::applyTranslation(const Vector<n>& disp)
{
    apex = apex + disp;
    baseCentre = baseCentre + disp;

    if (!this->physicsBodyActive())
        this->position = this->position + disp;

    for (int i = 0; i < childCount; ++i)
        children[i]->applyTranslation(disp);
}

template <int n>
void Cone<n>::updatePhysics(float dt)
{
    Shape<n>::updatePhysics(dt);
    for (int i = 0; i < childCount; ++i)
        children[i]->updatePhysics(dt);
}

template <int n>
void Cone<n>::scale(float s)
{
    Vector<n> c = this->position;
    apex       = c + (apex - c) * s;
    baseCentre = c + (baseCentre - c) * s;
    radius    *= s;

    rebuild();
    this->setColor(this->color[0], this->color[1], this->color[2], this->color[3]);
}

template class Cone<3>;
