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

    void rotate3D(const Vector<n>& angles,
                  Vector<n> pivot = Vector<n>(),
                  bool hasPivot = false) override;
    virtual void rotate(Vector<n> angles,
                        Vector<n> rotate_point = Vector<n>(),
                        bool hasCentroid = false) override;
void recalcPosition();

    virtual void applyTranslation(const Vector<n>& disp) override
    {
        p1 = p1 + disp;
        p2 = p2 + disp;
        p3 = p3 + disp;

        this->position = this->position + disp;
    }

    virtual void scale(float s) override;
    virtual void rotateAroundAxis(const Vector<n>& axis, float angle, Vector<n> pivot) override;
const Vector<n>& getA() const { return p1; }
const Vector<n>& getB() const { return p2; }
const Vector<n>& getC() const { return p3; }


};

#endif /*TRIANGLE_H*/
