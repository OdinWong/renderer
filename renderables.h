#ifndef __RENDERABLEOBJECT_H__
#define __RENDERABLEOBJECT_H__
#include "geometry.h"

//material class
class Material {
    private:
    Vec3f diffuse_colour;
    Vec3f albedo;
    float spec_exponent;
    public:
    Material(): diffuse_colour(Vec3f(0.5, 0.5, 0.5)), albedo(Vec3f(1,0,0)), spec_exponent(1) {}
    Material(const Vec3f &colour) : diffuse_colour(colour), albedo(Vec3f(1,0,0)), spec_exponent(1) {}
    Material(const Vec3f &colour, const Vec3f &albedo) : diffuse_colour(colour), albedo(albedo), spec_exponent(1) {}
    Material(const Vec3f &colour, const Vec3f &albedo, const float &s) : diffuse_colour(colour), albedo(albedo), spec_exponent(s) {}

    Vec3f get_diffuse() { return diffuse_colour; }
    Vec3f get_albedo() { return albedo; }
    float get_spec() { return spec_exponent; }
};

//abstract base class for all renderables
class RenderableObject {
    protected:
    Vec3f center;
    Material material;

    public:
    RenderableObject(const Vec3f &c, const Material &m);

    virtual bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const = 0;
    Vec3f get_center() const;
    Material get_material() const;
};

//renderable sphere
class Sphere: public RenderableObject {
    private:
    float radius;

    public:
    Sphere(const Vec3f &c, const float &r, const Material &m);

    
    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const override;
    float get_radius();
};

/*plane
class Plane: public RenderableObject {
    private:
    Vec3f face_normal;
    float width;
    float depth;
    
    public:
    Plane( const Vec3f &c, const Material &m, const Vec3f face_normal, const size_t w, const size_t d);
    
    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const override;
};*/

#endif //__RENDERABLEOBJECT_H__