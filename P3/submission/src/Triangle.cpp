#include "../include/math/Triangle.h"
#include "../include/engine/Renderer.h"
#include <cmath>

    template<int n>
Triangle<n>::Triangle(const Vector<n>& p1, const Vector<n>& p2, const Vector<n>& p3)
{
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    for (int i = 0; i < n; ++i)
        this->position[i] = (p1[i] + p2[i] + p3[i]) / 3.0f;

}
    template<int n>
Triangle<n>::Triangle(const Triangle<n>& other) 
{
    p1 = Vector<n>(other.p1);
    p2 = Vector<n>(other.p2);
    p3 = Vector<n>(other.p3);
    for (int i = 0; i < n; ++i)
        this->position[i] = (p1[i] + p2[i] + p3[i]) / 3.0f;

}
    template<int n>
Triangle<n>& Triangle<n>::operator*=(const Matrix<n,n>& mat) 
{

    // Create temporary copies so don't overwrite values too early
    Vector<n> old_p1 = p1;
    Vector<n> old_p2 = p2;
    Vector<n> old_p3 = p3;

    // For each corner: new_vector[i] = sum over j (other[i][j] * old_vector[j])
    for (int i = 0; i < n; ++i)
    {
        float sum_p1 = 0.0f;
        float sum_p2 = 0.0f;
        float sum_p3 = 0.0f;

        for (int j = 0; j < n; ++j)
        {
            // Using existing Matrix::operator[] → float*
            sum_p1 += mat[i][j] * old_p1[j];
            sum_p2 += mat[i][j] * old_p2[j];
            sum_p3 += mat[i][j] * old_p3[j];
        }

        p1[i] = sum_p1;
        p2[i] = sum_p2;
        p3[i] = sum_p3;
    }

    return *this;

}

template<int n>
float* Triangle<n>::getPoints() const 
{

    float* points = new float[3 * n];
    int idx = 0;

    // p3
    for (int k = 0; k < n; ++k) points[idx++] = p3[k];
    // p1
    for (int k = 0; k < n; ++k) points[idx++] = p1[k];
    // p2
    for (int k = 0; k < n; ++k) points[idx++] = p2[k];

    return points;

}
template<int n>
int Triangle<n>::getNumPoints() const 
{
    return 3;
}

template<int n>
void Triangle<n>::render(Renderer<n>& r) const
{   
    Color4 c(this->color[0], this->color[1], this->color[2], this->color[3]);

    r.drawShape(this, c);
}


template<int n>
void Triangle<n>::scale(float s)
{
    Vector<n> centroid;
    for (int i = 0; i < n; ++i)
        centroid[i] = (p1[i] + p2[i] + p3[i]) / 3.0f;

    p1 = centroid + (p1 - centroid) * s;
    p2 = centroid + (p2 - centroid) * s;
    p3 = centroid + (p3 - centroid) * s;
}
template<int n>
void Triangle<n>::recalcPosition()
{
    for (int i = 0; i < n; ++i)
        this->position[i] = (p1[i] + p2[i] + p3[i]) / 3.0f;
}

template<int n>
void Triangle<n>::rotate3D(const Vector<n>& angles,
        Vector<n> pivot,
        bool hasPivot)
{
    if (!hasPivot) pivot = this->position;

    Matrix<n,n> R = Matrix<n,n>::makeRotation(angles[0], angles[1], angles[2]);
    p1 = Matrix<n,n>::rotatePoint(p1, pivot, R);
    p2 = Matrix<n,n>::rotatePoint(p2, pivot, R);
    p3 = Matrix<n,n>::rotatePoint(p3, pivot, R);

    recalcPosition();
}

template<int n>
void Triangle<n>::rotate(Vector<n> angles,
        Vector<n> rotate_point,
        bool hasCentroid)
{
    rotate3D(angles, rotate_point, hasCentroid);
}

template<int n>
void Triangle<n>::rotateAroundAxis(const Vector<n>& axis, float angle, Vector<n> pivot)
{
    Matrix<n,n> R = Matrix<n,n>::makeRotationAroundAxis(axis, angle);
    p1 = Matrix<n,n>::rotatePoint(p1, pivot, R);
    p2 = Matrix<n,n>::rotatePoint(p2, pivot, R);
    p3 = Matrix<n,n>::rotatePoint(p3, pivot, R);
    recalcPosition();
}
template class Triangle<3>;
