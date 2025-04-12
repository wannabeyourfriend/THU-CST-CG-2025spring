#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


// TODO: Implement Group - add data structure to store a list of Object*
class Group : public Object3D {

public:

    Group() {
        objects = std::vector<Object3D*>();
    }

    explicit Group (int num_objects) {
        objects.resize(num_objects, nullptr);
    }

    ~Group() override {
        
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

private:
    std::vector<Object3D*> objects;
};

#endif
	
