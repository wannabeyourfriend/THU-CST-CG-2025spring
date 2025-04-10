#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>

/**
 * @brief Material class implementing the Phong reflection model.
 * This class handles the shading computation for objects in the scene,
 * incorporating both diffuse and specular reflections.
 */
class Material {
public:
    /**
     * @brief Constructs a material with diffuse and specular properties
     * @param d_color The diffuse color of the material
     * @param s_color The specular color of the material (defaults to black)
     * @param s The shininess exponent for specular reflection (defaults to 0)
     */
    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0) :
            diffuseColor(d_color), specularColor(s_color), shininess(s) {
    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }

    /**
     * @brief Computes the shading at a point using the Phong reflection model
     * 
     * @param ray The incoming ray that hit the surface
     * @param hit The hit information at the intersection point
     * @param dirToLight The direction vector from intersection point to light source
     * @param lightColor The color/intensity of the light source
     * @return Vector3f The computed color at the intersection point
     */
    Vector3f Shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor) {
        Vector3f shaded = Vector3f::ZERO;
        
        // Get the surface normal at intersection point
        Vector3f N = hit.getNormal();
        
        // Calculate diffuse term using Lambert's cosine law
        // diffuseTerm = clamp(L·N) = max(0, L·N)
        float diffuseTerm = std::max(0.0f, Vector3f::dot(dirToLight, N));
        
        // Calculate specular reflection
        // Compute reflection vector R = 2(L·N)N - L
        Vector3f L = dirToLight;
        Vector3f R = (2.0f * Vector3f::dot(L, N) * N - L).normalized();
        
        // Calculate view vector (from intersection point to camera)
        Vector3f V = -ray.getDirection().normalized();
        
        // Calculate specular term using Phong model
        // specularTerm = clamp(V·R)^shininess = max(0, V·R)^shininess
        float specularTerm = std::pow(std::max(0.0f, Vector3f::dot(V, R)), shininess);
        
        // Combine diffuse and specular components
        // Note: Ambient term is typically handled separately in the main rendering loop
        shaded = lightColor * diffuseColor * diffuseTerm + 
                 lightColor * specularColor * specularTerm;
        
        return shaded;
    }

protected:
    Vector3f diffuseColor;   // The diffuse color component of the material
    Vector3f specularColor;  // The specular color component of the material
    float shininess;        // The shininess exponent for specular reflection
};

#endif // MATERIAL_H
