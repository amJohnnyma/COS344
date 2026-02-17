#include "../include/Triangle.h"

template<int n>
Triangle<n>::Triangle(const Vector<n>& p1, const Vector<n>& p2, const Vector<n>& p3)
{

}
template<int n>
Triangle<n>::Triangle(const Triangle<n>&) 
{

}
template<int n>
Triangle<n>& Triangle<n>::operator*=(const Matrix<n,n>&) 
{

}
template<int n>
Triangle<n>* Triangle<n>::operator*(const Matrix<n,n>&) const 
{

}
template<int n>
float* Triangle<n>::getPoints() const 
{

}
template<int n>
int Triangle<n>::getNumPoints() const 
{
    return 3;
}
