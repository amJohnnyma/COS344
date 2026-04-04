#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>
#include <iomanip>

template <int n>
class Vector;

template<int n, int m>
class Matrix
{
protected:
    float **arr;

public:
    Matrix();
    Matrix(float **);
    Matrix(const Matrix<n,m> &);
    virtual ~Matrix();

    float *&operator[](int index) const
    {
        if (index >= n || index < 0)
            throw "Invalid index";
        return arr[index];
    }

    Matrix<n,m>& operator=(const Matrix<n,m>&);

    template<int a>
    Matrix<n,a> operator*(const Matrix<m,a>) const;

    Matrix<n,m> operator*(const float) const;
    Matrix<n,m> operator+(const Matrix<n,m>) const;
    Matrix<m,n> operator~() const;   // transpose

    void print() const
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
                std::cout << arr[i][j] << " ";
            std::cout << std::endl;
        }
    }

    int getM() const;
    int getN() const;

    float** get_arr() const { return arr; }
    float** get_arr()       { return arr; }

    float determinant() const;

    // Rotation Helpers
    static Matrix<n,n> makeRotation(float aX, float aY, float aZ)
    {
        const float cx = std::cos(aX), sx = std::sin(aX);
        const float cy = std::cos(aY), sy = std::sin(aY);
        const float cz = std::cos(aZ), sz = std::sin(aZ);

        Matrix<n,n> Rx;
        Rx[0][0]=1; Rx[0][1]=0; Rx[0][2]=0;
        Rx[1][0]=0; Rx[1][1]=cx; Rx[1][2]=-sx;
        Rx[2][0]=0; Rx[2][1]=sx; Rx[2][2]=cx;


        Matrix<n,n> Ry;
        Ry[0][0]=cy; Ry[0][1]=0; Ry[0][2]=sy;
        Ry[1][0]=0; Ry[1][1]=1; Ry[1][2]=0;
        Ry[2][0]=-sy; Ry[2][1]=0; Ry[2][2]=cy;


        Matrix<n,n> Rz;
        Rz[0][0]=cz; Rz[0][1]=-sz; Rz[0][2]=0;
        Rz[1][0]=sz; Rz[1][1]=cz; Rz[1][2]=0;
        Rz[2][0]=0; Rz[2][1]=0; Rz[2][2]=1;

        return Rz * Ry * Rx;
    }


    static Vector<n> rotatePoint(
            const Vector<n>& p,
            const Vector<n>& pivot,
            const Matrix<n,n>& R)
    {
        Vector<n> local = p - pivot;
        Vector<n> rotated;
        for(int i =0; i < n; ++i)
        {
            float sum = 0.0f;
            for(int j =0; j < n; ++j) sum += R[i][j] * local[j];
            rotated[i] = sum;

        }
        return rotated + pivot;
    }
};

#endif /*MATRIX_H*/
