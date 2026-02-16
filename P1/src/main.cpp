#include "../include/Matrix.h"
#include "../include/Vector.h"
//#include "../include/Shape.h"
//#include "../include/Triangle.h"
//#include "../include/Square.h"
#include <iostream>

#include <sstream>

bool approxEqual(float a, float b, float epsilon = 1e-5f) {
    return std::abs(a - b) < epsilon;
} // incase floats are being funny again

void printTestResult(const std::string& testName, bool passed) {
    std::cout << std::left << std::setw(45) << testName
              << (passed ? "[PASS]" : "[FAIL]") << "\n";
}

template<int rows>
Matrix<rows, 1> to_column_vector(const Vector<rows>& v) {
    Matrix<rows, 1> col;
    for (int i = 0; i < rows; ++i) {
        col[i][0] = v[i];
    }
    return col;
}

bool vector_tests()
{
    std::cout << "Vector<" << 3 << "> tests\n\n";
    bool allPassed = true;

    //Default constructor
    {
        Vector<3> v;
        bool ok = true;
        for (int i = 0; i < 3; ++i) {
            if (!approxEqual(v[i], 0.0f)) ok = false;
        }
        printTestResult("Default constructor → zero vector", ok);
        allPassed &= ok;
    }

    //Initializer list constructor
    {
        Vector<3> v{1.0f, -2.0f, 4.0f};
        bool ok = approxEqual(v[0], 1) && approxEqual(v[1], -2) && approxEqual(v[2], 4);
        printTestResult("Initializer list constructor", ok);
        allPassed &= ok;
    }

    //Constructor from raw float*
    {
        // this shallow copy makes no sense but ok
        float* raw = new float[3]{5.0f, 6.0f, -1.0f};
        Vector<3> v(raw);
        bool ok = approxEqual(v[0], 5) && approxEqual(v[1], 6) && approxEqual(v[2], -1);
        printTestResult("Constructor from float*", ok);
        allPassed &= ok;
    }

    //Copy constructor 
    {
        Vector<3> a{10, 20, 30};
        Vector<3> b(a);
        bool ok = approxEqual(b[0], 10) && approxEqual(b[1], 20) && approxEqual(b[2], 30);
        printTestResult("Copy constructor", true);
        allPassed &= ok;
    }

    //Copy assignment
    {
        Vector<3> a{7, 8, 9};
        Vector<3> b;
        b = a;
        bool ok = approxEqual(b[0], 7) && approxEqual(b[1], 8) && approxEqual(b[2], 9);
        printTestResult("Copy assignment operator=", ok);
        allPassed &= ok;
    }

    //operator+ 
    {
        Vector<3> a{1, 2, 3};
        Vector<3> b{4, -1, 5};
        Vector<3> sum = a + b;
        bool ok = approxEqual(sum[0], 5) && approxEqual(sum[1], 1) && approxEqual(sum[2], 8);
        printTestResult("operator+ (addition)", ok);
        allPassed &= ok;
    }

    //operator-
    {
        Vector<3> a{10, 20, 30};
        Vector<3> b{3, 5, 7};
        Vector<3> diff = a - b;
        bool ok = approxEqual(diff[0], 7) && approxEqual(diff[1], 15) && approxEqual(diff[2], 23);
        printTestResult("operator- (subtraction)", ok);
        allPassed &= ok;
    }

    //operator* (scalar)
    {
        Vector<3> v{2, -3, 4};
        Vector<3> res = v * 2.5f;
        bool ok = approxEqual(res[0], 5) && approxEqual(res[1], -7.5f) && approxEqual(res[2], 10);
        printTestResult("operator* (scalar multiplication)", ok);
        allPassed &= ok;
    }

    //operator* (dot product)
    {
        Vector<3> a{1, 2, 3};
        Vector<3> b{4, 5, 6};
        float dot = a * b;
        bool ok = approxEqual(dot, 1*4 + 2*5 + 3*6, 1e-4f);
        printTestResult("operator* (dot product)", ok);
        allPassed &= ok;
    }

    //magnitude()
    {
        Vector<3> v{3, 4, 0};
        float mag = v.magnitude();
        bool ok = approxEqual(mag, 5.0f);
        printTestResult("magnitude() / length", ok);
        allPassed &= ok;
    }

    //unitVector()
    {
        Vector<3> v{3, 4, 0};
        Vector<3> u = v.unitVector();
        float len = u.magnitude();
        bool ok = approxEqual(len, 1.0f, 1e-5f);
        ok &= approxEqual(u[0], 0.6f, 1e-5f);
        ok &= approxEqual(u[1], 0.8f, 1e-5f);
        ok &= approxEqual(u[2], 0.0f, 1e-5f);
        printTestResult("unitVector() / normalize", ok);
        allPassed &= ok;
    }

    //crossProduct
    {
        Vector<3> a{1, 0, 0};
        Vector<3> b{0, 1, 0};
        Vector<3> c = a.crossProduct(b);
        bool ok = approxEqual(c[0], 0) && approxEqual(c[1], 0) && approxEqual(c[2], 1);
        printTestResult("crossProduct (i × j = k)", ok);
        allPassed &= ok;
    }

    //operator[] const (already used above, but explicit test)
    {
        const Vector<3> v{10, 20, 30};
        bool ok = true;
        try {
            float x = v[1];   (void)x;
            try { float y = v[5]; (void)y; ok = false; } catch (...) {}
        } catch (...) {
            ok = false;
        }
        printTestResult("operator[] const + bounds check", ok);
        allPassed &= ok;
    }

    //getN()
    {
        Vector<3> v;
        bool ok = (v.getN() == 3);
        printTestResult("getN()", ok);
        allPassed &= ok;
    }

    //print() — visual check (no automated assert)
    {
        std::cout << "\nVisual check - print():\n";
        Vector<3> demo{1.5f, -0.75f, 2.0f};
        demo.print();
        std::cout << "(should see 3 numbers above)\n";
    }

    std::cout << "\n───────────────────────────────────────\n";
    std::cout << (allPassed ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << "\n";
    std::cout << "───────────────────────────────────────\n";

    return allPassed ? 0 : 1;
}
bool matrix_tests()
{
    std::cout << "Matrix tests (various small sizes)\n\n";
    bool allPassed = true;

    // 2×3 Matrix tests
    std::cout << "Testing Matrix<2,3>\n";

    // Default constructor (zero-filled)
    {
        Matrix<2,3> m;
        bool ok = true;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 3; ++j)
                if (!approxEqual(m[i][j], 0.0f)) ok = false;
        printTestResult("Default constructor → zero matrix 2×3", ok);
        allPassed &= ok;
    }

    // Copy constructor
    {
        Matrix<2,3> a;
        a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
        a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;

        Matrix<2,3> b(a);
        bool ok = approxEqual(b[0][0],1) && approxEqual(b[0][1],2) && approxEqual(b[0][2],3) &&
                  approxEqual(b[1][0],4) && approxEqual(b[1][1],5) && approxEqual(b[1][2],6);
        printTestResult("Copy constructor", ok);
        allPassed &= ok;
    }

    // Copy assignment
    {
        Matrix<2,3> a;
        a[0][0] = 7; a[0][1] = -1; a[0][2] = 0;
        a[1][0] = 3; a[1][1] = 9;  a[1][2] = -4;

        Matrix<2,3> b;
        b = a;

        bool ok = approxEqual(b[0][0],7) && approxEqual(b[0][1],-1) && approxEqual(b[0][2],0) &&
                  approxEqual(b[1][0],3) && approxEqual(b[1][1],9)  && approxEqual(b[1][2],-4);
        printTestResult("Copy assignment operator=", ok);
        allPassed &= ok;
    }

    // Addition
    {
        Matrix<2,3> a, b;
        a[0][0]=1; a[0][1]=2; a[0][2]=3;
        a[1][0]=4; a[1][1]=5; a[1][2]=6;

        b[0][0]=7; b[0][1]=-1;b[0][2]=0;
        b[1][0]=-2;b[1][1]=3; b[1][2]=8;

        Matrix<2,3> sum = a + b;

        bool ok = approxEqual(sum[0][0],8)  && approxEqual(sum[0][1],1)  && approxEqual(sum[0][2],3) &&
                  approxEqual(sum[1][0],2)  && approxEqual(sum[1][1],8)  && approxEqual(sum[1][2],14);
        printTestResult("operator+ (addition)", ok);
        allPassed &= ok;
    }

    // Scalar multiplication
    {
        Matrix<2,3> m;
        m[0][0]=1; m[0][1]=-2; m[0][2]=4;
        m[1][0]=0; m[1][1]=3;  m[1][2]=-1;

        Matrix<2,3> res = m * 2.5f;

        bool ok = approxEqual(res[0][0],2.5f) && approxEqual(res[0][1],-5.0f) && approxEqual(res[0][2],10.0f) &&
                  approxEqual(res[1][0],0.0f) && approxEqual(res[1][1],7.5f)  && approxEqual(res[1][2],-2.5f);
        printTestResult("operator* (scalar)", ok);
        allPassed &= ok;
    }

    // Matrix multiplication 2×3 × 3×4 → 2×4
    {
        Matrix<2,3> a;
        a[0][0]=1; a[0][1]=2; a[0][2]=3;
        a[1][0]=4; a[1][1]=5; a[1][2]=6;

        Matrix<3,4> b;
        b[0][0]=7;  b[0][1]=8;  b[0][2]=9;  b[0][3]=10;
        b[1][0]=11; b[1][1]=12; b[1][2]=13; b[1][3]=14;
        b[2][0]=15; b[2][1]=16; b[2][2]=17; b[2][3]=18;

        Matrix<2,4> c = a * b;

        bool ok = approxEqual(c[0][0], 74) && approxEqual(c[0][1], 80) && approxEqual(c[0][2], 86) && approxEqual(c[0][3], 92) &&
                  approxEqual(c[1][0],173) && approxEqual(c[1][1],188) && approxEqual(c[1][2],203) && approxEqual(c[1][3],218);
        printTestResult("operator* (matrix multiplication 2×3 × 3×4)", ok);
        allPassed &= ok;
    }

    // Transpose (~)
    {
        Matrix<2,3> a;
        a[0][0]=1; a[0][1]=2; a[0][2]=3;
        a[1][0]=4; a[1][1]=5; a[1][2]=6;

        Matrix<3,2> t = ~a;

        bool ok = approxEqual(t[0][0],1) && approxEqual(t[0][1],4) &&
                  approxEqual(t[1][0],2) && approxEqual(t[1][1],5) &&
                  approxEqual(t[2][0],3) && approxEqual(t[2][1],6);
        printTestResult("operator~ (transpose)", ok);
        allPassed &= ok;
    }

    // Square matrix – Determinant tests
    std::cout << "\nTesting determinant (square matrices)\n";

    // 1×1
    {
        Matrix<1,1> m;
        m[0][0] = 7.0f;
        float det = m.determinant();
        bool ok = approxEqual(det, 7.0f);
        printTestResult("determinant 1×1", ok);
        allPassed &= ok;
    }

    // 2×2
    {
        Matrix<2,2> m;
        m[0][0] = 4; m[0][1] = 7;
        m[1][0] = 2; m[1][1] = 6;

        float det = m.determinant();  // 4*6 - 7*2 = 24 - 14 = 10
        bool ok = approxEqual(det, 10.0f);
        printTestResult("determinant 2×2", ok);
        allPassed &= ok;
    }

    // 3×3 (classic example)
    {
        Matrix<3,3> m;
        m[0][0]= 2; m[0][1]=-1; m[0][2]= 0;
        m[1][0]=-1; m[1][1]= 2; m[1][2]=-1;
        m[2][0]= 0; m[2][1]=-1; m[2][2]= 2;

        float det = m.determinant();  // expected = 4
        bool ok = approxEqual(det, 4.0f, 1e-4f);
        printTestResult("determinant 3×3 (simple tridiagonal)", ok);
        allPassed &= ok;
    }

    // Singular matrix → det = 0
    {
        Matrix<3,3> m;
        m[0][0]=1; m[0][1]=2; m[0][2]=3;
        m[1][0]=4; m[1][1]=5; m[1][2]=6;
        m[2][0]=7; m[2][1]=8; m[2][2]=9;

        float det = m.determinant();
        bool ok = approxEqual(det, 0.0f, 1e-4f);
        printTestResult("determinant 3×3 (singular)", ok);
        allPassed &= ok;
    }

    // Non-square → should return 0
    {
        Matrix<2,4> nonsq;
        float det = nonsq.determinant();
        bool ok = approxEqual(det, 0.0f);
        printTestResult("determinant non-square matrix → 0", ok);
        allPassed &= ok;
    }

    // Visual check – print()
    {
        std::cout << "\nVisual check - print() 3×3:\n";
        Matrix<3,3> demo;
        demo[0][0]=1.5f; demo[0][1]=-0.2f; demo[0][2]=3.7f;
        demo[1][0]=0.0f; demo[1][1]=4.1f;  demo[1][2]=-2.9f;
        demo[2][0]=5.6f; demo[2][1]=1.0f;  demo[2][2]=0.0f;
        demo.print();
        std::cout << "(should see 3×3 matrix above)\n";
    }

    std::cout << "\n──────────────────────────────────────────────\n";
    std::cout << (allPassed ? "ALL MATRIX TESTS PASSED" : "SOME MATRIX TESTS FAILED") << "\n";
    std::cout << "──────────────────────────────────────────────\n";

    return allPassed;
}

