#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

/**
 * @brief Sphere class representing a 3D sphere
 * Implements ray-sphere intersection using quadratic equation
 */
class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
        center = Vector3f(0, 0, 0);
        radius = 1.0f;
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        this->center = center;
        this->radius = radius;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        // Compute ray-sphere intersection using quadratic equation
        Vector3f oc = r.getOrigin() - center;
        float a = r.getDirection().squaredLength();
        float b = 2.0f * Vector3f::dot(oc, r.getDirection());
        float c = oc.squaredLength() - radius * radius;
        float discriminant = b * b - 4 * a * c;
        
        if (discriminant < 0) {
            return false;
        }
        
        float sqrtDiscriminant = sqrt(discriminant);
        
        // Calculate nearest intersection point
        float t = (-b - sqrtDiscriminant) / (2.0f * a);
        
        // If nearest intersection is invalid, try the farther one
        if (t < tmin) {
            t = (-b + sqrtDiscriminant) / (2.0f * a);
            if (t < tmin) {
                return false;
            }
        }
        
        // If intersection is closer than current closest hit
        if (t < h.getT()) {
            Vector3f intersectionPoint = r.pointAtParameter(t);
            Vector3f normal = (intersectionPoint - center).normalized();
            h.set(t, material, normal);
            return true;
        }
        
        return false;
    }

protected:
    Vector3f center;  // Center position of the sphere
    float radius;     // Radius of the sphere
};

#endif
