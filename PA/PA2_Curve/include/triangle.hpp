#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;

class Triangle: public Object3D
{

public:
	Triangle() = delete;
        ///@param a b c are three vertex positions of the triangle
        

	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        
        // Calculate triangle normal using cross product
        Vector3f edge1 = b - a;
        Vector3f edge2 = c - a;
        normal = Vector3f::cross(edge1, edge2).normalized();
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
        return false;
	}
	Vector3f normal;
	Vector3f vertices[3];

    void drawGL() override {
        Object3D::drawGL();
        glBegin(GL_TRIANGLES);
        glNormal3fv(normal);
        glVertex3fv(vertices[0]); glVertex3fv(vertices[1]); glVertex3fv(vertices[2]);
        glEnd();
    }

protected:
};

#endif //TRIANGLE_H
