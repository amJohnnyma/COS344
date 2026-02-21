#include "../include/math/Matrix.h"
#include "../include/math/Vector.h"
#include <cmath>       // for std::abs

template<int n, int m>
Matrix<n,m>::Matrix()
{
    arr = new float*[n];
    for(int i = 0; i < n; i++)
    {
        arr[i] = new float[m]();   // value-initialize to 0
    }
}

template<int n, int m>
Matrix<n,m>::Matrix(float **values) : arr(values)
{
}

// Copy constructor
template<int n, int m>
Matrix<n,m>::Matrix(const Matrix<n,m> &other)
{
    arr = new float*[n];
    for(int i = 0; i < n; i++)
    {
        arr[i] = new float[m];
        for(int j = 0; j < m; j++)
        {
            arr[i][j] = other.arr[i][j];
        }
    }
}

template<int n, int m>
Matrix<n,m>::~Matrix()
{
    for(int i = 0; i < n; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;
    arr = nullptr;
}

// Copy assignment
template<int n, int m>
Matrix<n,m>& Matrix<n,m>::operator=(const Matrix<n,m>& other)
{
    if(this == &other) return *this;

    // clean old data
    for(int i = 0; i < n; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;

    // allocate new
    arr = new float*[n];
    for(int i = 0; i < n; i++)
    {
        arr[i] = new float[m];
        for(int j = 0; j < m; j++)
        {
            arr[i][j] = other.arr[i][j];
        }
    }

    return *this;
}

// Matrix × Matrix (template parameter a = result columns)
template<int n, int m>
template<int a>
Matrix<n,a> Matrix<n,m>::operator*(const Matrix<m,a> other) const
{
    Matrix<n,a> result;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < a; j++)
        {
            result[i][j] = 0.0f;
            for(int k = 0; k < m; k++)
            {
                result[i][j] += arr[i][k] * other[k][j];
            }
        }
    }
    return result;
}

// Matrix × scalar
template<int n, int m>
Matrix<n,m> Matrix<n,m>::operator*(const float scalar) const
{
    Matrix<n,m> result;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            result[i][j] = arr[i][j] * scalar;
        }
    }
    return result;
}

// Matrix + Matrix
template<int n, int m>
Matrix<n,m> Matrix<n,m>::operator+(const Matrix<n,m> other) const
{
    Matrix<n,m> result;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            result[i][j] = arr[i][j] + other.arr[i][j];
        }
    }
    return result;
}

// Transpose (~ operator)
template<int n, int m>
Matrix<m,n> Matrix<n,m>::operator~() const
{
    Matrix<m,n> result;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            result[j][i] = arr[i][j];
        }
    }
    return result;
}

template<int n, int m>
int Matrix<n,m>::getM() const
{
    return m;
}

template<int n, int m>
int Matrix<n,m>::getN() const
{
    return n;
}

// Determinant using Gaussian elimination with partial pivoting
template<int n, int m>
float Matrix<n,m>::determinant() const
{
    if (n != m) {
        return 0.0f;
    }

    if (n == 0) {
        return 1.0f;
    }

    // Local copy - will modify it
    float mat[n][n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            mat[i][j] = arr[i][j];
        }
    }

    float det = 1.0f;
    int sign = 1;

    for (int p = 0; p < n; ++p) {
        // Find pivot
        int max_row = p;
        for (int i = p + 1; i < n; ++i) {
            if (std::abs(mat[i][p]) > std::abs(mat[max_row][p])) {
                max_row = i;
            }
        }

        // Swap rows
        if (max_row != p) {
            sign = -sign;
            for (int j = 0; j < n; ++j) {
                std::swap(mat[p][j], mat[max_row][j]);
            }
        }

        float pivot = mat[p][p];
        if (std::abs(pivot) < 1e-12f) {
            return 0.0f;
        }

        det *= pivot;

        // Eliminate below current pivot
        for (int i = p + 1; i < n; ++i) {
            float factor = mat[i][p] / pivot;
            for (int j = p; j < n; ++j) {
                mat[i][j] -= factor * mat[p][j];
            }
        }
    }

    return (sign < 0 ? -det : det);
}

template class Matrix<2,2>;
template class Matrix<3,3>;
template class Matrix<4,4>;
template class Matrix<2,1>;
template class Matrix<3,1>;
