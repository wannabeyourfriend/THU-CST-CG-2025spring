#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>

/**
 * @brief Base camera class defining common camera functionality
 */
class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up).normalized();
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

/**
 * @brief Perspective camera implementation with field of view
 */
class PerspectiveCamera : public Camera {

public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
            const Vector3f &up, int imgW, int imgH, float angle) : Camera(center, direction, up, imgW, imgH) {
        // angle is in radian.
        this->angle = angle;
        this->tanAngle = tan(angle / 2);
    }

    /**
     * @brief Generate a ray from camera through the specified screen point
     * @param point Screen space coordinates
     * @return Ray from camera origin through the screen point
     */
    Ray generateRay(const Vector2f &point) override {
        // Convert pixel coordinates to normalized device coordinates (NDC)
        float x = (2.0f * point.x() / width - 1) * tanAngle * width / height;
        float y = -(1 - 2.0f * point.y() / height) * tanAngle;
        
        // Calculate ray direction in world space
        Vector3f rayDir = direction + x * horizontal + y * up;
        rayDir = rayDir.normalized();
        
        // Create and return the ray
        return Ray(center, rayDir);
    }

private:
    float angle;      // Field of view in radians
    float tanAngle;   // Precomputed tan(angle/2) for efficiency
};

#endif //CAMERA_H
