#ifndef SQUARE_H
#define SQUARE_H

#include "Shape.h"
#include "Matrix.h"
#include "Vector.h"
#include "Triangle.h"

#include <vector>

// composite
template <int n>
class Square : public Shape<n> {
private:
    Vector<n> tl;
    Vector<n> tr;
    Vector<n> br;
    Vector<n> bl;
    
    std::vector<Shape<n>*> children;
    
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

};

#endif /*SQUARE_H*/