bool matrix_vector_tests()
{
    std::cout << "Matrix × Vector interaction tests\n\n";
    bool allPassed = true;

    // Matrix<3,3> × Vector<3>
    {
        Matrix<3,3> A;
        A[0][0] = 1; A[0][1] = 0; A[0][2] = 0;
        A[1][0] = 0; A[1][1] = 2; A[1][2] = 0;
        A[2][0] = 0; A[2][1] = 0; A[2][2] = 3;

        Vector<3> v{5.0f, 7.0f, 11.0f};

        Matrix<3,1> result = A * to_column_vector(v);

        bool ok = approxEqual(result[0][0], 5.0f) &&
                  approxEqual(result[1][0], 14.0f) &&
                  approxEqual(result[2][0], 33.0f);

        printTestResult("Matrix<3,3> × Vector<3> (diagonal scaling)", ok);
        allPassed &= ok;
    }

    // Matrix<2,4> × Vector<4> → "Vector<2>"
    {
        Matrix<2,4> M;
        M[0][0] = 1; M[0][1] = 2; M[0][2] = 3; M[0][3] = 4;
        M[1][0] = 5; M[1][1] = 6; M[1][2] = 7; M[1][3] = 8;

        Vector<4> u{1.0f, -1.0f, 0.5f, 2.0f};

        Matrix<2,1> res = M * to_column_vector(u);

        // Expected:
        // row0: 1*1 + 2*(-1) + 3*0.5 + 4*2 = 1 - 2 + 1.5 + 8 = 8.5
        // row1: 5*1 + 6*(-1) + 7*0.5 + 8*2 = 5 - 6 + 3.5 + 16 = 18.5
        bool ok = approxEqual(res[0][0], 8.5f, 1e-4f) &&
                  approxEqual(res[1][0], 18.5f, 1e-4f);

        printTestResult("Matrix<2,4> × Vector<4>", ok);
        allPassed &= ok;
    }

    // Visual check
    {
        std::cout << "\nVisual check - Matrix<3,3> × Vector<3>:\n";
        Matrix<3,3> demo;
        demo[0][0]=1; demo[0][1]=2; demo[0][2]=3;
        demo[1][0]=0; demo[1][1]=1; demo[1][2]=4;
        demo[2][0]=5; demo[2][1]=0; demo[2][2]=1;

        Vector<3> vec{2, 1, 0};

        auto prod = demo * to_column_vector(vec);

        std::cout << "Matrix:\n"; demo.print();
        std::cout << "\nVector:\n" << vec[0] << " " << vec[1] << " " << vec[2] << "\n\n";
        std::cout << "Result (as 3×1 matrix):\n"; prod.print();
    }

    std::cout << "\n──────────────────────────────────────────────\n";
    std::cout << (allPassed ? "ALL MATRIX-VECTOR TESTS PASSED" : "SOME MATRIX-VECTOR TESTS FAILED") << "\n";
    std::cout << "──────────────────────────────────────────────\n";

    return allPassed;
}

int main(int argc, char const *argv[])
{
    bool vectorPass = vector_tests();
    bool matrixPass = matrix_tests();
    bool matrixVectorPass = matrix_vector_tests();

    return 0;
    
}
