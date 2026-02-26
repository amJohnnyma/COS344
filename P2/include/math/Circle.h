#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"
#include "Vector.h"
#include "Triangle.h"
#include <cmath>
#include <vector>

template <int n>
class Circle : public Shape<n> {
private:
    Vector<n> center;
    float radius;
    int segments;   // e.g. 32 for a smooth circle
    std::vector<Shape<n>*> children;

    std::vector<Shape<n>*> own_shape; // the square stores it's own triangle 
public:
    ~Circle() override {for(Shape<n>* child : children) delete child;}
    Circle(const Vector<n>& c, float r, int segs = 32);
    Circle(const Circle<n>&);
    virtual Circle<n>& operator*=(const Matrix<n,n>&) override;

    // Returns (segments * 3 * n) floats — triangle-fan decomposed into triangles.
    // Each triangle: center, p_i, p_{i+1}.
    virtual float* getPoints() const override;
    virtual int getNumPoints() const override{ return segments * 3; }

    virtual void print() const override{
        std::cout << "Circle center:" << std::endl;
        center.print();
        std::cout << "radius=" << radius
                  << " segments=" << segments << std::endl;
    }

    // for composite
    void add(Shape<n> * child) {children.push_back(child); }
    void remove(Shape<n>* child) {
        for (auto it = children.begin(); it != children.end(); ++it) {
            if (*it == child) {
                children.erase(it);
                break;
            }
        }
    }
    void render(Renderer<n>& r) const override
    {
        for (Shape<n>* child : children)
        {
            child->render(r);
        }
    }
    void setColor(float r, float g, float b, float a = 1.0f) override
    {
        Shape<n>::setColor(r, g, b, a);
        for (Shape<n>* child : children)
            child->setColor(r, g, b, a);
    }

    virtual void rotate(float theta, Vector<n> rotate_point = Vector<n>(), bool hasCentroid = false) override;

    virtual void updatePhysics(float dt) override
    {
        Shape<n>::updatePhysics(dt);
        for(auto * child : children)
        {
            child->updatePhysics(dt);
        }
    }

    virtual void applyTranslation(const Vector<n>& disp) override
    {
        center = center +  disp;
        for (Shape<n>* child : children) child->applyTranslation(disp);

    }

};

#endif /*CIRCLE_H*/
