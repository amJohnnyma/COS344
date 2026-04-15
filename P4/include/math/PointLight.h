
// basically an empty class
#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Shape.h"
#include "Vector.h"
#include "Triangle.h"
#include <cmath>

template <int n>
class PointLight : public Shape<n> {

    public:
    PointLight(const Vector<n>& pos, const Vector<n>& col, const float & radius);

    ~PointLight() override;


    void add(Shape<n>* child);

    void remove(Shape<n>* child);

    virtual PointLight<n>& operator*=(const Matrix<n,n>& mat) override;


    virtual float* getPoints() const override; // has no points

    virtual int getNumPoints() const override; // 0 points

    virtual void print() const override;

    virtual void render(Renderer<n>& r) const override;

    virtual void setColor(float r, float g, float b, float a = 1.0f) override;

    virtual void rotate(Vector<n> angles,
                        Vector<n> rotate_point = Vector<n>(),
                        bool hasCentroid = false) override;

    void rotate3D(const Vector<n>& angles,
                  Vector<n> pivot = Vector<n>(),
                  bool hasPivot = false) override;

    virtual void applyTranslation(const Vector<n>& disp) override;

    virtual void updatePhysics(float dt) override;

    virtual void scale(float s) override;
    virtual void rotateAroundAxis(const Vector<n>& axis, float angle, Vector<n> pivot) override;

};

#endif /*CIRCLE_H*/
