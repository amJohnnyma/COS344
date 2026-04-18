
#include "../include/math/Sphere.h"


    template <int n>
void Sphere<n>::recalcPosition()
{
    this->position = center;
}


    template <int n>
void Sphere<n>::rebuild()
{
    for (int i = 0; i < childCount; ++i)
        delete children[i];

    childCount = 0;

    const int stacks = segments / 2;
    const float pi = 3.14159265358;

    auto rotated = [&](Vector<n> v) {
        // translate to origin, rotate, translate back
        Vector<n> local = v - center;
        Vector<n> r;
        for (int i = 0; i < n; ++i) {
            r[i] = 0.0f;
            for (int j = 0; j < n; ++j)
                r[i] += rotation[i][j] * local[j];
        }
        return r + center;
    };

    Vector<n> top = center;
    top[1] = center[1] + radius; // y is up
    top = rotated(top);


    float phi1 = pi * (1.f / stacks);
    float y1 = center[1] + radius * std::cos(phi1);
    float r1 = radius * std::sin(phi1);

    // ring below pole
    for (int j = 0; j < segments; j ++)
    {
        float theta0 = (2.f * pi * j) / segments;
        float theta1 = (2.f * pi * (j + 1)) / segments;

        Vector<n> a = center, b = center;
        a[0] = center[0] + r1 * std::cos(theta0);
        a[1] = y1;
        a[2] = center[2] + r1 * std::sin(theta0);

        b[0] = center[0] + r1 * std::cos(theta1);
        b[1] = y1;
        b[2] = center[2] + r1 * std::sin(theta1);

        add(new Triangle<n>(top, rotated(b), rotated(a)));

    }
    // mid quad bands
    for (int i = 1; i < stacks - 1; ++i)
    {
        float phi_i0 = pi * (float(i)     / stacks);
        float phi_i1 = pi * (float(i + 1) / stacks);

        float ya = center[1] + radius * std::cos(phi_i0);
        float ra = radius * std::sin(phi_i0);
        float yb = center[1] + radius * std::cos(phi_i1);
        float rb = radius * std::sin(phi_i1);

        for (int j = 0; j < segments; ++j)
        {
            float theta0 = (2.0f * pi * j)       / segments;
            float theta1 = (2.0f * pi * (j + 1)) / segments;

            Vector<n> p00 = center, p10 = center,
                p01 = center, p11 = center;

            p00[0] = center[0] + ra * std::cos(theta0); p00[1] = ya; p00[2] = center[2] + ra * std::sin(theta0);
            p10[0] = center[0] + ra * std::cos(theta1); p10[1] = ya; p10[2] = center[2] + ra * std::sin(theta1);
            p01[0] = center[0] + rb * std::cos(theta0); p01[1] = yb; p01[2] = center[2] + rb * std::sin(theta0);
            p11[0] = center[0] + rb * std::cos(theta1); p11[1] = yb; p11[2] = center[2] + rb * std::sin(theta1);

            // Quad split into 2 triangles
            add(new Triangle<n>(rotated(p00), rotated(p10), rotated(p11)));
            add(new Triangle<n>(rotated(p00),rotated( p11),rotated( p01)));
        }
    }
    // south pole
    Vector<n> bot = center;
    bot[1] = center[1] - radius;
    bot = rotated(bot);

    float phi_last = pi * (float(stacks - 1) / stacks);
    float y_last   = center[1] + radius * std::cos(phi_last);
    float r_last   = radius * std::sin(phi_last);

    for (int j = 0; j < segments; ++j)
    {
        float theta0 = (2.0f * pi * j)       / segments;
        float theta1 = (2.0f * pi * (j + 1)) / segments;

        Vector<n> a = center, b = center;
        a[0] = center[0] + r_last * std::cos(theta0);
        a[1] = y_last;
        a[2] = center[2] + r_last * std::sin(theta0);

        b[0] = center[0] + r_last * std::cos(theta1);
        b[1] = y_last;
        b[2] = center[2] + r_last * std::sin(theta1);

        add(new Triangle<n>(bot, rotated(a),rotated( b)));   // reversed winding for south
    }



    setColor(this->color[0], this->color[1], this->color[2], this->color[3]);

    for (int i = 0; i < childCount; ++i)
    {
        children[i]->setShapeType(this->shapeType);
    }
}


    template <int n>
