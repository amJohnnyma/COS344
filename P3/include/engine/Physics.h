#ifndef PHYSICS_H
#define PHYSICS_H

#include "../math/Vector.h"
#include "../math/Shape.h"  
#include "PhysicsBody.h"
#include <cmath>

template <int n>
struct Collision {
    Vector<n> normal;
    float penetration;
    Vector<n> contactPoint;
    bool valid = false;
    PhysicsType otherPType;
    Shape<n>* otherShape = nullptr;
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
                if (!bodies[i]->physicsBodyActive() && !bodies[j]->physicsBodyActive()) continue;

                Collision<n> col = detectCollision<n>(*bodies[i], *bodies[j]);
                if (col.valid && col.penetration > 0)
                {
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
        bool aIs = (a.getPhysicsType() == PhysicsType::BALL);
        bool bIs = (b.getPhysicsType() == PhysicsType::BALL);

        if (aIs && bIs) return circleSphereSelf(a.getPhysicsBody(), b.getPhysicsBody());
        if (aIs)        return circleSphereVsShape(a.getPhysicsBody(), &b);
        if (bIs)        return circleSphereVsShape(b.getPhysicsBody(), &a);

        return Collision<n>{};
    }


    template<int n>
    Collision<n> circleSphereSelf(const PhysicsBody<n>& a, const PhysicsBody<n>& b) const
    {
        if constexpr (n == 2)
            return circleVsCircle(a, b);
        else
            return sphereVsSphere(a, b);
    }

    template<int n>
    Collision<n> circleSphereVsShape(const PhysicsBody<n>& body, const Shape<n>* shape) const
    {
        if constexpr (n == 2)
            return circleVsShape(body, shape);
        else
            return sphereVsShape(body, shape);
    }


    template<int n>
    Collision<n> circleVsCircle(const PhysicsBody<n>& a, const PhysicsBody<n>& b) const
    {
        Collision<n> col;
        Vector<n> delta = a.pos - b.pos;
        float distSq = delta * delta;
        float radSum = a.radius + b.radius;
        if (distSq >= radSum * radSum) return col;

        float dist = std::sqrt(distSq);
        col.penetration = radSum - dist;
        col.normal = (dist > 1e-6f) ? delta * (1.f / dist) : Vector<n>{0, 1};
        col.contactPoint = b.pos + col.normal * b.radius;
        col.valid = true;
        return col;
    }

    template<int n>
    Collision<n> circleVsLineSegment(const PhysicsBody<n>& circle,
                                      const Vector<n>& lineStart,
                                      const Vector<n>& lineEnd) const
    {
        Collision<n> col;

        Vector<2> c2 = circle.pos2D();
        Vector<2> s2 = {lineStart[0], lineStart[1]};
        Vector<2> e2 = {lineEnd[0],   lineEnd[1]};

        Vector<2> L     = e2 - s2;
        float     lenSq = L * L;
        if (lenSq < 1e-8f) return col;

        Vector<2> toCircle = c2 - s2;
        float t = clamp((toCircle * L) / lenSq, 0.0f, 1.0f);

        Vector<2> closest2 = s2 + L * t;
        Vector<2> delta2   = c2 - closest2;
        float distSq = delta2 * delta2;
        if (distSq > circle.radius * circle.radius) return col;

        float dist = std::sqrt(distSq);
        col.penetration = circle.radius - dist;
        Vector<2> normal2 = (dist > 1e-6f) ? delta2.unitVector() : Vector<2>{0, 1};

        col.normal[0] = normal2[0];
        col.normal[1] = normal2[1];
        col.contactPoint[0] = closest2[0];
        col.contactPoint[1] = closest2[1];
        col.valid = true;
        return col;
    }

