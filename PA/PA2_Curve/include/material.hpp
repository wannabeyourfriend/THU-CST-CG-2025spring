#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <glut.h>

class Material {
public:

    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0) :
            diffuseColor(d_color), specularColor(s_color), shininess(s) {

    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }


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

    // For OpenGL, this is fully implemented
    void Use() {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Vector4f(diffuseColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Vector4f(specularColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Vector2f(shininess * 4.0, 1.0f));
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
};


#endif // MATERIAL_H
