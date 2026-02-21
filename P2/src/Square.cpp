#include "../include/math/Square.h"

template<int n>
Square<n>::Square(const Vector<n>& center, float height, float width)
{

    /*
Apart from the first and second components, all of the member variables should have the
same values for the remaining component as the centre parameter. In other words, if the
centre parameter has the value of 5 for the third component, all of the member variables
should have the value of 5 for the third component.
*/
    float half_width  = width  / 2.0f;
    float half_height = height / 2.0f;

    // For n=1
    if (n == 1)
    {
        Vector<n> halfW{width / 2.0f};
        tl = center - halfW;
        tr = center + halfW;
        bl = center - halfW;
        br = center + halfW;
        return;
    }

    // Normal 2D+ case: set x and y offsets, copy higher components from center
    Vector<n> offset_tl{-half_width, +half_height};
    Vector<n> offset_tr{+half_width, +half_height};
    Vector<n> offset_br{+half_width, -half_height};
    Vector<n> offset_bl{-half_width, -half_height};

    tl = center + offset_tl;
    tr = center + offset_tr;
    br = center + offset_br;
    bl = center + offset_bl;

    // Copy components 2..n-1 from center (z, w, etc.)
    for (int i = 2; i < n; ++i)
    {
        tl[i] = center[i];
        tr[i] = center[i];
        br[i] = center[i];
        bl[i] = center[i];
    }
}
template<int n>
Square<n>::Square(const Vector<n>& tl, const Vector<n>& tr, const Vector<n>& br, const Vector<n>& bl) 
{
    this->tl = Vector<n>(tl);
    this->tr = Vector<n>(tr);
    this->bl = Vector<n>(bl);
    this->br = Vector<n>(br);

}
template<int n>
Square<n>::Square(const Square<n>& other) 
{
    tl = Vector<n>(other.tl);
    tr = Vector<n>(other.tr);
    bl = Vector<n>(other.bl);
    br = Vector<n>(other.br);

}
template<int n>
Square<n>& Square<n>::operator*=(const Matrix<n,n>& other)
{
    // Create temporary copies so don't overwrite values too early
    Vector<n> old_tl = tl;
    Vector<n> old_tr = tr;
    Vector<n> old_br = br;
    Vector<n> old_bl = bl;

    // For each corner: new_vector[i] = sum over j (other[i][j] * old_vector[j])
    for (int i = 0; i < n; ++i)
    {
        float sum_tl = 0.0f;
        float sum_tr = 0.0f;
        float sum_br = 0.0f;
        float sum_bl = 0.0f;

        for (int j = 0; j < n; ++j)
        {
            // Using existing Matrix::operator[] → float*
            sum_tl += other[i][j] * old_tl[j];
            sum_tr += other[i][j] * old_tr[j];
            sum_br += other[i][j] * old_br[j];
            sum_bl += other[i][j] * old_bl[j];
        }

        tl[i] = sum_tl;
        tr[i] = sum_tr;
        br[i] = sum_br;
        bl[i] = sum_bl;
    }

    return *this;
}
template<int n>
Square<n>* Square<n>::operator*(const Matrix<n,n>& other) const 
{
    Square<n>* result = new Square<n>(*this);
    *result *= other;
    return result;

}

// return 6 vertices
template<int n>
float* Square<n>::getPoints() const
{
    float* points = new float[6 * n];
    int idx = 0;

    // Triangle 1:  tl → tr → bl
    for (int k = 0; k < n; ++k) points[idx++] = tl[k];
    for (int k = 0; k < n; ++k) points[idx++] = tr[k];
    for (int k = 0; k < n; ++k) points[idx++] = bl[k];

    // Triangle 2:  tr → br → bl
    for (int k = 0; k < n; ++k) points[idx++] = tr[k];
    for (int k = 0; k < n; ++k) points[idx++] = br[k];
    for (int k = 0; k < n; ++k) points[idx++] = bl[k];



    return points;

}
template<int n>
int Square<n>::getNumPoints() const 
{
    return 6;

}


template class Square<2>;
