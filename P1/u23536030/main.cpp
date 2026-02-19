#include "Matrix.h"
#include "Vector.h"
#include "Shape.h"
#include "Triangle.h"
#include "Square.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <limits>

bool approxEqual(float a, float b, float epsilon = 1e-5f) {
    return std::abs(a - b) < epsilon;
}

void printTestResult(const std::string& testName, bool passed) {
    std::cout << std::left << std::setw(50) << testName
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
    std::cout << "═══════════════════════════════════════════════\n";
    std::cout << "Vector<" << 3 << "> tests\n";
    std::cout << "═══════════════════════════════════════════════\n\n";
    bool allPassed = true;

    std::cout << "--- Basic Constructors ---\n";

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
        printTestResult("Copy constructor", ok);
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

    // Self-assignment
    {
        Vector<3> v{1, 2, 3};
        v = v;
        bool ok = approxEqual(v[0], 1) && approxEqual(v[1], 2) && approxEqual(v[2], 3);
        printTestResult("Self-assignment (v = v)", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Arithmetic Operations ---\n";

    //operator+ 
    {
        Vector<3> a{1, 2, 3};
        Vector<3> b{4, -1, 5};
        Vector<3> sum = a + b;
        bool ok = approxEqual(sum[0], 5) && approxEqual(sum[1], 1) && approxEqual(sum[2], 8);
        printTestResult("operator+ (addition)", ok);
        allPassed &= ok;
    }

    // Addition with zero vector
    {
        Vector<3> a{5, -3, 2};
        Vector<3> zero{0, 0, 0};
        Vector<3> sum = a + zero;
        bool ok = approxEqual(sum[0], 5) && approxEqual(sum[1], -3) && approxEqual(sum[2], 2);
        printTestResult("Addition with zero vector", ok);
        allPassed &= ok;
    }

    // Addition with negative values
    {
        Vector<3> a{-1, -2, -3};
        Vector<3> b{-4, -5, -6};
        Vector<3> sum = a + b;
        bool ok = approxEqual(sum[0], -5) && approxEqual(sum[1], -7) && approxEqual(sum[2], -9);
        printTestResult("Addition with negative values", ok);
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

    // Subtraction resulting in zero
    {
        Vector<3> a{5, 10, 15};
        Vector<3> b{5, 10, 15};
        Vector<3> diff = a - b;
        bool ok = approxEqual(diff[0], 0) && approxEqual(diff[1], 0) && approxEqual(diff[2], 0);
        printTestResult("Subtraction resulting in zero vector", ok);
        allPassed &= ok;
    }

    // Subtraction with negative result
    {
        Vector<3> a{1, 2, 3};
        Vector<3> b{4, 5, 6};
        Vector<3> diff = a - b;
        bool ok = approxEqual(diff[0], -3) && approxEqual(diff[1], -3) && approxEqual(diff[2], -3);
        printTestResult("Subtraction with negative result", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Scalar Multiplication ---\n";

    //operator* (scalar)
    {
        Vector<3> v{2, -3, 4};
        Vector<3> res = v * 2.5f;
        bool ok = approxEqual(res[0], 5) && approxEqual(res[1], -7.5f) && approxEqual(res[2], 10);
        printTestResult("operator* (scalar multiplication)", ok);
        allPassed &= ok;
    }

    // Scalar multiplication by zero
    {
        Vector<3> v{5, -3, 7};
        Vector<3> res = v * 0.0f;
        bool ok = approxEqual(res[0], 0) && approxEqual(res[1], 0) && approxEqual(res[2], 0);
        printTestResult("Scalar multiplication by zero", ok);
        allPassed &= ok;
    }

    // Scalar multiplication by one
    {
        Vector<3> v{3, -4, 5};
        Vector<3> res = v * 1.0f;
        bool ok = approxEqual(res[0], 3) && approxEqual(res[1], -4) && approxEqual(res[2], 5);
        printTestResult("Scalar multiplication by one", ok);
        allPassed &= ok;
    }

    // Scalar multiplication by negative
    {
        Vector<3> v{2, -3, 4};
        Vector<3> res = v * (-2.0f);
        bool ok = approxEqual(res[0], -4) && approxEqual(res[1], 6) && approxEqual(res[2], -8);
        printTestResult("Scalar multiplication by negative", ok);
        allPassed &= ok;
    }

    // Scalar multiplication by very small number
    {
        Vector<3> v{1000, 2000, 3000};
        Vector<3> res = v * 0.001f;
        bool ok = approxEqual(res[0], 1) && approxEqual(res[1], 2) && approxEqual(res[2], 3);
        printTestResult("Scalar multiplication by small number", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Dot Product ---\n";

    //operator* (dot product)
    {
        Vector<3> a{1, 2, 3};
        Vector<3> b{4, 5, 6};
        float dot = a * b;
        bool ok = approxEqual(dot, 1*4 + 2*5 + 3*6, 1e-4f);
        printTestResult("operator* (dot product)", ok);
        allPassed &= ok;
    }

    // Dot product with zero vector
    {
        Vector<3> a{5, -3, 7};
        Vector<3> zero{0, 0, 0};
        float dot = a * zero;
        bool ok = approxEqual(dot, 0.0f);
        printTestResult("Dot product with zero vector", ok);
        allPassed &= ok;
    }

    // Dot product of orthogonal vectors
    {
        Vector<3> a{1, 0, 0};
        Vector<3> b{0, 1, 0};
        float dot = a * b;
        bool ok = approxEqual(dot, 0.0f);
        printTestResult("Dot product of orthogonal vectors", ok);
        allPassed &= ok;
    }

    // Dot product with itself (magnitude squared)
    {
        Vector<3> v{3, 4, 0};
        float dot = v * v;
        bool ok = approxEqual(dot, 25.0f);  // 3² + 4² = 25
        printTestResult("Dot product with itself (magnitude²)", ok);
        allPassed &= ok;
    }

    // Dot product with negative values
    {
        Vector<3> a{1, -2, 3};
        Vector<3> b{-1, 2, -3};
        float dot = a * b;
        bool ok = approxEqual(dot, -14.0f);  // -1 + -4 + -9 = -14
        printTestResult("Dot product with negative values", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Magnitude ---\n";

    //magnitude()
    {
        Vector<3> v{3, 4, 0};
        float mag = v.magnitude();
        bool ok = approxEqual(mag, 5.0f);
        printTestResult("magnitude() / length", ok);
        allPassed &= ok;
    }

    // Magnitude of zero vector
    {
        Vector<3> v{0, 0, 0};
        float mag = v.magnitude();
        bool ok = approxEqual(mag, 0.0f);
        printTestResult("Magnitude of zero vector", ok);
        allPassed &= ok;
    }

    // Magnitude of unit vector
    {
        Vector<3> v{1, 0, 0};
        float mag = v.magnitude();
        bool ok = approxEqual(mag, 1.0f);
        printTestResult("Magnitude of unit vector", ok);
        allPassed &= ok;
    }

    // Magnitude with negative components
    {
        Vector<3> v{-3, -4, 0};
        float mag = v.magnitude();
        bool ok = approxEqual(mag, 5.0f);
        printTestResult("Magnitude with negative components", ok);
        allPassed &= ok;
    }

    // Magnitude of larger vector
    {
        Vector<3> v{1, 2, 2};
        float mag = v.magnitude();
        bool ok = approxEqual(mag, 3.0f, 1e-5f);  // sqrt(1 + 4 + 4) = 3
        printTestResult("Magnitude (1,2,2)", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Unit Vector / Normalization ---\n";

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

    // Unit vector of already normalized vector
    {
        Vector<3> v{1, 0, 0};
        Vector<3> u = v.unitVector();
        bool ok = approxEqual(u[0], 1.0f) && approxEqual(u[1], 0.0f) && approxEqual(u[2], 0.0f);
        ok &= approxEqual(u.magnitude(), 1.0f);
        printTestResult("Unit vector of already normalized vector", ok);
        allPassed &= ok;
    }

    // Unit vector with negative components
    {
        Vector<3> v{-6, -8, 0};
        Vector<3> u = v.unitVector();
        bool ok = approxEqual(u[0], -0.6f, 1e-5f) && approxEqual(u[1], -0.8f, 1e-5f);
        ok &= approxEqual(u.magnitude(), 1.0f, 1e-5f);
        printTestResult("Unit vector with negative components", ok);
        allPassed &= ok;
    }

    // Unit vector of zero vector (edge case - may cause division by zero)
    {
        Vector<3> v{0, 0, 0};
        bool hasError = false;
        try {
            Vector<3> u = v.unitVector();
            // Check if result contains NaN or Inf
            bool containsInvalid = std::isnan(u[0]) || std::isinf(u[0]) ||
                std::isnan(u[1]) || std::isinf(u[1]) ||
                std::isnan(u[2]) || std::isinf(u[2]);
            hasError = containsInvalid;
        } catch (...) {
            hasError = true;
        }
        printTestResult("Unit vector of zero vector (edge case)", hasError);
        // Note: This test expects the edge case to be handled somehow
        allPassed &= hasError; // This might need adjustment based on implementation
    }

    std::cout << "\n--- Cross Product ---\n";

    //crossProduct
    {
        Vector<3> a{1, 0, 0};
        Vector<3> b{0, 1, 0};
        Vector<3> c = a.crossProduct(b);
        bool ok = approxEqual(c[0], 0) && approxEqual(c[1], 0) && approxEqual(c[2], 1);
        printTestResult("crossProduct (i × j = k)", ok);
        allPassed &= ok;
    }

    // Cross product anti-commutativity (a × b = -(b × a))
    {
        Vector<3> a{1, 0, 0};
        Vector<3> b{0, 1, 0};
        Vector<3> c1 = a.crossProduct(b);
        Vector<3> c2 = b.crossProduct(a);
        bool ok = approxEqual(c1[0], -c2[0]) && approxEqual(c1[1], -c2[1]) && approxEqual(c1[2], -c2[2]);
        printTestResult("Cross product anti-commutativity", ok);
        allPassed &= ok;
    }

    // Cross product of parallel vectors (should be zero)
    {
        Vector<3> a{2, 4, 6};
        Vector<3> b{1, 2, 3};  // b = 0.5 * a
        Vector<3> c = a.crossProduct(b);
        bool ok = approxEqual(c[0], 0, 1e-4f) && approxEqual(c[1], 0, 1e-4f) && approxEqual(c[2], 0, 1e-4f);
        printTestResult("Cross product of parallel vectors", ok);
        allPassed &= ok;
    }

    // Cross product with itself (should be zero)
    {
        Vector<3> a{5, -3, 7};
        Vector<3> c = a.crossProduct(a);
        bool ok = approxEqual(c[0], 0, 1e-4f) && approxEqual(c[1], 0, 1e-4f) && approxEqual(c[2], 0, 1e-4f);
        printTestResult("Cross product with itself", ok);
        allPassed &= ok;
    }

    // Cross product with zero vector
    {
        Vector<3> a{5, -3, 7};
        Vector<3> zero{0, 0, 0};
        Vector<3> c = a.crossProduct(zero);
        bool ok = approxEqual(c[0], 0) && approxEqual(c[1], 0) && approxEqual(c[2], 0);
        printTestResult("Cross product with zero vector", ok);
        allPassed &= ok;
    }

    // Cross product with arbitrary vectors
    {
        Vector<3> a{2, 3, 4};
        Vector<3> b{5, 6, 7};
        Vector<3> c = a.crossProduct(b);
        // Expected: (3*7-4*6, 4*5-2*7, 2*6-3*5) = (-3, 6, -3)
        bool ok = approxEqual(c[0], -3) && approxEqual(c[1], 6) && approxEqual(c[2], -3);
        printTestResult("Cross product (2,3,4) × (5,6,7)", ok);
        allPassed &= ok;
    }

    // Cross product magnitude (|a × b| = |a||b|sin(θ))
    {
        Vector<3> a{1, 0, 0};
        Vector<3> b{0, 1, 0};
        Vector<3> c = a.crossProduct(b);
        float magCross = c.magnitude();
        float magA = a.magnitude();
        float magB = b.magnitude();
        bool ok = approxEqual(magCross, magA * magB, 1e-5f);  // sin(90°) = 1
        printTestResult("Cross product magnitude (perpendicular)", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Array Access ---\n";

    //operator[] const
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

    // Non-const array access
    {
        Vector<3> v{1, 2, 3};
        v[1] = 42.0f;
        bool ok = approxEqual(v[1], 42.0f);
        printTestResult("operator[] non-const modification", ok);
        allPassed &= ok;
    }

    // Array access at boundary (first element)
    {
        Vector<3> v{7, 8, 9};
        bool ok = approxEqual(v[0], 7.0f);
        printTestResult("operator[] at index 0", ok);
        allPassed &= ok;
    }

    // Array access at boundary (last element)
    {
        Vector<3> v{7, 8, 9};
        bool ok = approxEqual(v[2], 9.0f);
        printTestResult("operator[] at last index (n-1)", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Utility Functions ---\n";

    //getN()
    {
        Vector<3> v;
        bool ok = (v.getN() == 3);
        printTestResult("getN()", ok);
        allPassed &= ok;
    }

    // getN() for different sizes
    {
        Vector<1> v1;
        Vector<5> v5;
        Vector<10> v10;
        bool ok = (v1.getN() == 1) && (v5.getN() == 5) && (v10.getN() == 10);
        printTestResult("getN() for various dimensions", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Special Values ---\n";

    // Very large values
    {
        Vector<3> v{1e6f, 1e6f, 1e6f};
        float mag = v.magnitude();
        bool ok = approxEqual(mag, std::sqrt(3.0f) * 1e6f, 1e1f);
        printTestResult("Magnitude with large values", ok);
        allPassed &= ok;
    }

    // Very small values
    {
        Vector<3> v{1e-6f, 1e-6f, 1e-6f};
        float mag = v.magnitude();
        bool ok = approxEqual(mag, std::sqrt(3.0f) * 1e-6f, 1e-11f);
        printTestResult("Magnitude with small values", ok);
        allPassed &= ok;
    }

    // Mixed positive and negative
    {
        Vector<3> a{1, -1, 1};
        Vector<3> b{-1, 1, -1};
        Vector<3> sum = a + b;
        bool ok = approxEqual(sum[0], 0) && approxEqual(sum[1], 0) && approxEqual(sum[2], 0);
        printTestResult("Addition with mixed signs", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Visual Checks ---\n";
    {
        std::cout << "Visual check - print():\n";
        Vector<3> demo{1.5f, -0.75f, 2.0f};
        demo.print();
        std::cout << "(should see 3 numbers above)\n";
    }

    std::cout << "\n═══════════════════════════════════════════════\n";
    std::cout << (allPassed ? "ALL VECTOR TESTS PASSED " : "SOME VECTOR TESTS FAILED ") << "\n";
    std::cout << "═══════════════════════════════════════════════\n\n";

    return allPassed;
}

bool matrix_tests()
{
    std::cout << "═══════════════════════════════════════════════\n";
    std::cout << "Matrix tests (various sizes)\n";
    std::cout << "═══════════════════════════════════════════════\n\n";
    bool allPassed = true;

    std::cout << "--- Testing Matrix<2,3> Basics ---\n";

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

    // Self-assignment
    {
        Matrix<2,3> m;
        m[0][0] = 1; m[0][1] = 2; m[0][2] = 3;
        m[1][0] = 4; m[1][1] = 5; m[1][2] = 6;
        m = m;
        bool ok = approxEqual(m[0][0],1) && approxEqual(m[0][1],2) && approxEqual(m[1][1],5);
        printTestResult("Self-assignment (m = m)", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Arithmetic Operations ---\n";

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

    // Addition with zero matrix
    {
        Matrix<2,3> a;
        a[0][0]=5; a[0][1]=-3; a[0][2]=2;
        a[1][0]=1; a[1][1]=7;  a[1][2]=-4;

        Matrix<2,3> zero;  // default constructor creates zero matrix

        Matrix<2,3> sum = a + zero;

        bool ok = approxEqual(sum[0][0],5) && approxEqual(sum[0][1],-3) && approxEqual(sum[0][2],2) &&
            approxEqual(sum[1][0],1) && approxEqual(sum[1][1],7)  && approxEqual(sum[1][2],-4);
        printTestResult("Addition with zero matrix", ok);
        allPassed &= ok;
    }

    // Addition with negative values
    {
        Matrix<2,3> a, b;
        a[0][0]=-1; a[0][1]=-2; a[0][2]=-3;
        a[1][0]=-4; a[1][1]=-5; a[1][2]=-6;

        b[0][0]=-7; b[0][1]=-8; b[0][2]=-9;
        b[1][0]=-10;b[1][1]=-11;b[1][2]=-12;

        Matrix<2,3> sum = a + b;

        bool ok = approxEqual(sum[0][0],-8) && approxEqual(sum[0][1],-10) && approxEqual(sum[0][2],-12) &&
            approxEqual(sum[1][0],-14) && approxEqual(sum[1][1],-16) && approxEqual(sum[1][2],-18);
        printTestResult("Addition with all negative values", ok);
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

    // Scalar multiplication by zero
    {
        Matrix<2,3> m;
        m[0][0]=5; m[0][1]=-3; m[0][2]=7;
        m[1][0]=2; m[1][1]=8;  m[1][2]=-1;

        Matrix<2,3> res = m * 0.0f;

        bool ok = true;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 3; ++j)
                if (!approxEqual(res[i][j], 0.0f)) ok = false;
        printTestResult("Scalar multiplication by zero", ok);
        allPassed &= ok;
    }

    // Scalar multiplication by one
    {
        Matrix<2,3> m;
        m[0][0]=3; m[0][1]=-4; m[0][2]=5;
        m[1][0]=6; m[1][1]=7;  m[1][2]=-8;

        Matrix<2,3> res = m * 1.0f;

        bool ok = approxEqual(res[0][0],3) && approxEqual(res[0][1],-4) && approxEqual(res[0][2],5) &&
            approxEqual(res[1][0],6) && approxEqual(res[1][1],7)  && approxEqual(res[1][2],-8);
        printTestResult("Scalar multiplication by one", ok);
        allPassed &= ok;
    }

    // Scalar multiplication by negative
    {
        Matrix<2,3> m;
        m[0][0]=2; m[0][1]=-3; m[0][2]=4;
        m[1][0]=1; m[1][1]=5;  m[1][2]=-2;

        Matrix<2,3> res = m * (-1.0f);

        bool ok = approxEqual(res[0][0],-2) && approxEqual(res[0][1],3) && approxEqual(res[0][2],-4) &&
            approxEqual(res[1][0],-1) && approxEqual(res[1][1],-5) && approxEqual(res[1][2],2);
        printTestResult("Scalar multiplication by negative", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Matrix Multiplication ---\n";

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

    // Matrix multiplication with identity (square)
    {
        Matrix<3,3> a;
        a[0][0]=2; a[0][1]=3; a[0][2]=4;
        a[1][0]=5; a[1][1]=6; a[1][2]=7;
        a[2][0]=8; a[2][1]=9; a[2][2]=10;

        Matrix<3,3> identity;
        identity[0][0]=1; identity[0][1]=0; identity[0][2]=0;
        identity[1][0]=0; identity[1][1]=1; identity[1][2]=0;
        identity[2][0]=0; identity[2][1]=0; identity[2][2]=1;

        Matrix<3,3> result = a * identity;

        bool ok = approxEqual(result[0][0],2) && approxEqual(result[0][1],3) && approxEqual(result[0][2],4) &&
            approxEqual(result[1][0],5) && approxEqual(result[1][1],6) && approxEqual(result[1][2],7) &&
            approxEqual(result[2][0],8) && approxEqual(result[2][1],9) && approxEqual(result[2][2],10);
        printTestResult("Matrix multiplication with identity", ok);
        allPassed &= ok;
    }

    // Matrix multiplication with zero matrix
    {
        Matrix<2,3> a;
        a[0][0]=1; a[0][1]=2; a[0][2]=3;
        a[1][0]=4; a[1][1]=5; a[1][2]=6;

        Matrix<3,2> zero;  // zero matrix

        Matrix<2,2> result = a * zero;

        bool ok = true;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                if (!approxEqual(result[i][j], 0.0f)) ok = false;
        printTestResult("Matrix multiplication with zero matrix", ok);
        allPassed &= ok;
    }

    // Square matrix multiplication (2×2 × 2×2)
    {
        Matrix<2,2> a, b;
        a[0][0]=1; a[0][1]=2;
        a[1][0]=3; a[1][1]=4;

        b[0][0]=5; b[0][1]=6;
        b[1][0]=7; b[1][1]=8;

        Matrix<2,2> c = a * b;

        // Expected: [[19, 22], [43, 50]]
        bool ok = approxEqual(c[0][0],19) && approxEqual(c[0][1],22) &&
            approxEqual(c[1][0],43) && approxEqual(c[1][1],50);
        printTestResult("Square matrix multiplication 2×2", ok);
        allPassed &= ok;
    }

    // Matrix multiplication non-commutativity (A*B != B*A)
    {
        Matrix<2,2> a, b;
        a[0][0]=1; a[0][1]=2;
        a[1][0]=3; a[1][1]=4;

        b[0][0]=5; b[0][1]=6;
        b[1][0]=7; b[1][1]=8;

        Matrix<2,2> ab = a * b;
        Matrix<2,2> ba = b * a;

        bool ok = !approxEqual(ab[0][0], ba[0][0]) || !approxEqual(ab[0][1], ba[0][1]) ||
            !approxEqual(ab[1][0], ba[1][0]) || !approxEqual(ab[1][1], ba[1][1]);
        printTestResult("Matrix multiplication non-commutativity", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Transpose ---\n";

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

    // Transpose of square matrix
    {
        Matrix<3,3> a;
        a[0][0]=1; a[0][1]=2; a[0][2]=3;
        a[1][0]=4; a[1][1]=5; a[1][2]=6;
        a[2][0]=7; a[2][1]=8; a[2][2]=9;

        Matrix<3,3> t = ~a;

        bool ok = approxEqual(t[0][0],1) && approxEqual(t[0][1],4) && approxEqual(t[0][2],7) &&
            approxEqual(t[1][0],2) && approxEqual(t[1][1],5) && approxEqual(t[1][2],8) &&
            approxEqual(t[2][0],3) && approxEqual(t[2][1],6) && approxEqual(t[2][2],9);
        printTestResult("Transpose of square matrix", ok);
        allPassed &= ok;
    }

    // Double transpose returns original
    {
        Matrix<2,3> a;
        a[0][0]=1; a[0][1]=2; a[0][2]=3;
        a[1][0]=4; a[1][1]=5; a[1][2]=6;

        Matrix<2,3> tt = ~(~a);

        bool ok = approxEqual(tt[0][0],1) && approxEqual(tt[0][1],2) && approxEqual(tt[0][2],3) &&
            approxEqual(tt[1][0],4) && approxEqual(tt[1][1],5) && approxEqual(tt[1][2],6);
        printTestResult("Double transpose returns original", ok);
        allPassed &= ok;
    }

    // Transpose of 1×n and n×1 matrices
    {
        Matrix<1,4> row;
        row[0][0]=1; row[0][1]=2; row[0][2]=3; row[0][3]=4;

        Matrix<4,1> col = ~row;

        bool ok = approxEqual(col[0][0],1) && approxEqual(col[1][0],2) &&
            approxEqual(col[2][0],3) && approxEqual(col[3][0],4);
        printTestResult("Transpose of row to column vector", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Determinant (Square Matrices) ---\n";

    // 1×1
    {
        Matrix<1,1> m;
        m[0][0] = 7.0f;
        float det = m.determinant();
        bool ok = approxEqual(det, 7.0f);
        printTestResult("determinant 1×1", ok);
        allPassed &= ok;
    }

    // 1×1 negative
    {
        Matrix<1,1> m;
        m[0][0] = -5.0f;
        float det = m.determinant();
        bool ok = approxEqual(det, -5.0f);
        printTestResult("determinant 1×1 (negative)", ok);
        allPassed &= ok;
    }

    // 1×1 zero
    {
        Matrix<1,1> m;
        m[0][0] = 0.0f;
        float det = m.determinant();
        bool ok = approxEqual(det, 0.0f);
        printTestResult("determinant 1×1 (zero)", ok);
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

    // 2×2 identity
    {
        Matrix<2,2> m;
        m[0][0] = 1; m[0][1] = 0;
        m[1][0] = 0; m[1][1] = 1;

        float det = m.determinant();
        bool ok = approxEqual(det, 1.0f);
        printTestResult("determinant 2×2 identity", ok);
        allPassed &= ok;
    }

    // 2×2 zero (singular)
    {
        Matrix<2,2> m;
        m[0][0] = 1; m[0][1] = 2;
        m[1][0] = 2; m[1][1] = 4;  // rows are linearly dependent

        float det = m.determinant();
        bool ok = approxEqual(det, 0.0f, 1e-4f);
        printTestResult("determinant 2×2 singular", ok);
        allPassed &= ok;
    }

    // 2×2 negative determinant
    {
        Matrix<2,2> m;
        m[0][0] = 1; m[0][1] = 2;
        m[1][0] = 3; m[1][1] = 4;

        float det = m.determinant();  // 1*4 - 2*3 = -2
        bool ok = approxEqual(det, -2.0f);
        printTestResult("determinant 2×2 negative", ok);
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

    // 3×3 identity
    {
        Matrix<3,3> m;
        m[0][0]=1; m[0][1]=0; m[0][2]=0;
        m[1][0]=0; m[1][1]=1; m[1][2]=0;
        m[2][0]=0; m[2][1]=0; m[2][2]=1;

        float det = m.determinant();
        bool ok = approxEqual(det, 1.0f);
        printTestResult("determinant 3×3 identity", ok);
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

    // 3×3 with known determinant
    {
        Matrix<3,3> m;
        m[0][0]=6; m[0][1]=1; m[0][2]=1;
        m[1][0]=4; m[1][1]=-2; m[1][2]=5;
        m[2][0]=2; m[2][1]=8; m[2][2]=7;

        float det = m.determinant();  // expected = -306
        bool ok = approxEqual(det, -306.0f, 1e-3f);
        printTestResult("determinant 3×3 (det = -306)", ok);
        allPassed &= ok;
    }

    // 3×3 diagonal matrix
    {
        Matrix<3,3> m;
        m[0][0]=2; m[0][1]=0; m[0][2]=0;
        m[1][0]=0; m[1][1]=3; m[1][2]=0;
        m[2][0]=0; m[2][1]=0; m[2][2]=5;

        float det = m.determinant();  // 2*3*5 = 30
        bool ok = approxEqual(det, 30.0f);
        printTestResult("determinant 3×3 diagonal", ok);
        allPassed &= ok;
    }

    // 4×4 identity
    {
        Matrix<4,4> m;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = (i == j) ? 1.0f : 0.0f;

        float det = m.determinant();
        bool ok = approxEqual(det, 1.0f, 1e-4f);
        printTestResult("determinant 4×4 identity", ok);
        allPassed &= ok;
    }

    // 4×4 zero
    {
        Matrix<4,4> m;  // all zeros
        float det = m.determinant();
        bool ok = approxEqual(det, 0.0f);
        printTestResult("determinant 4×4 zero matrix", ok);
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

    // Another non-square case
    {
        Matrix<3,2> nonsq;
        nonsq[0][0]=1; nonsq[0][1]=2;
        nonsq[1][0]=3; nonsq[1][1]=4;
        nonsq[2][0]=5; nonsq[2][1]=6;
        float det = nonsq.determinant();
        bool ok = approxEqual(det, 0.0f);
        printTestResult("determinant 3×2 non-square → 0", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Edge Cases ---\n";

    // Very small matrix (1×1)
    {
        Matrix<1,1> m;
        m[0][0] = 42.0f;
        bool ok = approxEqual(m[0][0], 42.0f);
        printTestResult("1×1 matrix creation and access", ok);
        allPassed &= ok;
    }

    // Large matrix dimensions
    {
        Matrix<10,10> large;
        large[5][5] = 99.0f;
        bool ok = approxEqual(large[5][5], 99.0f);
        printTestResult("10×10 matrix creation and access", ok);
        allPassed &= ok;
    }

    // Rectangular extreme (1×10)
    {
        Matrix<1,10> wide;
        for (int i = 0; i < 10; ++i)
            wide[0][i] = static_cast<float>(i);
        bool ok = approxEqual(wide[0][0], 0.0f) && approxEqual(wide[0][9], 9.0f);
        printTestResult("1×10 wide matrix", ok);
        allPassed &= ok;
    }

    // Rectangular extreme (10×1)
    {
        Matrix<10,1> tall;
        for (int i = 0; i < 10; ++i)
            tall[i][0] = static_cast<float>(i * 2);
        bool ok = approxEqual(tall[0][0], 0.0f) && approxEqual(tall[9][0], 18.0f);
        printTestResult("10×1 tall matrix", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Visual Checks ---\n";
    {
        std::cout << "Visual check - print() 3×3:\n";
        Matrix<3,3> demo;
        demo[0][0]=1.5f; demo[0][1]=-0.2f; demo[0][2]=3.7f;
        demo[1][0]=0.0f; demo[1][1]=4.1f;  demo[1][2]=-2.9f;
        demo[2][0]=5.6f; demo[2][1]=1.0f;  demo[2][2]=0.0f;
        demo.print();
        std::cout << "(should see 3×3 matrix above)\n";
    }

    std::cout << "\n═══════════════════════════════════════════════\n";
    std::cout << (allPassed ? "ALL MATRIX TESTS PASSED " : "SOME MATRIX TESTS FAILED ") << "\n";
    std::cout << "═══════════════════════════════════════════════\n\n";

    return allPassed;
}

bool square_tests()
{
    std::cout << "═══════════════════════════════════════════════\n";
    std::cout << "Square tests (2D and 3D homogeneous)\n";
    std::cout << "═══════════════════════════════════════════════\n\n";
    bool allPassed = true;

    auto approxEqual = [](float a, float b, float eps = 1e-5f) {
        return std::abs(a - b) < eps;
    };

    auto pointsApproxEqual = [&](const Vector<2>& a, const Vector<2>& b) {
        return approxEqual(a[0], b[0]) && approxEqual(a[1], b[1]);
    };

    auto pointsApproxEqual3 = [&](const Vector<3>& a, const Vector<3>& b) {
        return approxEqual(a[0], b[0]) && approxEqual(a[1], b[1]) && approxEqual(a[2], b[2]);
    };

    std::cout << "--- Square<2> basic construction & access ---\n";

    // Default-like construction via center + size (assuming it exists)
    {
        Vector<2> center{4.0f, 5.0f};
        Square<2> sq(center, 6.0f, 8.0f);  // height=6, width=8
        // expected
        // tl = (0,8)
        // tr = (8,8)
        // bl = (0,2)
        // br = (8,2)

        // Expected corners (assuming center-based constructor)
        Vector<2> exp_tl{0.0f, 8.0f};   // x-4, y+3
        Vector<2> exp_tr{8.0f, 8.0f};
        Vector<2> exp_br{8.0f, 2.0f};
        Vector<2> exp_bl{0.0f, 2.0f};

        /*
        std::cout << "Center: " << std::endl;
        center.print();
        std::cout << "Square: " << std::endl;
        sq.print();
        std::cout << "Expected: " << std::endl;
        exp_tl.print();
        exp_tr.print();
        exp_bl.print();
        exp_br.print();
        */

        float* pts = sq.getPoints();
        bool ok = 
            approxEqual(pts[0],  exp_tl[0]) && approxEqual(pts[1],  exp_tl[1]) &&
            approxEqual(pts[2],  exp_tr[0]) && approxEqual(pts[3],  exp_tr[1]) &&
            approxEqual(pts[4],  exp_br[0]) && approxEqual(pts[5],  exp_br[1]) &&
            approxEqual(pts[6],  exp_bl[0]) && approxEqual(pts[7],  exp_bl[1]);

        printTestResult("Square<2>(center, height, width) – checking via getPoints()", ok);
        allPassed &= ok;

    }

    // Four-points constructor
    {
        Vector<2> p1{0, 3};
        Vector<2> p2{4, 3};
        Vector<2> p3{4, 0};
        Vector<2> p4{0, 0};

        Square<2> sq(p1, p2, p3, p4);

        float* pts = sq.getPoints();
        bool ok = 
            approxEqual(pts[0],  p1[0]) && approxEqual(pts[1],  p1[1]) &&
            approxEqual(pts[2],  p2[0]) && approxEqual(pts[3],  p2[1]) &&
            approxEqual(pts[4],  p3[0]) && approxEqual(pts[5],  p3[1]) &&
            approxEqual(pts[6],  p4[0]) && approxEqual(pts[7],  p4[1]);

        printTestResult("Square<2> four-points constructor", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Square<2> transformation (*=) ---\n";

    // Simple translation
    {
        Square<2> sq(
                Vector<2>{0, 2},
                Vector<2>{4, 2},
                Vector<2>{4, 0},
                Vector<2>{0, 0}
                );

        // Translation matrix (move +3 right, +1 up)
        float** trans_data = new float*[2];
        for (int i = 0; i < 2; ++i) {
            trans_data[i] = new float[2];
        }

        trans_data[0][0] = 1.0f; trans_data[0][1] = 0.0f;
        trans_data[1][0] = 0.0f; trans_data[1][1] = 1.0f;

        Matrix<2,2> identity(trans_data);
        // Note: for translation in 2D affine you normally need 3×3 matrix

        // For real translation test → better use n=3
        float** scale_data= new float*[2];
        for (int i = 0; i < 2; ++i) {
            scale_data[i] = new float[2];
        }

        scale_data[0][0] = 2.0f; scale_data[0][1] = 0.0f;
        scale_data[1][0] = 0.0f; scale_data[1][1] = 1.5f;
        Matrix<2,2> scale(scale_data);

        sq *= scale;

        float* pts = sq.getPoints();

        Vector<2> got_tl { pts[0], pts[1] };
        Vector<2> got_tr { pts[2], pts[3] };
        Vector<2> got_br { pts[4], pts[5] };
        Vector<2> got_bl { pts[6], pts[7] };

        bool ok = 
            pointsApproxEqual(got_tl, {0.0f, 3.0f}) &&
            pointsApproxEqual(got_tr, {8.0f, 3.0f}) &&
            pointsApproxEqual(got_br, {8.0f, 0.0f}) &&
            pointsApproxEqual(got_bl, {0.0f, 0.0f});


        printTestResult("Square<2> *= uniform-ish scale (2×, 1.5×)", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Square<3> (homogeneous) transformation ---\n";
    // Square in homogeneous coordinates (z=1) - translation
    {
        Vector<3> tl_h{1, 4, 1};
        Vector<3> tr_h{5, 4, 1};
        Vector<3> br_h{5, 1, 1};
        Vector<3> bl_h{1, 1, 1};

        Square<3> sq(tl_h, tr_h, br_h, bl_h);

        // Translation: move +2 in x, +3 in y
        float** tmat = new float*[3];
        tmat[0] = new float[3]{1, 0, 2};
        tmat[1] = new float[3]{0, 1, 3};
        tmat[2] = new float[3]{0, 0, 1};

        Matrix<3,3> translate(tmat);

        sq *= translate;

        float* pts = sq.getPoints();

        bool ok =
            // tl → expected {3, 7, 1}
            approxEqual(pts[0],  3.0f) && approxEqual(pts[1],  7.0f) && approxEqual(pts[2],  1.0f) &&
            // tr → {7, 7, 1}
            approxEqual(pts[3],  7.0f) && approxEqual(pts[4],  7.0f) && approxEqual(pts[5],  1.0f) &&
            // br → {7, 4, 1}
            approxEqual(pts[6],  7.0f) && approxEqual(pts[7],  4.0f) && approxEqual(pts[8],  1.0f) &&
            // bl → {3, 4, 1}
            approxEqual(pts[9],  3.0f) && approxEqual(pts[10], 4.0f) && approxEqual(pts[11], 1.0f);

        printTestResult("Square<3> *= translation (homogeneous) via getPoints()", ok);
        allPassed &= ok;

    }

    // Rotation test (90° CCW around origin)
    {
        Vector<3> tl_h{1, 1, 1};
        Vector<3> tr_h{3, 1, 1};
        Vector<3> br_h{3, -1, 1};
        Vector<3> bl_h{1, -1, 1};

        Square<3> sq(tl_h, tr_h, br_h, bl_h);

        // 90° CCW rotation matrix
        float** rmat = new float*[3];
        rmat[0] = new float[3]{ 0, -1, 0 };
        rmat[1] = new float[3]{ 1,  0, 0 };
        rmat[2] = new float[3]{ 0,  0, 1 };

        Matrix<3,3> rot(rmat);

        sq *= rot;

        float* pts = sq.getPoints();

        bool ok =
            // tl → expected {-1, 1, 1}
            approxEqual(pts[0], -1.0f) && approxEqual(pts[1],  1.0f) && approxEqual(pts[2],  1.0f) &&
            // tr → {-1, 3, 1}
            approxEqual(pts[3], -1.0f) && approxEqual(pts[4],  3.0f) && approxEqual(pts[5],  1.0f) &&
            // br → {1, 3, 1}
            approxEqual(pts[6],  1.0f) && approxEqual(pts[7],  3.0f) && approxEqual(pts[8],  1.0f) &&
            // bl → {1, 1, 1}
            approxEqual(pts[9],  1.0f) && approxEqual(pts[10], 1.0f) && approxEqual(pts[11], 1.0f);

        printTestResult("Square<3> *= 90° rotation CCW via getPoints()", ok);
        allPassed &= ok;

    }


    std::cout << "\n";
    printTestResult("Square overall result", allPassed);
    return allPassed;
}
bool triangle_tests()
{
    std::cout << "═══════════════════════════════════════════════\n";
    std::cout << "Triangle tests – trying to cover all public interface\n";
    std::cout << "═══════════════════════════════════════════════\n\n";
    bool allPassed = true;

    auto approxEqual = [](float a, float b, float eps = 1e-5f) {
        return std::abs(a - b) < eps;
    };

    auto vec2eq = [&](const Vector<2>& a, const Vector<2>& b) {
        return approxEqual(a[0], b[0]) && approxEqual(a[1], b[1]);
    };

    auto vec3eq = [&](const Vector<3>& a, const Vector<3>& b) {
        return approxEqual(a[0], b[0]) && approxEqual(a[1], b[1]) && approxEqual(a[2], b[2]);
    };

    std::cout << "--- Triangle<2> construction variants & getPoints() ---\n";

    // 1. Normal constructor + getPoints()
    {
        Vector<2> p1{1.0f, 3.0f};
        Vector<2> p2{5.0f, 2.0f};
        Vector<2> p3{2.0f, 7.0f};
        Triangle<2> tri(p1, p2, p3);

        float* pts = tri.getPoints();
        bool ok = 
            vec2eq({pts[0], pts[1]}, p1) &&
            vec2eq({pts[2], pts[3]}, p2) &&
            vec2eq({pts[4], pts[5]}, p3);

        printTestResult("Triangle<2> (p1,p2,p3) → getPoints() order correct", ok);
        allPassed &= ok;
    }

    // 2. Copy constructor
    {
        Vector<2> a{0,0}, b{4,0}, c{2,5};
        Triangle<2> original(a, b, c);
        Triangle<2> copy(original);

        float* pts = copy.getPoints();
        bool ok = 
            approxEqual(pts[0], 0.0f) && approxEqual(pts[1], 0.0f) &&
            approxEqual(pts[2], 4.0f) && approxEqual(pts[3], 0.0f) &&
            approxEqual(pts[4], 2.0f) && approxEqual(pts[5], 5.0f);

        printTestResult("Triangle<2> copy constructor preserves points", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Triangle<2> operator*= (in-place transform) ---\n";

    {
        Vector<2> a{1,1}, b{5,1}, c{3,4};
        Triangle<2> tri(a, b, c);

                                                  //
        float** m = new float*[2];
        m[0] = new float[3]{0.0f, -1.0f};
        m[1] = new float[3]{1.0f, 0.0f};
        Matrix<2,2> rot(m);

        tri *= rot;

        float* pts = tri.getPoints();
        bool ok =
            vec2eq({pts[0], pts[1]}, {-1, 1}) &&
            vec2eq({pts[2], pts[3]}, {-1, 5}) &&
            vec2eq({pts[4], pts[5]}, {-4, 3});

        printTestResult("Triangle<2> *= 90° CCW rotation", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Triangle<3> (homogeneous) – construction & getPoints ---\n";

    {
        Vector<3> p1{2,1,1}, p2{6,1,1}, p3{4,5,1};
        Triangle<3> tri(p1, p2, p3);

        float* pts = tri.getPoints();
        bool ok = 
            vec3eq({pts[0],pts[1],pts[2]}, p1) &&
            vec3eq({pts[3],pts[4],pts[5]}, p2) &&
            vec3eq({pts[6],pts[7],pts[8]}, p3);

        printTestResult("Triangle<3> constructor + getPoints()", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Triangle<3> operator*= (affine transforms) ---\n";

    // Translation
    {
        Vector<3> a{0,0,1}, b{4,0,1}, c{2,3,1};
        Triangle<3> tri(a, b, c);


        float** m = new float*[3];
        m[0] = new float[3]{1.0f, 0.0f, 5.0f};
        m[1] = new float[3]{0.0f, 1.0f,  2.0f};
        m[2] = new float[3]{0.0f, 0.0f,  1.0f};
        Matrix<3,3> trans(m);

        tri *= trans;

        float* pts = tri.getPoints();
        bool ok =
            vec3eq({pts[0],pts[1],pts[2]}, {5,2,1}) &&
            vec3eq({pts[3],pts[4],pts[5]}, {9,2,1}) &&
            vec3eq({pts[6],pts[7],pts[8]}, {7,5,1});

        printTestResult("Triangle<3> *= translation", ok);
        allPassed &= ok;
    }

    // Scale + rotation combo
    {
        Vector<3> a{1,0,1}, b{3,0,1}, c{2,2,1};
        Triangle<3> tri(a, b, c);


        float** m = new float*[3];
        m[0] = new float[3]{0.0f, 2.0f, 0.0f};
        m[1] = new float[3]{2.0f, 0.0f,  0.0f};
        m[2] = new float[3]{0.0f, 0.0f,  1.0f};
        Matrix<3,3> xf(m);

        tri *= xf;

        float* pts = tri.getPoints();
        bool ok =
            vec3eq({pts[0],pts[1],pts[2]}, { 0, 2,1}) &&
            vec3eq({pts[3],pts[4],pts[5]}, { 0, 6,1}) &&
            vec3eq({pts[6],pts[7],pts[8]}, {4, 4,1});

        printTestResult("Triangle<3> *= rot90° + scale×2", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Triangle<n> operator* (new object) ---\n";

    {
        Vector<3> p1{1,1,1}, p2{5,1,1}, p3{3,4,1};
        Triangle<3> tri(p1, p2, p3);

        float** m = new float*[3];
        m[0] = new float[3]{1.0f, 0.0f, 10.0f};
        m[1] = new float[3]{0.0f, 1.0f,  5.0f};
        m[2] = new float[3]{0.0f, 0.0f,  1.0f};
        Matrix<3,3> shift(m);

        Triangle<3>* shifted = tri * shift;   // calls operator*

        float* pts = shifted->getPoints();
        bool ok =
            vec3eq({pts[0],pts[1],pts[2]}, {11,6,1}) &&
            vec3eq({pts[3],pts[4],pts[5]}, {15,6,1}) &&
            vec3eq({pts[6],pts[7],pts[8]}, {13,9,1});

        printTestResult("Triangle<3> * Matrix → new translated triangle", ok);
        allPassed &= ok;

        delete shifted;
    }

    std::cout << "\n--- getNumPoints() consistency ---\n";

    {
        Triangle<2> t2({0,0},{1,0},{0,1});
        Triangle<3> t3({0,0,1},{1,0,1},{0,1,1});

        bool ok = (t2.getNumPoints() == 3) && (t3.getNumPoints() == 3);

        printTestResult("getNumPoints() returns 3 for both <2> and <3>", ok);
        allPassed &= ok;
    }

    std::cout << "\n";
    printTestResult("ALL Triangle tests summary", allPassed);
    return allPassed;
}

bool matrix_vector_tests()
{
    std::cout << "═══════════════════════════════════════════════\n";
    std::cout << "Matrix × Vector interaction tests\n";
    std::cout << "═══════════════════════════════════════════════\n\n";
    bool allPassed = true;

    // ========== BASIC MATRIX-VECTOR MULTIPLICATION ==========
    std::cout << "--- Basic Matrix-Vector Multiplication ---\n";

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

        bool ok = approxEqual(res[0][0], 8.5f, 1e-4f) &&
            approxEqual(res[1][0], 18.5f, 1e-4f);

        printTestResult("Matrix<2,4> × Vector<4>", ok);
        allPassed &= ok;
    }

    // Identity matrix × vector
    {
        Matrix<3,3> I;
        I[0][0] = 1; I[0][1] = 0; I[0][2] = 0;
        I[1][0] = 0; I[1][1] = 1; I[1][2] = 0;
        I[2][0] = 0; I[2][1] = 0; I[2][2] = 1;

        Vector<3> v{7.0f, -3.0f, 5.0f};

        Matrix<3,1> result = I * to_column_vector(v);

        bool ok = approxEqual(result[0][0], 7.0f) &&
            approxEqual(result[1][0], -3.0f) &&
            approxEqual(result[2][0], 5.0f);

        printTestResult("Identity matrix × vector", ok);
        allPassed &= ok;
    }

    // Zero matrix × vector
    {
        Matrix<3,3> zero;  // default is zero

        Vector<3> v{7.0f, -3.0f, 5.0f};

        Matrix<3,1> result = zero * to_column_vector(v);

        bool ok = approxEqual(result[0][0], 0.0f) &&
            approxEqual(result[1][0], 0.0f) &&
            approxEqual(result[2][0], 0.0f);

        printTestResult("Zero matrix × vector", ok);
        allPassed &= ok;
    }

    // Matrix × zero vector
    {
        Matrix<3,3> M;
        M[0][0] = 2; M[0][1] = 3; M[0][2] = 4;
        M[1][0] = 5; M[1][1] = 6; M[1][2] = 7;
        M[2][0] = 8; M[2][1] = 9; M[2][2] = 10;

        Vector<3> zero{0.0f, 0.0f, 0.0f};

        Matrix<3,1> result = M * to_column_vector(zero);

        bool ok = approxEqual(result[0][0], 0.0f) &&
            approxEqual(result[1][0], 0.0f) &&
            approxEqual(result[2][0], 0.0f);

        printTestResult("Matrix × zero vector", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Rotation and Transformation ---\n";

    // 2D rotation matrix (90 degrees)
    {
        Matrix<2,2> rot90;
        rot90[0][0] = 0; rot90[0][1] = -1;
        rot90[1][0] = 1; rot90[1][1] = 0;

        Vector<2> v{1.0f, 0.0f};

        Matrix<2,1> result = rot90 * to_column_vector(v);

        bool ok = approxEqual(result[0][0], 0.0f, 1e-5f) &&
            approxEqual(result[1][0], 1.0f, 1e-5f);

        printTestResult("2D rotation 90° (1,0) → (0,1)", ok);
        allPassed &= ok;
    }

    // Scaling transformation
    {
        Matrix<3,3> scale;
        scale[0][0] = 2; scale[0][1] = 0; scale[0][2] = 0;
        scale[1][0] = 0; scale[1][1] = 3; scale[1][2] = 0;
        scale[2][0] = 0; scale[2][1] = 0; scale[2][2] = 4;

        Vector<3> v{1.0f, 1.0f, 1.0f};

        Matrix<3,1> result = scale * to_column_vector(v);

        bool ok = approxEqual(result[0][0], 2.0f) &&
            approxEqual(result[1][0], 3.0f) &&
            approxEqual(result[2][0], 4.0f);

        printTestResult("Scaling transformation", ok);
        allPassed &= ok;
    }

    // Reflection matrix
    {
        Matrix<2,2> reflect;
        reflect[0][0] = 1; reflect[0][1] = 0;
        reflect[1][0] = 0; reflect[1][1] = -1;  // reflect across x-axis

        Vector<2> v{3.0f, 4.0f};

        Matrix<2,1> result = reflect * to_column_vector(v);

        bool ok = approxEqual(result[0][0], 3.0f) &&
            approxEqual(result[1][0], -4.0f);

        printTestResult("Reflection across x-axis", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Edge Cases ---\n";

    // 1×1 matrix × 1D vector
    {
        Matrix<1,1> m;
        m[0][0] = 5.0f;

        Vector<1> v{3.0f};

        Matrix<1,1> result = m * to_column_vector(v);

        bool ok = approxEqual(result[0][0], 15.0f);

        printTestResult("1×1 matrix × 1D vector", ok);
        allPassed &= ok;
    }

    // Tall matrix × vector
    {
        Matrix<5,2> tall;
        for (int i = 0; i < 5; ++i) {
            tall[i][0] = static_cast<float>(i + 1);
            tall[i][1] = static_cast<float>(i + 1);
        }

        Vector<2> v{2.0f, 3.0f};

        Matrix<5,1> result = tall * to_column_vector(v);

        // Row i: (i+1)*2 + (i+1)*3 = (i+1)*5
        bool ok = approxEqual(result[0][0], 5.0f) &&
            approxEqual(result[1][0], 10.0f) &&
            approxEqual(result[2][0], 15.0f) &&
            approxEqual(result[3][0], 20.0f) &&
            approxEqual(result[4][0], 25.0f);

        printTestResult("5×2 matrix × 2D vector", ok);
        allPassed &= ok;
    }

    // Matrix with negative values
    {
        Matrix<3,3> M;
        M[0][0] = -1; M[0][1] = -2; M[0][2] = -3;
        M[1][0] = -4; M[1][1] = -5; M[1][2] = -6;
        M[2][0] = -7; M[2][1] = -8; M[2][2] = -9;

        Vector<3> v{1.0f, 1.0f, 1.0f};

        Matrix<3,1> result = M * to_column_vector(v);

        bool ok = approxEqual(result[0][0], -6.0f) &&
            approxEqual(result[1][0], -15.0f) &&
            approxEqual(result[2][0], -24.0f);

        printTestResult("Matrix with negative values × vector", ok);
        allPassed &= ok;
    }

    // Mixed positive and negative
    {
        Matrix<2,3> M;
        M[0][0] = 1;  M[0][1] = -2; M[0][2] = 3;
        M[1][0] = -4; M[1][1] = 5;  M[1][2] = -6;

        Vector<3> v{2.0f, -1.0f, 3.0f};

        Matrix<2,1> result = M * to_column_vector(v);

        // Row 0: 1*2 + -2*-1 + 3*3 = 2 + 2 + 9 = 13
        // Row 1: -4*2 + 5*-1 + -6*3 = -8 - 5 - 18 = -31
        bool ok = approxEqual(result[0][0], 13.0f) &&
            approxEqual(result[1][0], -31.0f);

        printTestResult("Matrix × vector with mixed signs", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Chained Operations ---\n";

    // (M1 × M2) × v should equal M1 × (M2 × v)
    {
        Matrix<2,2> M1;
        M1[0][0] = 1; M1[0][1] = 2;
        M1[1][0] = 3; M1[1][1] = 4;

        Matrix<2,2> M2;
        M2[0][0] = 5; M2[0][1] = 6;
        M2[1][0] = 7; M2[1][1] = 8;

        Vector<2> v{1.0f, 2.0f};

        Matrix<2,2> M1M2 = M1 * M2;
        Matrix<2,1> result1 = M1M2 * to_column_vector(v);

        Matrix<2,1> M2v = M2 * to_column_vector(v);
        // Convert M2v back to vector for M1 multiplication
        Vector<2> M2v_vec{M2v[0][0], M2v[1][0]};
        Matrix<2,1> result2 = M1 * to_column_vector(M2v_vec);

        bool ok = approxEqual(result1[0][0], result2[0][0], 1e-4f) &&
            approxEqual(result1[1][0], result2[1][0], 1e-4f);

        printTestResult("Associativity: (M1×M2)×v = M1×(M2×v)", ok);
        allPassed &= ok;
    }

    std::cout << "\n--- Visual Checks ---\n";
    {
        std::cout << "Visual check - Matrix<3,3> × Vector<3>:\n";
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

    std::cout << "\n═══════════════════════════════════════════════\n";
    std::cout << (allPassed ? "ALL MATRIX-VECTOR TESTS PASSED " : "SOME MATRIX-VECTOR TESTS FAILED ") << "\n";
    std::cout << "═══════════════════════════════════════════════\n\n";

    return allPassed;
}

int main(int argc, char const *argv[])
{
    std::cout << "\n";
    std::cout << "║   COMPREHENSIVE  SUITE      ║\n";
    std::cout << "\n";

    bool vectorPass = vector_tests();
    std::cout << "\n";

    bool matrixPass = matrix_tests();
    std::cout << "\n";

    bool matrixVectorPass = matrix_vector_tests();
    std::cout << "\n";

    bool squarePass = square_tests();
    std::cout << "\n";

    bool trianglePass = triangle_tests();
    std::cout << "\n";

    std::cout << " ════════════════════════════════════════════════ \n";
    std::cout << "║              FINAL TEST SUMMARY                ║\n";
    std::cout << " ════════════════════════════════════════════════ \n";
    std::cout << "║ Vector Tests:        " << (vectorPass ? " PASSED" : " FAILED") << "                  ║\n";
    std::cout << "║ Matrix Tests:        " << (matrixPass ? " PASSED" : " FAILED") << "                  ║\n";
    std::cout << "║ Matrix-Vector Tests: " << (matrixVectorPass ? " PASSED" : " FAILED") << "                  ║\n";
    std::cout << "║ Square Tests: " << (squarePass? " PASSED" : " FAILED") << "                  ║\n";
    std::cout << "║ Triangle Tests: " << (trianglePass ? " PASSED" : " FAILED") << "                  ║\n";
    std::cout << " ════════════════════════════════════════════════ \n";
    std::cout << "║ Overall:             " << (vectorPass && matrixPass && matrixVectorPass && squarePass && trianglePass ? " ALL PASSED" : " SOME FAILED") << "              ║\n";
    std::cout << " ════════════════════════════════════════════════ \n";

    return (vectorPass && matrixPass && matrixVectorPass) ? 0 : 1;
}
