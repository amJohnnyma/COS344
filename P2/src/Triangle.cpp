#include "../include/math/Triangle.h"
#include "../include/engine/Renderer.h"
#include <cmath>

    template<int n>
Triangle<n>::Triangle(const Vector<n>& p1, const Vector<n>& p2, const Vector<n>& p3)
{
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->position[0] = (p1[0] + p2[0] + p3[0]) / 3.0f;
    this->position[1] = (p1[1] + p2[1] + p3[1]) / 3.0f;
    for (int i = 2; i < n; ++i)
        this->position[i] = (p1[i] + p2[i] + p3[i]) / 3.0f;

}
    template<int n>
Triangle<n>::Triangle(const Triangle<n>& other) 
{
    p1 = Vector<n>(other.p1);
    p2 = Vector<n>(other.p2);
    p3 = Vector<n>(other.p3);
    this->position[0] = (p1[0] + p2[0] + p3[0]) / 3.0f;
    this->position[1] = (p1[1] + p2[1] + p3[1]) / 3.0f;
    for (int i = 2; i < n; ++i)
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

    template <int n>
void Triangle<n>::rotate(float theta, Vector<n> rotate_point, bool hasCentroid)
{
    if constexpr (n < 2) {
        return;
    }
    float radians = theta * (3.14159265f / 180.f);
    if (!hasCentroid)
    {
        rotate_point[0] = (p1[0] + p2[0] + p3[0]) / 3.0f;
        rotate_point[1] = (p1[1] + p2[1] + p3[1]) / 3.0f;
    }

    const float Cx = rotate_point[0];
    const float Cy = rotate_point[1];
    const float c  = std::cos(radians);
    const float s  = std::sin(radians);

    // Rotate one point
    auto do_rotate = [&](Vector<n>& p) {
        const float dx = p[0] - Cx;
        const float dy = p[1] - Cy;

        p[0] = Cx + dx * c - dy * s;
        p[1] = Cy + dx * s + dy * c;      };

    do_rotate(p1);
    do_rotate(p2);
    do_rotate(p3);


}

template class Triangle<2>;
template class Triangle<3>;
