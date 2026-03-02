#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"
#include "Vector.h"
#include "Triangle.h"
#include <cmath>

template <int n>
class Circle : public Shape<n> {
private:
    Vector<n> center;
    float radius;
    int segments;   // e.g. 32 for a smooth circle
    static const int MAX_CHILDREN = 128;
    Shape<n>* children[MAX_CHILDREN] = {};
    int childCount = 0;

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
    void add(Shape<n>* child)
    { 
        if (childCount < MAX_CHILDREN) 
            children[childCount++] = child; 
    }
    void remove(Shape<n>* child) {
        for (int i = 0; i < childCount; i++) {
            if (children[i] == child) {
                for (int j = i; j < childCount - 1; j++) children[j] = children[j+1];
                childCount--;
                return;
            }
        }
    }
    void render(Renderer<n>& r) const override
    {
        for(int i = 0; i < childCount; i++) children[i]->render(r);
    }
    void setColor(float r, float g, float b, float a = 1.0f) override
    {
        Shape<n>::setColor(r, g, b, a);
        for(int i = 0; i < childCount; i++) children[i]->setColor(r,g,b,a);
    }

    virtual void rotate(float theta, Vector<n> rotate_point = Vector<n>(), bool hasCentroid = false) override;

    virtual void updatePhysics(float dt) override
    {
        Shape<n>::updatePhysics(dt);

        for(int i = 0; i < childCount; i++) children[i]->updatePhysics(dt);
    }

    virtual void applyTranslation(const Vector<n>& disp) override
    {
        center = center +  disp;

        for(int i = 0; i < childCount; i++) children[i]->applyTranslation(disp);

    }

};

#endif /*CIRCLE_H*/
