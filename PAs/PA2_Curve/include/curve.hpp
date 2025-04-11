#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>

#include <algorithm>

// TODO (PA2): Implement Bernstein class to compute spline basis function.
//       You may refer to the python-script for implementation.

class Bernstein {   
public:
    static float basisFunction(int i, int n, float t) {
        return binomialCoeff(n, i) * pow(t, i) * pow(1 - t, n - i);
    }
    
private:
    //  DP 实现组合数
    static int binomialCoeff(int n, int k) {
        if (k < 0 || k > n) return 0;
        if (k == 0 || k == n) return 1;
        
        int result = 1;
        for (int i = 0; i < k; ++i) {
            result *= (n - i);
            result /= (i + 1);
        }
        return result;
    }
};

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
};

class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;
public:
    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return false;
    }

    std::vector<Vector3f> &getControls() {
        return controls;
    }

    virtual void discretize(int resolution, std::vector<CurvePoint>& data) = 0;

    void drawGL() override {
        Object3D::drawGL();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        glPointSize(4);
        glBegin(GL_POINTS);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        std::vector<CurvePoint> sampledPoints;
        discretize(30, sampledPoints);
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (auto & cp : sampledPoints) { glVertex3fv(cp.V); }
        glEnd();
        glPopAttrib();
    }
};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            exit(0);
        }
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        
        int numSegments = (controls.size() - 1) / 3;
        
        for (int segment = 0; segment < numSegments; ++segment) {
            int baseIndex = segment * 3;
            Vector3f p0 = controls[baseIndex];
            Vector3f p1 = controls[baseIndex + 1];
            Vector3f p2 = controls[baseIndex + 2];
            Vector3f p3 = controls[baseIndex + 3];
            
            for (int i = 0; i <= resolution; ++i) {
                if (i == resolution && segment < numSegments - 1) {
                    continue;
                }
                
                float t = (float)i / resolution;
                
                CurvePoint cp;
                
                float b0 = Bernstein::basisFunction(0, 3, t);
                float b1 = Bernstein::basisFunction(1, 3, t);
                float b2 = Bernstein::basisFunction(2, 3, t);
                float b3 = Bernstein::basisFunction(3, 3, t);
                
                cp.V = p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3;
                
                Vector3f tangent = (p1 - p0) * 3 * (1-t)*(1-t) + 
                                  (p2 - p1) * 6 * (1-t)*t + 
                                  (p3 - p2) * 3 * t*t;
                
                if (tangent.length() > 1e-6) {
                    cp.T = tangent.normalized();
                } else {
                    if (i > 0) {
                        cp.T = (cp.V - data.back().V).normalized();
                    } else if (segment > 0) {
                        cp.T = (cp.V - data.back().V).normalized();
                    } else {
                        cp.T = Vector3f(1, 0, 0); 
                    }
                }
                
                data.push_back(cp);
            }
        }
    }
};

class BsplineCurve : public Curve {
public:
    BsplineCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        
        int n = controls.size() - 1;  
        
        for (int i = 0; i <= n - 3; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                if (j == resolution && i < n - 3) {
                    continue;
                }
                
                float t = (float)j / resolution;
                
                CurvePoint cp;
                
                Vector3f p0 = controls[i];
                Vector3f p1 = controls[i + 1];
                Vector3f p2 = controls[i + 2];
                Vector3f p3 = controls[i + 3];
                
                float b0 = (-t*t*t + 3*t*t - 3*t + 1) / 6.0f;
                float b1 = (3*t*t*t - 6*t*t + 4) / 6.0f;
                float b2 = (-3*t*t*t + 3*t*t + 3*t + 1) / 6.0f;
                float b3 = (t*t*t) / 6.0f;
                
                cp.V = p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3;
                
                float d0 = (-3*t*t + 6*t - 3) / 6.0f;
                float d1 = (9*t*t - 12*t) / 6.0f;
                float d2 = (-9*t*t + 6*t + 3) / 6.0f;
                float d3 = (3*t*t) / 6.0f;
                
                Vector3f tangent = p0 * d0 + p1 * d1 + p2 * d2 + p3 * d3;
                if (tangent.length() > 1e-6) {
                    cp.T = tangent.normalized();
                } else {
                    if (j > 0) {
                        cp.T = (cp.V - data.back().V).normalized();
                    } else if (i > 0) {
                        cp.T = (cp.V - data.back().V).normalized();
                    } else {
                        cp.T = Vector3f(1, 0, 0); // 默认切线
                    }
                }
                
                data.push_back(cp);
            }
        }
    }
};

#endif // CURVE_HPP
