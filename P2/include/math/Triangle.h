#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"
#include "Vector.h"

//leaf
template <int n>
class Triangle : public Shape<n> {
private:
    Vector<n> p1;
    Vector<n> p2;
    Vector<n> p3;
public:
    ~Triangle() override {}
    Triangle(const Vector<n>& p1, const Vector<n>& p2, const Vector<n>& p3);
    Triangle(const Triangle<n>&);
    virtual Triangle<n>& operator*=(const Matrix<n,n>&) override;
    virtual float* getPoints() const override;
    virtual int getNumPoints() const override;

    virtual void print() const override{
        std::cout << "_ P1 _ " << std::endl; p1.print();
        std::cout << "_ P2 _ " << std::endl; p2.print();
        std::cout << "_ P3 _ " << std::endl; p3.print();
    }
    virtual void render(Renderer<n>& r) const override;

    virtual void rotate(float theta, Vector<n> rotate_point = Vector<n>(), bool hasCentroid = false) override;

    virtual void applyTranslation(const Vector<n>& disp) override
    {
        p1 = p1 + disp;
        p2 = p2 + disp;
        p3 = p3 + disp;

        this->position = this->position + disp;
    }

    virtual void scale(float s) override
    {
        Vector<n> centroid;
        for (int i = 0; i < n; ++i)
            centroid[i] = (p1[i] + p2[i] + p3[i]) / 3.0f;

        p1 = centroid + (p1 - centroid) * s;
        p2 = centroid + (p2 - centroid) * s;
        p3 = centroid + (p3 - centroid) * s;
    }


};

#endif /*TRIANGLE_H*/
