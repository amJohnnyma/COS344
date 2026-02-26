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

    virtual void updatePhysics(float dt) override
    {
        this->physicsBody.vel = Vector<2>{0.01f, 0.f};
            this->physicsBody.update(dt); 
            this->position = this->physicsBody.pos;
            updatePosition();

    }

    virtual void updatePosition() override 
    {
        std::cout << "Vel: "; this->physicsBody.vel.print();
        p1 = p1 + this->physicsBody.vel;
        p2 = p2 + this->physicsBody.vel;
        p3 = p3 + this->physicsBody.vel;
    }

};

#endif /*TRIANGLE_H*/
