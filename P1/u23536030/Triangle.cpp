#include "Triangle.h"

template<int n>
Triangle<n>::Triangle(const Vector<n>& p1, const Vector<n>& p2, const Vector<n>& p3)
{
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;

}
template<int n>
Triangle<n>::Triangle(const Triangle<n>& other) 
{
    p1 = Vector<n>(other.p1);
    p2 = Vector<n>(other.p2);
    p3 = Vector<n>(other.p3);

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
Triangle<n>* Triangle<n>::operator*(const Matrix<n,n>& mat) const
{

    Triangle<n>* result = new Triangle<n>(*this);
    *result *= mat;
    return result;
}
template<int n>
float* Triangle<n>::getPoints() const 
{

    float* points = new float[3 * n];
    int idx = 0;

    // p1
    for (int k = 0; k < n; ++k) points[idx++] = p1[k];
    // p2
    for (int k = 0; k < n; ++k) points[idx++] = p2[k];
    // p3
    for (int k = 0; k < n; ++k) points[idx++] = p3[k];

    return points;

}
template<int n>
int Triangle<n>::getNumPoints() const 
{
    return 3;
}
