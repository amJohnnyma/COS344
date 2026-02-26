#ifndef PHYSICS_H
#define PHYSICS_H

#include "../math/Vector.h"
#include "../math/Shape.h"  
#include "PhysicsBody.h"
#include <vector>
#include <algorithm>
#include <cmath>

using Vec2 = Vector<2>;

struct Collision {
    Vec2 normal;       // unit normal pointing toward circle
    float penetration; // positive = overlap depth
    Vec2 contactPoint; // useful for better response
    bool valid = false;
};

struct LineSegment {
    Vec2 a, b;
    Vec2 direction() const { return b - a; }
    float length() const { return direction().magnitude(); }
    Vec2 unit() const {
        float len = length();
        return len > 1e-6f ? direction() * (1.0f / len) : Vec2{0,0};
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
    Collision circleVsLineSegment(const PhysicsBody& circle,
                                 const Vec2& lineStart, const Vec2& lineEnd) const {

        Collision col;
        Vec2 L = lineEnd - lineStart;
        float lenSq = L * L;  // dot product
        if (lenSq < 1e-8f) return col;  // degenerate

        Vec2 toCircle = circle.pos - lineStart;
        float t = std::clamp((toCircle * L) / lenSq, 0.0f, 1.0f);

        Vec2 closest = lineStart + L * t;
        Vec2 delta   = circle.pos - closest;
        float distSq = delta * delta;

        if (distSq > circle.radius * circle.radius) {
            return col;  // no collision
        }

        float dist = std::sqrt(distSq);
        col.penetration = circle.radius - dist;
        col.normal      = (dist > 1e-6f) ? delta * (1.0f / dist) : Vec2{0,1}; // fallback
        col.contactPoint = closest;
        col.valid       = true;

        return col;
    }

    //Circle vs Shape<2> by extracting edges    
    Collision circleVsShape(const PhysicsBody& circle, const Shape<2>* shape) const {
        Collision deepest;
        deepest.penetration = -1e9f;

        // Very basic: get all points, assume convex polygon → check each edge
        float* raw = shape->getPoints();
        int n = shape->getNumPoints();
        if (n < 3) return deepest;

        for (int i = 0; i < n; ++i) {
            Vec2 a{raw[i*2],     raw[i*2+1]};
            Vec2 b{raw[(i+1)%n*2], raw[(i+1)%n*2 + 1]};

            Collision c = circleVsLineSegment(circle, a, b);
            if (c.valid && c.penetration > deepest.penetration) {
                deepest = c;
            }
        }

        delete[] raw;  // Important! your getPoints() returns new[]
        return deepest;
    }

    void resolveCollision(PhysicsBody& ball,
                         const Collision& col,
                         PhysicsBody* wallBody = nullptr) {  // wallBody = nullptr for static

        if (!col.valid || col.penetration <= 0) return;

        // Position correction (push out of wall)
        ball.pos = ball.pos + col.normal * (col.penetration + 0.001f);

        // Velocity reflection
        float vDotN = ball.vel * col.normal;
        if (vDotN >= 0) return;  // already moving away

        Vec2 impulse = col.normal * (-(1.0f + ball.restitution) * vDotN);

        // If wall is static → infinite mass
        if (!wallBody) {
            ball.vel = ball.vel + impulse;
        } else {
            // Future: elastic collision between two moving bodies
            // For now most walls are static
        }

        // Optional: reduce velocity along tangent (extra friction)
        Vec2 tangent = {-col.normal.get_arr()[1], col.normal.get_arr()[0]};
        float vTangent = ball.vel * tangent;
        ball.vel = ball.vel - tangent * (vTangent * 0.15f);  // small sliding friction
    }
};

#endif