Sphere<n>::Sphere(const Vector<n>& center, float r,
        int segs)
    : center(center), radius(r), segments(segs)
{
    this->shapeType = 9;
    recalcPosition();
    rebuild();
}


    template <int n>
Sphere<n>::Sphere(const Sphere<n>& o)
    : center(o.center),radius(o.radius), segments(o.segments)
{
    recalcPosition();
    rebuild();
}

    template <int n>
Sphere<n>::~Sphere()
{
    for (int i = 0; i < childCount; ++i)
        delete children[i];
}



    template <int n>
void Sphere<n>::add(Shape<n>* child)
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
void Sphere<n>::remove(Shape<n>* child)
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
Sphere<n>& Sphere<n>::operator*=(const Matrix<n,n>& mat)
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


    recalcPosition();
    rebuild();
    return *this;
}

template <int n>
float* Sphere<n>::getPoints() const
{
    const int totalFloats = 2 * segments * 3 * n;
    float* pts = new float[totalFloats];
    int idx = 0;

    for (int i = 0; i < segments; ++i) {
    }
    return pts;
}

template <int n>
int Sphere<n>::getNumPoints() const
{
    const int stacks = segments/2;
    return (2 * segments + (stacks - 2) * segments * 2) * 3;
}

template <int n>
void Sphere<n>::print() const
{
    std::cout << "Cone:\n";
    std::cout << " radius=" << radius << " segments=" << segments << "\n";
}

template <int n>
void Sphere<n>::render(Renderer<n>& r) const
{
    for (int i = 0; i < childCount; ++i)
        children[i]->render(r);
}

    template <int n>
void Sphere<n>::setColor(float r, float g, float b, float a)
{
    Shape<n>::setColor(r, g, b, a);
    for (int i = 0; i < childCount; ++i)
        children[i]->setColor(r, g, b, a);
}

    template <int n>
void Sphere<n>::rotate(Vector<n> angles, Vector<n> rotate_point, bool hasCentroid)
{
    rotate3D(angles, rotate_point, hasCentroid);
}


    template <int n>
Vector<n> Sphere<n>::rotatePoint(const Vector<n>& point, const Vector<n>& pivot, const Matrix<n,n>& R)
{
    Vector<n> translated = point - pivot;
    Vector<n> rotated = R * translated;           // Uses Matrix::operator*(Vector)
    return rotated + pivot;
}

    template <int n>
void Sphere<n>::rotate3D(const Vector<n>& angles,
        Vector<n> pivot,
        bool hasPivot)
{
    if (!hasPivot)
        pivot = this->position;

    // Use Matrix<n,n>::makeRotation as requested
    Matrix<n,n> R = Matrix<n,n>::makeRotation(angles[0], angles[1], angles[2]);
    center = Matrix<n,n>::rotatePoint(center, pivot, R);

    rotation = R * rotation;


    recalcPosition();
    rebuild();
}

    template<int n>
void Sphere<n>::rotateAroundAxis(const Vector<n>& axis, float angle, Vector<n> pivot)
{
    Matrix<n,n> R = Matrix<n,n>::makeRotationAroundAxis(axis, angle);
    center = Matrix<n,n>::rotatePoint(center, pivot, R);

    rotation = R * rotation;

    recalcPosition();
    rebuild();
}

    template <int n>
void Sphere<n>::applyTranslation(const Vector<n>& disp)
{

    if (!this->physicsBodyActive())
        this->position = this->position + disp;

    for (int i = 0; i < childCount; ++i)
        children[i]->applyTranslation(disp);
}

    template <int n>
void Sphere<n>::updatePhysics(float dt)
{
    Shape<n>::updatePhysics(dt);
    for (int i = 0; i < childCount; ++i)
        children[i]->updatePhysics(dt);
}

    template <int n>
void Sphere<n>::scale(float s)
{
    Vector<n> c = this->position;
    radius    *= s;

    rebuild();
    setColor(this->color[0], this->color[1], this->color[2], this->color[3]);
}

template <int n>
    void Sphere<n>::addSegments(int count)
{
    segments += count;
    recalcPosition();
    rebuild();


}

template class Sphere<3>;
