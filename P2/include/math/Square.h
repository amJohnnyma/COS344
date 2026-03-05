#ifndef SQUARE_H
#define SQUARE_H

#include "Shape.h"
#include "Matrix.h"
#include "Vector.h"
#include "Triangle.h"


// composite
template <int n>
class Square : public Shape<n> {
private:
    Vector<n> tl;
    Vector<n> tr;
    Vector<n> br;
    Vector<n> bl;
    
    static const int MAX_CHILDREN = 128;
    Shape<n>* children[MAX_CHILDREN] = {};
    int childCount = 0;
    
public:
    ~Square() override {for(Shape<n>* child : children) delete child;}
    Square(const Vector<n>& center, float height, float width);
    Square(const Vector<n>& tl, const Vector<n>& tr,
           const Vector<n>& br, const Vector<n>& bl);
    Square(const Square<n>&);
    virtual Square<n>& operator*=(const Matrix<n,n>&) override;
    virtual float* getPoints() const override;
    virtual int getNumPoints() const override;

    virtual void print() const override{
        std::cout << "_ TL _ " << std::endl; tl.print();
        std::cout << "_ TR _ " << std::endl; tr.print();
        std::cout << "_ BR _ " << std::endl; br.print();
        std::cout << "_ BL _ " << std::endl; bl.print();
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
        for(int i = 0; i < childCount; i ++) children[i]->render(r);
    }
    void setColor(float r, float g, float b, float a = 1.0f) override
    {
        Shape<n>::setColor(r,g,b,a);
        for(int i = 0; i < childCount; i ++) children[i]->setColor(r,g,b,a);
    }

    virtual void rotate(float theta, Vector<n> rotate_point = Vector<n>(), bool hasCentroid = false) override;

    virtual void applyTranslation(const Vector<n>& disp) override
    {
        tl = tl + disp; tr = tr + disp; br = br + disp; bl = bl + disp;
        this->position = this->position + disp;

        for(int i = 0; i < childCount; i ++) children[i]->applyTranslation(disp);

    }

    virtual void updatePhysics(float dt) override
    {
        Shape<n>::updatePhysics(dt);
        for(int i = 0; i < childCount; i ++) children[i]->updatePhysics(dt);
    }

    virtual void scale(float s) override
    {
        Vector<n> center = (tl + tr + br + bl) * 0.25f;

        tl = center + (tl - center) * s;
        tr = center + (tr - center) * s;
        br = center + (br - center) * s;
        bl = center + (bl - center) * s;

        // Clear old children and rebuild like the constructor
        for (int i = 0; i < childCount; i++)
            delete children[i];
        childCount = 0;

        rebuild();
        this->setColor(this->color[0],this->color[1],this->color[2]);
    }
    void rebuild()
    {
        for (int i = 0; i < childCount; i++) delete children[i];
        childCount = 0;
        add(new Triangle<n>(tl, tr, br));
        add(new Triangle<n>(tl, br, bl));
        this->setColor(this->color[0], this->color[1], this->color[2], this->color[3]);
    }


};

#endif /*SQUARE_H*/
