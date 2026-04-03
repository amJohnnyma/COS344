#ifndef PHYSICS_H
#define PHYSICS_H

#include "../math/Vector.h"
#include "../math/Shape.h"  
#include "PhysicsBody.h"
#include <cmath>


template <int n>
struct Collision {
    Vector<n> normal;       // unit normal pointing toward circle
    float penetration; // positive = overlap depth
    Vector<n> contactPoint; // useful for better response
    bool valid = false;
    PhysicsType otherPType;
    Shape<n> * otherShape = nullptr;
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
template<typename T>
static T clamp(T v, T lo, T hi) { return v < lo ? lo : (v > hi ? hi : v); }

class PhysicsEngine {
    public:
        template<int n>
            void update(Shape<n>** bodies, int count, float dt) {
                for (int i = 0; i < count; i++) {
                    if (bodies[i]->physicsBodyActive())
                        bodies[i]->updatePhysics(dt);
                }
                for (int i = 0; i < count; i++) {
                    for (int j = i + 1; j < count; j++) {
                        if(!bodies[i]->physicsBodyActive() && !bodies[j]->physicsBodyActive()) continue;

                        Collision<n> col = detectCollision<n>(*bodies[i], *bodies[j]);
                        if ((col.valid && col.penetration > 0))
                        {
    //printf("Collision: body[%d] (hasPhysics=%d, type=%d) vs body[%d] (hasPhysics=%d, type=%d) | penetration=%.4f\n",
     //   i, bodies[i]->physicsBodyActive(), bodies[i]->getPhysicsType(),
      //  j, bodies[j]->physicsBodyActive(), bodies[j]->getPhysicsType(),
       // col.penetration);
    if (bodies[i]->physicsBodyActive())
        resolveCollision<n>(bodies[i]->getPhysicsBody(), bodies[j]->getPhysicsBody(), col);
    else
        resolveCollision<n>(bodies[j]->getPhysicsBody(), bodies[i]->getPhysicsBody(), col);
                        }
                    }
                }
            }

    private:
        template<int n>
            Collision<n> detectCollision(const Shape<n>& a, const Shape<n>& b) const {
                bool aIsCircle = (a.getPhysicsType() == PhysicsType::BALL);
                bool bIsCircle = (b.getPhysicsType() == PhysicsType::BALL);

                if(aIsCircle && bIsCircle) return circleVsCircle(a.getPhysicsBody(), b.getPhysicsBody());
                if(aIsCircle) return circleVsShape(a.getPhysicsBody(), &b);
                if(bIsCircle) return circleVsShape(b.getPhysicsBody(), &a);

                return Collision<n>{};
            }


        template <int n>
            Collision<n> circleVsLineSegment(const PhysicsBody<n>& circle,
                    const Vector<n>& lineStart,
                    const Vector<n>& lineEnd) const {
                Collision<n> col;

                // Project to 2D
                Vector<2> c2  = circle.pos2D();
                Vector<2> s2  = {lineStart[0], lineStart[1]};
                Vector<2> e2  = {lineEnd[0],   lineEnd[1]};

                Vector<2> L   = e2 - s2;
                float     lenSq = L * L;
                if (lenSq < 1e-8f) return col;

                Vector<2> toCircle = c2 - s2;
                float t = clamp((toCircle * L) / lenSq, 0.0f, 1.0f);

                Vector<2> closest2 = s2 + L * t;
                Vector<2> delta2   = c2 - closest2;
                float distSq   = delta2 * delta2;

                if (distSq > circle.radius * circle.radius) return col;

                float dist = std::sqrt(distSq);
                col.penetration  = circle.radius - dist;
                Vector<2> normal2 = (dist > 1e-6f) ? delta2.unitVector() : Vector<2>{0,1};

                // Lift back to n dimensions (z=0 or keep original z if needed)
                col.normal[0] = normal2[0];
                col.normal[1] = normal2[1];
                // col.normal[2..n-1] = 0 or preserve if meaningful


                Vector<2> contactPoint2 = Vector<2>{col.contactPoint[0], col.contactPoint[1]};
                contactPoint2 = closest2;           // again, z=0 or from line
                col.contactPoint[0] = closest2[0];
                col.contactPoint[1] = closest2[1];

                col.valid = true;
                return col;
            }
        // Circle vs arbitrary Shape<n> (polygon edges)
        // Z-layer gating: only collide if the circle's z overlaps the shape's z range.
        // For n=2 there is no z, so always collide. For n=3, z is "height" and two
        // objects at different heights don't physically interact.
        template<int n>
            Collision<n> circleVsShape(const PhysicsBody<n>& circle, const Shape<n>* shape) const {
                Collision<n> deepest;
                deepest.penetration = -1e9f;

                float* raw = shape->getPoints();
                int num = shape->getNumPoints();
                if (num < 3) {
                    delete[] raw;
                    return deepest;
                }
                // check type of other


                // Z-layer check (only meaningful for n >= 3)
                if constexpr (n >= 3) {
                    // Gather the z values of this shape's vertices to find its z range
                    float shapeZMin =  1e9f, shapeZMax = -1e9f;
                    for (int i = 0; i < num; ++i) {
                        float z = raw[i * n + 2];
                        if (z < shapeZMin) shapeZMin = z;
                        if (z > shapeZMax) shapeZMax = z;
                    }
                    float circleZ = circle.pos[2];
                    // If the circle's z is completely outside [shapeZMin, shapeZMax], skip
                    if (circleZ < shapeZMin - circle.radius || circleZ > shapeZMax + circle.radius) {
                        delete[] raw;
                        return deepest;   // no collision — different layers
                    }

                }

                for (int i = 0; i < num; ++i) {
                    int j = (i + 1) % num;
                    Vector<n> a, b;
                    for (int k = 0; k < n; ++k) {
                        a[k] = raw[i * n + k];
                        b[k] = raw[j * n + k];
                    }

                    Collision<n> c = circleVsLineSegment(circle, a, b);
                    if (c.valid && c.penetration > deepest.penetration) {

                        deepest = c;

                        deepest.otherPType = shape->getPhysicsType();
                    }
                }

                delete[] raw;
                return deepest;
            }

