#include "../include/math/PointLight.h"

// Constructor
template <int n>
PointLight<n>::PointLight(const Vector<n>& pos, const Vector<n>& col, const float & radius) {
    // Empty
}

// Destructor
template <int n>
PointLight<n>::~PointLight() {
    // Empty
}

// Add child
template <int n>
void PointLight<n>::add(Shape<n>* child) {
    // Empty
}

// Remove child
template <int n>
void PointLight<n>::remove(Shape<n>* child) {
    // Empty
}

// Get points (PointLight has no points)
template <int n>
float* PointLight<n>::getPoints() const {
    return nullptr;
}

// Get number of points
template <int n>
int PointLight<n>::getNumPoints() const {
    return 0;
}

// Print
template <int n>
void PointLight<n>::print() const {
    // Empty
}

// Render
template <int n>
void PointLight<n>::render(Renderer<n>& r) const {
    // Empty
}

// Set color
template <int n>
void PointLight<n>::setColor(float r, float g, float b, float a) {
    // Empty
}

// Rotate
template <int n>
void PointLight<n>::rotate(Vector<n> angles,
                           Vector<n> rotate_point,
                           bool hasCentroid) {
    // Empty
}

    template <int n>
PointLight<n>& PointLight<n>::operator*=(const Matrix<n,n>& mat)
{
    return *this;
}

// Rotate3D
template <int n>
void PointLight<n>::rotate3D(const Vector<n>& angles,
                             Vector<n> pivot,
                             bool hasPivot) {
    // Empty
}

// Apply translation
template <int n>
void PointLight<n>::applyTranslation(const Vector<n>& disp) {
    // Empty
}

// Update physics
template <int n>
void PointLight<n>::updatePhysics(float dt) {
    // Empty
}

// Scale
template <int n>
void PointLight<n>::scale(float s) {
    // Empty
}

// Rotate around axis
template <int n>
void PointLight<n>::rotateAroundAxis(const Vector<n>& axis,
                                     float angle,
                                     Vector<n> pivot) {
    // Empty
}

template class PointLight<3>;
