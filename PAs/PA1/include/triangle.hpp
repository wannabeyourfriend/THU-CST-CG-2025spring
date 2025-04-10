#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

/**
 * @brief Triangle class representing a triangle in 3D space
 * Implements intersection testing using Möller–Trumbore algorithm
 */
class Triangle: public Object3D {

public:
    Triangle() = delete;

    /**
     * @brief Construct a triangle from three vertices
     * @param a First vertex position
     * @param b Second vertex position
     * @param c Third vertex position
     * @param m Material of the triangle
     */
    Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        
        // Calculate triangle normal using cross product
        Vector3f edge1 = b - a;
        Vector3f edge2 = c - a;
        normal = Vector3f::cross(edge1, edge2).normalized();
    }

    /**
     * @brief Ray-triangle intersection test using Möller–Trumbore algorithm
     * @param ray The ray to test intersection with
     * @param hit Hit record to store intersection information
     * @param tmin Minimum valid intersection distance
     * @return true if intersection found, false otherwise
     */
    bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
        // Implementation of Möller–Trumbore algorithm
        Vector3f edge1 = vertices[1] - vertices[0];
        Vector3f edge2 = vertices[2] - vertices[0];
        Vector3f pvec = Vector3f::cross(ray.getDirection(), edge2);
        float det = Vector3f::dot(edge1, pvec);
        
        // Ray is parallel to triangle if determinant is close to 0
        if (fabs(det) < 1e-8) return false;
        
        float invDet = 1.0f / det;
        
        // Calculate first barycentric coordinate (u)
        Vector3f tvec = ray.getOrigin() - vertices[0];
        float u = Vector3f::dot(tvec, pvec) * invDet;
        
        // Check if u is within valid range [0,1]
        if (u < 0.0f || u > 1.0f) return false;
        
        // Calculate second barycentric coordinate (v)
        Vector3f qvec = Vector3f::cross(tvec, edge1);
        float v = Vector3f::dot(ray.getDirection(), qvec) * invDet;
        
        // Check if v is within valid range and u+v <= 1
        if (v < 0.0f || u + v > 1.0f) return false;
        
        // Calculate intersection parameter t
        float t = Vector3f::dot(edge2, qvec) * invDet;
        
        // Check if intersection is within valid range
        if (t >= tmin && t < hit.getT()) {
            hit.set(t, material, normal);
            return true;
        }
        
        return false;
    }
    Vector3f normal;      // Triangle normal vector
    Vector3f vertices[3]; // Array storing three vertices

protected:

};

#endif //TRIANGLE_H