    template<int n>
    Collision<n> circleVsShape(const PhysicsBody<n>& circle, const Shape<n>* shape) const
    {
        Collision<n> deepest;
        deepest.penetration = -1e9f;

        float* raw = shape->getPoints();
        int num = shape->getNumPoints();
        if (num < 3) { delete[] raw; return deepest; }

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


    template<int n>
    Collision<n> sphereVsSphere(const PhysicsBody<n>& a, const PhysicsBody<n>& b) const
    {
        Collision<n> col;
        Vector<n> delta = a.pos - b.pos;
        float distSq = 0.f;
        for (int i = 0; i < n; ++i) distSq += delta[i] * delta[i];
        float radSum = a.radius + b.radius;
        if (distSq >= radSum * radSum) return col;

        float dist = std::sqrt(distSq);
        col.penetration = radSum - dist;
        for (int i = 0; i < n; ++i)
            col.normal[i] = (dist > 1e-6f) ? delta[i] / dist : (i == 1 ? 1.f : 0.f);
        col.contactPoint = b.pos;
        col.valid = true;
        return col;
    }

    template<int n>
    Collision<n> sphereVsTriangle(const PhysicsBody<n>& sphere,
                                   const Vector<n>& A,
                                   const Vector<n>& B,
                                   const Vector<n>& C) const
    {
        Collision<n> col;

        Vector<n> AB = B - A;
        Vector<n> AC = C - A;

        Vector<n> normal;
        normal[0] = AB[1]*AC[2] - AB[2]*AC[1];
        normal[1] = AB[2]*AC[0] - AB[0]*AC[2];
        normal[2] = AB[0]*AC[1] - AB[1]*AC[0];

        float len = 0.f;
        for (int i = 0; i < n; ++i) len += normal[i] * normal[i];
        len = std::sqrt(len);
        if (len < 1e-8f) return col;
        for (int i = 0; i < n; ++i) normal[i] /= len;

        Vector<n> toSphere = sphere.pos - A;
        float dist = 0.f;
        for (int i = 0; i < n; ++i) dist += toSphere[i] * normal[i];

        if (dist < 0.f || dist > sphere.radius) return col;

        Vector<n> projected = sphere.pos;
        for (int i = 0; i < n; ++i) projected[i] -= dist * normal[i];

        auto cross2 = [&](const Vector<n>& u, const Vector<n>& v, const Vector<n>& w) {
            Vector<n> uv = v - u;
            Vector<n> uw = w - u;
            float cx = uv[1]*uw[2] - uv[2]*uw[1];
            float cy = uv[2]*uw[0] - uv[0]*uw[2];
            float cz = uv[0]*uw[1] - uv[1]*uw[0];
            return cx*normal[0] + cy*normal[1] + cz*normal[2];
        };

        float d0 = cross2(A, B, projected);
        float d1 = cross2(B, C, projected);
        float d2 = cross2(C, A, projected);

        auto closestOnSegment = [&](const Vector<n>& P, const Vector<n>& Q) {
            Vector<n> PQ = Q - P;
            Vector<n> PS = sphere.pos - P;
            float lenSq = 0.f;
            for (int i = 0; i < n; ++i) lenSq += PQ[i]*PQ[i];
            if (lenSq < 1e-8f) return P;
            float t = 0.f;
            for (int i = 0; i < n; ++i) t += PS[i]*PQ[i];
            t = clamp(t / lenSq, 0.f, 1.f);
            Vector<n> res = P;
            for (int i = 0; i < n; ++i) res[i] += t * PQ[i];
            return res;
        };

        auto distSqTo = [&](const Vector<n>& p) {
            float s = 0.f;
            for (int i = 0; i < n; ++i) { float d = sphere.pos[i]-p[i]; s += d*d; }
            return s;
        };

        Vector<n> closestPoint;
        if (d0 >= 0.f && d1 >= 0.f && d2 >= 0.f)
        {
            closestPoint = projected;
        }
        else
        {
            Vector<n> c0 = closestOnSegment(A, B);
            Vector<n> c1 = closestOnSegment(B, C);
            Vector<n> c2 = closestOnSegment(C, A);

            closestPoint   = c0;
            float best     = distSqTo(c0);
            if (distSqTo(c1) < best) { best = distSqTo(c1); closestPoint = c1; }
            if (distSqTo(c2) < best) {                       closestPoint = c2; }
        }

        Vector<n> delta = sphere.pos - closestPoint;
        float distSq = 0.f;
        for (int i = 0; i < n; ++i) distSq += delta[i]*delta[i];
        if (distSq > sphere.radius * sphere.radius) return col;

        float d = std::sqrt(distSq);
        col.penetration = sphere.radius - d;
        if (d > 1e-6f)
            for (int i = 0; i < n; ++i) col.normal[i] = delta[i] / d;
        else
            col.normal = normal;

        col.contactPoint = closestPoint;
        col.valid = true;
        return col;
    }

    template<int n>
    Collision<n> sphereVsShape(const PhysicsBody<n>& sphere, const Shape<n>* shape) const
    {
        Collision<n> deepest;
        deepest.penetration = -1e9f;

        float* raw = shape->getPoints();
        int num = shape->getNumPoints();

        for (int i = 0; i < num; i += 3)
        {
            Vector<n> A, B, C;
            for (int k = 0; k < n; ++k) {
                A[k] = raw[(i + 0) * n + k];
                B[k] = raw[(i + 1) * n + k];
                C[k] = raw[(i + 2) * n + k];
            }
            Collision<n> col = sphereVsTriangle(sphere, A, B, C);
            if (col.valid && col.penetration > deepest.penetration) {
                deepest = col;
                deepest.otherPType = shape->getPhysicsType();
            }
        }

        delete[] raw;
        return deepest;
    }


    template<int n>
    void resolveCollision(PhysicsBody<n>& a, PhysicsBody<n>& b, const Collision<n>& col)
    {
        if (!col.valid || col.penetration <= 0) return;

        if (col.otherPType == PhysicsType::WATER)
        {
            if constexpr (n == 2)
                a.setVel2D(a.vel2D() * 0.7f);
            else
                a.vel = a.vel * 0.7f;
            return;
        }

        bool bIsDynamic = (col.otherPType == PhysicsType::BALL);

        if constexpr (n == 2)
        {
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
            else
            {
                a.pos[0] += col.normal[0] * (col.penetration + 0.001f);
                a.pos[1] += col.normal[1] * (col.penetration + 0.001f);
            }

            Vector<2> norm2 = {col.normal[0], col.normal[1]};
            Vector<2> velA  = a.vel2D();
            Vector<2> velB  = b.vel2D();
            Vector<2> relVel = velA - velB;
            float vDotN = relVel * norm2;
            if (vDotN >= 0) return;

            float e          = bIsDynamic ? std::min(a.restitution, b.restitution) : a.restitution;
            float invMassSum = bIsDynamic ? (1.f/a.mass + 1.f/b.mass) : (1.f/a.mass);
            float j          = -(1.f + e) * vDotN / invMassSum;

            Vector<2> impulse2 = norm2 * j;
            a.setVel2D(velA + impulse2 * (1.f / a.mass));
            if (bIsDynamic) b.setVel2D(velB - impulse2 * (1.f / b.mass));

            velA = a.vel2D();
            velB = bIsDynamic ? b.vel2D() : Vector<2>{0, 0};
            Vector<2> tangent2 = {-norm2[1], norm2[0]};
            float vTan = (velA - velB) * tangent2;
            Vector<2> frictionImpulse = tangent2 * (vTan * 0.15f * 0.5f);
            a.setVel2D(velA - frictionImpulse);
            if (bIsDynamic) b.setVel2D(velB + frictionImpulse);
        }
        else
        {
            if (bIsDynamic)
            {
                float totalMass = a.mass + b.mass;
                float aShare = b.mass / totalMass;
                float bShare = a.mass / totalMass;
                for (int i = 0; i < n; ++i)
                {
                    a.pos[i] += col.normal[i] * (col.penetration + 0.001f) * aShare;
                    b.pos[i] -= col.normal[i] * (col.penetration + 0.001f) * bShare;
                }
            }
            else
            {
                for (int i = 0; i < n; ++i)
                    a.pos[i] += col.normal[i] * (col.penetration + 0.001f);
            }

            // Relative velocity along normal
            Vector<n> velA   = a.vel;
            Vector<n> velB   = bIsDynamic ? b.vel : Vector<n>{};
            Vector<n> relVel = velA - velB;

            float vDotN = 0.f;
            for (int i = 0; i < n; ++i) vDotN += relVel[i] * col.normal[i];
            if (vDotN >= 0) return;
// Grounded check — surface normal pointing mostly up means resting on a floor
if (col.normal[1] > 0.7f)
{
    a.grounded = true;
    a.vel[1] = 0.0f;
    vDotN = 0.f;  // recalc would be 0, just return
    return;
}

            float e          = bIsDynamic ? std::min(a.restitution, b.restitution) : a.restitution;
            float invMassSum = bIsDynamic ? (1.f/a.mass + 1.f/b.mass) : (1.f/a.mass);
            float j          = -(1.f + e) * vDotN / invMassSum;

            for (int i = 0; i < n; ++i)
                a.vel[i] += col.normal[i] * j / a.mass;
            if (bIsDynamic)
                for (int i = 0; i < n; ++i)
                    b.vel[i] -= col.normal[i] * j / b.mass;


            // Friction
            velA = a.vel;
            velB = bIsDynamic ? b.vel : Vector<n>{};
            relVel = velA - velB;

            // Tangent = relVel - (relVel.n)n
            float rDotN = 0.f;
            for (int i = 0; i < n; ++i) rDotN += relVel[i] * col.normal[i];
            Vector<n> tangent = relVel;
            for (int i = 0; i < n; ++i) tangent[i] -= rDotN * col.normal[i];

            float tLen = 0.f;
            for (int i = 0; i < n; ++i) tLen += tangent[i] * tangent[i];
            tLen = std::sqrt(tLen);
            if (tLen < 1e-6f) return;
            for (int i = 0; i < n; ++i) tangent[i] /= tLen;

            float vTan = 0.f;
            for (int i = 0; i < n; ++i) vTan += relVel[i] * tangent[i];

            float frictionJ = vTan * 0.15f * 0.5f;
            for (int i = 0; i < n; ++i)
                a.vel[i] -= tangent[i] * frictionJ;
            if (bIsDynamic)
                for (int i = 0; i < n; ++i)
                    b.vel[i] += tangent[i] * frictionJ;
        }
    }
};

#endif
