
#include "../math/Vector.h"
#include "../math/Matrix.h"

template <int n>
class Camera {
    Vector<n> pos;
    float zoom;
public:
    void follow(const Vector<n>& target);
    Matrix<n,n> getViewMatrix() const;  // Translate(-pos) * Scale(zoom)
    Matrix<n,n> getProjMatrix(float aspect) const;  // Ortho(-10,10,-10,10,0,100) for n=2/3
};
