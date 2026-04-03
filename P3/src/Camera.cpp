#include "../include/engine/Camera.h"

template<int n>
Matrix<n,n> Camera<n>::makeIdentity()
{
    Matrix<n,n> m;
    for (int i = 0; i < n; ++i)
        m[i][i] = 1.0f;
    return m;
}

template<int n>
Matrix<n,n> Camera<n>::getViewMatrix() const
{
    Matrix<n,n> m = makeIdentity();
    for (int i = 0; i < n; ++i)
        m[i][i] = zoom;
    return m;
}

template<int n>
Matrix<n,n> Camera<n>::getProjMatrix(float aspect) const
{
    Matrix<n,n> m = makeIdentity();
    float halfW = 10.0f / zoom;
    float halfH = 10.0f / zoom;
    if (n >= 1) m[0][0] = 1.0f / (halfW * aspect);
    if (n >= 2) m[1][1] = 1.0f / halfH;
    return m;
}

template class Camera<2>;
template class Camera<3>;