        template <int n>
            Collision<n> circleVsCircle(const PhysicsBody<n>& a, const PhysicsBody<n>& b) const
            {
                Collision<n> col;
                Vector<n> delta = a.pos - b.pos;
                float distSq = delta * delta;
                float radSum = a.radius + b.radius;

                if(distSq >= radSum * radSum) return col; // no col


                float dist = std::sqrt(distSq);
                col.penetration = radSum - dist;
                col.normal = (dist > 1e-6f) ? delta * (1.f / dist) : Vector<n>{0,1};
                col.contactPoint = b.pos + col.normal * b.radius;
                col.valid = true;
                return col;
            }

        /*

        // Circle vs AABB (faster for axis-aligned squares)
        template<int n>
        Collision<n> circleVsAABB(const PhysicsBody<n>& circle,
        const Vector<n>& rectCenter,
        const Vector<n>& rectHalfSize) const {
        Collision<n> col;

        Vector<2> c2   = circle.pos2D();
        Vector<2> cen2 = {rectCenter[0], rectCenter[1]};
        Vector<2> half2 = {rectHalfSize[0], rectHalfSize[1]};

        Vector<2> closest2 = c2;
        closest2[0] = std::clamp(c2[0], cen2[0] - half2[0], cen2[0] + half2[0]);
        closest2[1] = std::clamp(c2[1], cen2[1] - half2[1], cen2[1] + half2[1]);

        Vector<2> delta2 = c2 - closest2;
        float distSq = delta2 * delta2;

        if (distSq > circle.radius * circle.radius + 1e-6f) return col;

        float dist = (distSq > 1e-8f) ? std::sqrt(distSq) : 0.0f;
        Vector<2> normal2 = (dist > 1e-6f) ? delta2.normalized() : Vector<2>{0,1};

        col.penetration  = circle.radius - dist;
        col.normal[0]    = normal2[0];
        col.normal[1]    = normal2[1];
        col.contactPoint[0] = closest2[0];
        col.contactPoint[1] = closest2[1];
        col.valid = true;
        return col;
        }
        */

        // Resolve collision against a static wall.
        // All impulse math is 2D (x-y plane); z velocity is left untouched.
        template <int n>
            void resolveCollision(PhysicsBody<n>& a, PhysicsBody<n>& b, const Collision<n>& col) {
                if ((!col.valid || col.penetration <= 0)) return;



                Vector<2> norm2 = {col.normal[0], col.normal[1]};

                // if it is water then just slow the ball down
                if(col.otherPType == PhysicsType::WATER)
                {
                    a.setVel2D(a.vel2D() * 0.7f);
                    return;
                }

                bool bIsDynamic = (col.otherPType == PhysicsType::BALL);

                if (bIsDynamic)
                {
                    float totalMass = a.mass + b.mass;
                    float aShare = b.mass / totalMass;
                    float bShare = a.mass / totalMass;

                    a.pos[0] += col.normal[0] * (col.penetration + 0.001f) * aShare;
                    a.pos[1] += col.normal[1] * (col.penetration + 0.001f) * aShare;
                    b.pos[0] -= col.normal[0] * (col.penetration + 0.001f) * bShare;
                    b.pos[1] -= col.normal[1] * (col.penetration + 0.001f) * bShare;
                }
                else{

                    a.pos[0] += col.normal[0] * (col.penetration + 0.001f);
                    a.pos[1] += col.normal[1] * (col.penetration + 0.001f);
                }


                Vector<2> velA = a.vel2D();
                Vector<2> velB = b.vel2D();
                Vector<2> relVel = velA - velB;
                float vDotN = relVel * norm2;

                if(vDotN >= 0) return; // already moving apart

                float e = bIsDynamic ? std::min(a.restitution, b.restitution) : a.restitution;
                float invMassSum = bIsDynamic ? (1.f / a.mass + 1.f / b.mass) : (1.f / a.mass);
                float j = -(1.f + e) * vDotN / invMassSum;

                Vector<2> impulse2 = norm2 * j;
                a.setVel2D(velA + impulse2 * (1.f / a.mass));
                if(bIsDynamic)
                    b.setVel2D(velB + impulse2 * (1.f / b.mass));

                velA = a.vel2D();
                velB = bIsDynamic ? b.vel2D() : Vector<2>{0,0};
                Vector<2> tangent2 = {-norm2[1], norm2[0]};
                float vTan = (velA - velB) * tangent2;
                Vector<2> frictionImpulse = tangent2 * (vTan * 0.15f * 0.5f);
                a.setVel2D(velA - frictionImpulse);
                if(bIsDynamic)
                    b.setVel2D(velB + frictionImpulse);

            }

};


#endif
