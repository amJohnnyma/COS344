#include "../include/math/Vector.h"
#include <cmath>
#include <iostream>


template<int n>
Vector<n>::Vector() : arr(new float[n]())
{

}

template<int n>
Vector<n>::Vector(std::initializer_list<float> init) : arr(new float[n]())
{
    std::size_t i = 0;
    for (float it : init)
    {
        if (i > n) return;
        arr[i] = it;
        i ++;
    }

}

template<int n>
Vector<n>::Vector(float* values) : arr(values)
{
}

template<int n>
Vector<n>::~Vector()
{
   delete[] arr;
   arr = nullptr;
}


template<int n>
Vector<n>::Vector(const Vector<n>& other) : arr(new float[n])
{
    std::copy(other.arr, other.arr + n, arr);
}

template<int n>
Vector<n>::Vector(const Matrix<n,1>& mat)
{
    arr = new float[n]();
    for(int i = 0; i < n; i ++)
    {
        arr[i] = mat.get_arr()[i][0];
    }
}


template<int n>
Vector<n>& Vector<n>::operator=(const Vector<n>& other)
{
    if (this == &other) {
        return *this;
    }

    float* temp = new float[n];
    std::copy(other.arr, other.arr + n, temp);

    delete[] arr;

    arr = temp;

    return *this;
}


template<int n>
Vector<n> Vector<n>::operator+(const Vector<n> other) const
{
    Vector<n> result;
    for(int i = 0; i < n; i ++)
    {
        result.arr[i] = arr[i] + other.arr[i];
    }
    return result;
}

template<int n>
Vector<n> Vector<n>::operator-(const Vector<n> other) const
{
    Vector<n> result;
    for(int i = 0; i < n; i ++)
    {
        result.arr[i] = arr[i] - other.arr[i];
    }
    return result;
}

template<int n>
Vector<n> Vector<n>::operator*(const float scalar) const
{

    Vector<n> result;
    for(int i = 0; i < n; i ++)
    {
        result.arr[i] = arr[i] * scalar;
    }
    return result;
}

template<int n>
float Vector<n>::operator*(const Vector<n> other) const    // dot product
{
    float result = 0.0f;
    for(int i = 0; i < n; i ++)
    {
        result += (arr[i] * other.arr[i]);
    }

    return result;
}


template<int n>
float Vector<n>::magnitude() const
{
    float result = 0.0f;
    for(int i = 0; i < n; i++)
    {
        result += std::pow(arr[i], 2);
    }
    result = std::sqrt(result);
    return result;
}

template<int n>
Vector<n>::operator Matrix<n,1>() const
{
    Matrix<n,1> m;      

    for (int i = 0; i < n; ++i) {
        m[i][0] = arr[i];          
    }

    return m;
}

template<int n>
Vector<3> Vector<n>::crossProduct(const Vector<3> other) const
{
    if (n != 3) return Vector<3>(); // maybe make throw
//a2b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1
    Vector<3> result;
    result.get_arr()[0] = (arr[1] * other.get_arr()[2]) - (arr[2] * other.get_arr()[1]); 
    result.get_arr()[1] = (arr[2] * other.get_arr()[0]) - (arr[0] * other.get_arr()[2]); 
    result.get_arr()[2] = (arr[0] * other.get_arr()[1]) - (arr[1] * other.get_arr()[0]); 

    return result;

}

template<int n>
Vector<n> Vector<n>::unitVector() const
{
    float mag = magnitude();
    if (mag == 0.0f || std::isnan(mag)) {
        throw "Invalid unit vector";
    }

    Vector<n> result(*this);
    float inv = 1.0f / mag;
    for (int i = 0; i < n; ++i) {
        result.arr[i] *= inv;
    }
    return result;
}

template<int n>
int Vector<n>::getN() const
{
    return n;
}

template class Vector<2>;
template class Vector<3>;