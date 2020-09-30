#include "geometry.h"
#include "renderables.h"
#include <vector>
#include <limits>

/*******RenderableObject*******/
RenderableObject::RenderableObject(const Vec3f &c, const Material &m) : center(c), material(m) {}

Vec3f RenderableObject::get_center() const {return center; }
Material RenderableObject::get_material() const { return material; }

/********sphere********/

Sphere::Sphere(const Vec3f &c, const float &r, const Material &m) : RenderableObject(c,m), radius(r) {}

//ray intersect check for the sphere
bool Sphere::ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const{
    //find vector origin -> center of circle
    Vec3f L = center - orig;
    //dot product to get projection of center onto ray (tca)
    float tca = L*dir;
    //pythag to get vector center -> tca
    float d2 = L*L - tca*tca;
    // case one: no intersection
    if(d2 > radius*radius) return false;
    //get center -> first intersect point
    float thc = sqrtf(radius*radius - d2);
    //first intersect 
    t0 = tca - thc;
    // second intersect
    float t1 = tca + thc;
    //if first intersect behind origin, try second
    if(t0 < 0) t0 = t1;
    //second also behind, no intersect in front
    if(t0 < 0) return false;
    return true;
}

/********Plane**********/
//Plane::Plane(const Vec3f &c, const Material &m, const Vec3f face_N, const size_t w, const size_t d) : RenderableObject(c,m), face_normal(face_N), width(w), depth(d) {}

//bool Plane::ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const {

//}

float Sphere::get_radius() { return radius; }