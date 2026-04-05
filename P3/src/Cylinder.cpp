#include "../include/math/Cylinder.h"


template <int n>
void Cylinder<n>::recalcPosition()
{
    for (int k = 0; k < n; ++k)
        this->position[k] = (topCentre[k] + botCentre[k]) * 0.5f;
}

template <int n>
Vector<n> Cylinder<n>::rimPoint(const Vector<n>& centre, int i) const
{
    const float twoPi = 2.0f * 3.14159265358979323846f;
    float t = twoPi * i / segments;

    Vector<n> axis = topCentre - botCentre;
    float mag = axis.magnitude();
    if (mag < 1e-12f) return centre;

    Vector<n> axisN = axis * (1.0f / mag);

    // Choose reference vector not collinear with axisN
    Vector<n> worldRef{};
    if (std::abs(axisN[1]) < 0.99f) {
        worldRef[1] = 1.0f;
    } else {
        worldRef[0] = 1.0f;
    }

    Vector<n> right = axisN.crossProduct(worldRef);
    float rightMag = right.magnitude();
    if (rightMag > 1e-12f)
        right = right * (1.0f / rightMag);

    Vector<n> up = axisN.crossProduct(right);

    return centre + right * (radius * std::cos(t))
                  + up   * (radius * std::sin(t));
}

template <int n>
void Cylinder<n>::rebuild()
{
    for (int i = 0; i < childCount; ++i)
        delete children[i];

    childCount = 0;

    for (int i = 0; i < segments; ++i) {
        Vector<n> t0 = rimPoint(topCentre, i);
        Vector<n> t1 = rimPoint(topCentre, (i + 1) % segments);
        Vector<n> b0 = rimPoint(botCentre, i);
        Vector<n> b1 = rimPoint(botCentre, (i + 1) % segments);

        // Lateral quad split into 2 triangles (CCW from outside)
        add(new Triangle<n>(t0, b0, b1));
        add(new Triangle<n>(t0, b1, t1));

        // Top cap
        add(new Triangle<n>(topCentre, t0, t1));

        // Bottom cap
        add(new Triangle<n>(botCentre, b1, b0));
    }

    setColor(this->color[0], this->color[1], this->color[2], this->color[3]);
}


template <int n>
Cylinder<n>::Cylinder(const Vector<n>& top_centre,
                      const Vector<n>& bot_centre,
                      float r,
                      int segs)
    : topCentre(top_centre), botCentre(bot_centre), radius(r), segments(segs)
{
    recalcPosition();
    rebuild();
}

template <int n>
Cylinder<n> Cylinder<n>::upright(const Vector<n>& base_centre,
                                 float r, float height, int segs)
{
    Vector<n> top = base_centre;
    top[1] += height;
    return Cylinder<n>(top, base_centre, r, segs);
}

template <int n>
Cylinder<n>::Cylinder(const Cylinder<n>& o)
    : topCentre(o.topCentre), botCentre(o.botCentre),
      radius(o.radius), segments(o.segments)
{
    recalcPosition();
    rebuild();
}

template <int n>
Cylinder<n>::~Cylinder()
{
    for (int i = 0; i < childCount; ++i)
        delete children[i];
}


    template <int n>
void Cylinder<n>::add(Shape<n>* child)
{
    if (childCount >= MAX_CHILDREN) {
        std::cerr << "ERROR: Too many children in Cylinder!\n";
        delete child;
        return;
    }
    if (child == nullptr) return;
    children[childCount++] = child;
}

    template <int n>
void Cylinder<n>::remove(Shape<n>* child)
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
Cylinder<n>& Cylinder<n>::operator*=(const Matrix<n,n>& mat)
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

    applyMat(topCentre);
    applyMat(botCentre);

    recalcPosition();
    rebuild();
    return *this;
}

template <int n>
float* Cylinder<n>::getPoints() const
{
    const int totalFloats = 4 * segments * 3 * n;
    float* pts = new float[totalFloats];
    int idx = 0;

    auto pushTri = [&](const Vector<n>& a, const Vector<n>& b, const Vector<n>& c) {
        for (int k = 0; k < n; ++k) pts[idx++] = a[k];
        for (int k = 0; k < n; ++k) pts[idx++] = b[k];
        for (int k = 0; k < n; ++k) pts[idx++] = c[k];
    };

    for (int i = 0; i < segments; ++i) {
        Vector<n> t0 = rimPoint(topCentre, i);
        Vector<n> t1 = rimPoint(topCentre, (i + 1) % segments);
        Vector<n> b0 = rimPoint(botCentre, i);
        Vector<n> b1 = rimPoint(botCentre, (i + 1) % segments);

        pushTri(t0, b0, b1);     // lateral 1
        pushTri(t0, b1, t1);     // lateral 2
        pushTri(topCentre, t1, t0); // top cap
        pushTri(botCentre, b0, b1); // bottom cap
    }
    return pts;
}

template <int n>
int Cylinder<n>::getNumPoints() const
{
    return 4 * segments * 3;
}

template <int n>
void Cylinder<n>::print() const
{
    std::cout << "Cylinder:\n";
    std::cout << " TopCentre: "; topCentre.print();
    std::cout << " BotCentre: "; botCentre.print();
    std::cout << " radius=" << radius << " segments=" << segments << "\n";
}

template <int n>
void Cylinder<n>::render(Renderer<n>& r) const
{
    for (int i = 0; i < childCount; ++i)
        children[i]->render(r);
}

template <int n>
void Cylinder<n>::setColor(float r, float g, float b, float a)
{
    Shape<n>::setColor(r, g, b, a);
    for (int i = 0; i < childCount; ++i)
        children[i]->setColor(r, g, b, a);
}

template <int n>
void Cylinder<n>::rotate(Vector<n> angles, Vector<n> rotate_point, bool hasCentroid)
{
    rotate3D(angles, rotate_point, hasCentroid);
}



template <int n>
void Cylinder<n>::rotate3D(const Vector<n>& angles,
                           Vector<n> pivot,
                           bool hasPivot)
{
    if (!hasPivot)
        pivot = this->position;

    Matrix<n,n> R = Matrix<n,n>::makeRotation(angles[0], angles[1], angles[2]);

    topCentre = Matrix<n,n>::rotatePoint(topCentre, pivot, R);
    botCentre = Matrix<n,n>::rotatePoint(botCentre, pivot, R);

    recalcPosition();
    rebuild();
}

template <int n>
void Cylinder<n>::applyTranslation(const Vector<n>& disp)
{
    topCentre = topCentre + disp;
    botCentre = botCentre + disp;

    if (!this->physicsBodyActive())
        this->position = this->position + disp;

    for (int i = 0; i < childCount; ++i)
        children[i]->applyTranslation(disp);
}

template <int n>
void Cylinder<n>::updatePhysics(float dt)
{
    Shape<n>::updatePhysics(dt);
    for (int i = 0; i < childCount; ++i)
        children[i]->updatePhysics(dt);
}

template <int n>
void Cylinder<n>::scale(float s)
{
    Vector<n> c = this->position;
    topCentre = c + (topCentre - c) * s;
    botCentre = c + (botCentre - c) * s;
    radius *= s;

    rebuild();
    this->setColor(this->color[0], this->color[1],
                   this->color[2], this->color[3]);
}

template class Cylinder<3>;
