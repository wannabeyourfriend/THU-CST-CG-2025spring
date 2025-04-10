#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
public:
    Plane() {
        normal = Vector3f(0, 0, 1);
        d = 0;
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->normal = normal.normalized();
        this->d = d;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float denominator = Vector3f::dot(normal, r.getDirection());
        
        // 如果光线方向与平面法线平行或几乎平行，则不相交
        if (fabs(denominator) < 1e-8) {
            return false;
        }
        
        // 计算交点参数 t
        float t = (d - Vector3f::dot(normal, r.getOrigin())) / denominator;
        
        // 如果 t 在有效范围内，则有交点
        if (t >= tmin && t < h.getT()) {
            h.set(t, material, normal);
            return true;
        }
        
        return false;
    }

protected:
    Vector3f normal;  // 平面法线
    float d;          // 平面方程中的常数项
};

#endif //PLANE_H
		

