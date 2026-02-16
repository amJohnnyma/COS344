//#include "../include/Matrix.h"
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

int main(int argc, char const *argv[])
{
    bool vectorPass = vector_tests();

    return 0;
    
}
