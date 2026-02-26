#ifndef PHYSICS_H
#define PHYSICS_H

#include "../math/Vector.h"
#include "../math/Shape.h"  
#include "PhysicsBody.h"
#include <vector>
#include <algorithm>
#include <cmath>


template <int n>
struct Collision {
    Vector<n> normal;       // unit normal pointing toward circle
    float penetration; // positive = overlap depth
    Vector<n> contactPoint; // useful for better response
    bool valid = false;
};

template <int n>
struct LineSegment {
    Vector<n> a, b;
    Vector<n> direction() const { return b - a; }
    float length() const { return direction().magnitude(); }
    Vector<n> unit() const {
        float len = length();
        return len > 1e-6f ? direction() * (1.0f / len) : Vector<n>{0,0};
    }
};


class PhysicsEngine {
public:
    template<int n>
    void update(std::vector<Shape<n>*>& bodies, float dt) {
        for (auto* body : bodies) {
            body->updatePhysics(dt);
        }
    }

    //Collision: Circle vs one line segmen
    template <int n>
    Collision<n> circleVsLineSegment(const PhysicsBody<n>& circle,
                                 const Vector<n>& lineStart, const Vector<n>& lineEnd) const {

        Collision<n> col;
        Vector<n> L = lineEnd - lineStart;
        float lenSq = L * L;  // dot product
        if (lenSq < 1e-8f) return col;  // degenerate

        Vector<n> toCircle = circle.pos - lineStart;
        float t = std::clamp((toCircle * L) / lenSq, 0.0f, 1.0f);

        Vector<n> closest = lineStart + L * t;
        Vector<n> delta   = circle.pos - closest;
        float distSq = delta * delta;

        if (distSq > circle.radius * circle.radius) {
            return col;  // no collision
        }

        float dist = std::sqrt(distSq);
        col.penetration = circle.radius - dist;
        col.normal      = (dist > 1e-6f) ? delta * (1.0f / dist) : Vector<n>{0,1}; // fallback
        col.contactPoint = closest;
        col.valid       = true;

        return col;
    }

    //Circle vs Shape<2> by extracting edges    
    template<int k>
    Collision<k> circleVsShape(const PhysicsBody<k>& circle, const Shape<k>* shape) const {
        Collision<k> deepest;
        deepest.penetration = -1e9f;

        // Very basic: get all points, assume convex polygon → check each edge
        float* raw = shape->getPoints();
        int n = shape->getNumPoints();
        if (n < 3) return deepest;

        for (int i = 0; i < n; ++i) {
            Vector<k> a{raw[i*2],     raw[i*2+1]};
            Vector<k> b{raw[(i+1)%n*2], raw[(i+1)%n*2 + 1]};

            Collision c = circleVsLineSegment(circle, a, b);
            if (c.valid && c.penetration > deepest.penetration) {
                deepest = c;
            }
        }

        delete[] raw;  // Important! your getPoints() returns new[]
        return deepest;
    }

    template <int n>
    void resolveCollision(PhysicsBody<n>& ball,
                         const Collision<n>& col,
                         PhysicsBody<n>* wallBody = nullptr) {  // wallBody = nullptr for static

        if (!col.valid || col.penetration <= 0) return;

        // Position correction (push out of wall)
        ball.pos = ball.pos + col.normal * (col.penetration + 0.001f);

        // Velocity reflection
        float vDotN = ball.vel * col.normal;
        if (vDotN >= 0) return;  // already moving away

        Vector<n> impulse = col.normal * (-(1.0f + ball.restitution) * vDotN);

        // If wall is static → infinite mass
        if (!wallBody) {
            ball.vel = ball.vel + impulse;
        } else {
            // Future: elastic collision between two moving bodies
            // For now most walls are static
        }

        // Optional: reduce velocity along tangent (extra friction)
        Vector<n> tangent = {-col.normal.get_arr()[1], col.normal.get_arr()[0]};
        float vTangent = ball.vel * tangent;
        ball.vel = ball.vel - tangent * (vTangent * 0.15f);  // small sliding friction
    }
};

#endif
