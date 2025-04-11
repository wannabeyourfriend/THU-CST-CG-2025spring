#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


class Group : public Object3D {

public:

    Group() {
        objects.clear();
    }

    explicit Group (int num_objects) {
        objects.resize(num_objects, nullptr);
    }

    ~Group() override {
        for (auto obj : objects) {
            delete obj;
        }
        objects.clear();
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool result = false;
        
        for (auto obj : objects) {
            if (obj && obj->intersect(r, h, tmin)) {
                result = true;
            }
        }
        
        return result;
    }

    void addObject(int index, Object3D *obj) {
        if (index >= 0 && index < objects.size()) {
            objects[index] = obj;
        }
    }

    int getGroupSize() {
        return objects.size();
    }
    
    void drawGL() override {
        Object3D::drawGL();
        for (auto obj : objects) {
            if (obj) {
                obj->drawGL();
            }
        }
    }

private:
    std::vector<Object3D*> objects;
};

#endif
	
