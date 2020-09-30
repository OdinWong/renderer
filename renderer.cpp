#include "geometry.h"
#include "renderables.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>


struct Light {
    Light(const Vec3f &p, const float &i) : position(p), intensity(i) {}
    Vec3f position;
    float intensity;
};

bool scene_intersect(const  Vec3f &orig, const Vec3f &dir, const std::vector<RenderableObject *> &objects, Vec3f &hit, Vec3f &N, Material &material) {
    //init object distance to maximum
    float obj_dist = std::numeric_limits<float>::max();
    //check each object
    for (size_t i = 0; i < objects.size(); i++) {
        float dist_i;
        //if ray intersects with the object and the distance is less than the current closest
        if(objects[i]->ray_intersect(orig, dir, dist_i) && dist_i < obj_dist){
            obj_dist = dist_i;
            //get vector to the intersection point
            hit = orig + dir*dist_i;
            //surface normal at intersection point
            N = (hit - objects[i]->get_center()).normalize();
            material = objects[i]->get_material();
        }
    }
    //consider an intersection if distance is less than 1000 -- subject to change
    return obj_dist<1000;
    
}

Vec3f reflect(const Vec3f &I, const Vec3f &N) {
    return I - N*2.f*(I*N);
}

Vec3f cast_ray(const Vec3f &orig, const Vec3f &dir, const std::vector<RenderableObject *> &objects, std::vector<Light> lights, size_t depth = 4) {
    Vec3f point, N;
    Material material;
    Vec3f ambient = Vec3f(0.1, 0.1, 0.2);
    

    if (depth <= 0 || !scene_intersect(orig, dir, objects, point, N, material)) {
        return ambient;
    }

    Vec3f reflect_dir = reflect(dir,N).normalize();
    Vec3f refelct_orig = reflect_dir*N < 0 ? point + N*1e-3 : point + N*1e-3;
    Vec3f reflect_colour = cast_ray(refelct_orig, reflect_dir, objects, lights, depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    for (size_t i = 0; i < lights.size(); i++) {
        Vec3f light_dir = (lights[i].position - point).normalize();
        float light_distance = (lights[i].position - point).norm();
        
        Vec3f shadow_orig = light_dir*N < 0 ? point - N*1e-3 : point + N*1e-3;
        Vec3f shadow_pt, shadow_N;
        Material tempmaterial;       
        if (scene_intersect(shadow_orig, light_dir, objects, shadow_pt, shadow_N, tempmaterial) && (shadow_pt-shadow_orig).norm() < light_distance)
            continue;

         specular_light_intensity += powf(std::max(0.f, -reflect(-light_dir, N)*dir), material.get_spec())*lights[i].intensity;
         diffuse_light_intensity += lights[i].intensity * std::max(0.f, light_dir*N);
    }
    

    return material.get_diffuse()*diffuse_light_intensity*material.get_albedo()[0] + Vec3f(1.,1.,1.)*specular_light_intensity*material.get_albedo()[1] + reflect_colour*material.get_albedo()[2];
}

void render(const std::vector<RenderableObject *> &objects, std::vector<Light> lights) {
    const int width = 1024;
    const int height = 768;
    const int fov = M_PI*(2/3.);
    std::vector<Vec3f> buffer(width*height);

    //render
    for (size_t j = 0; j<height; j++) {
        for (size_t i = 0; i<width; i++) {
            float x =  (2*(i + 0.5)/(float)width  - 1)*tan(fov/2.)*width/(float)height;
            float y = -(2*(j + 0.5)/(float)height - 1)*tan(fov/2.);
            Vec3f dir = Vec3f(x, y, -1).normalize();
            buffer[i+j*width] = cast_ray(Vec3f(0,0,0), dir, objects, lights, 4);
        }
    }

    //output ppm file
    std::ofstream outfile;
    outfile.open("./out.ppm");
    outfile << "P6\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height*width; i++) {
        for (size_t j = 0; j < 3; j++) {
            outfile << (char)(255 * std::max(0.f, std::min(1.f, buffer[i][j])));
        }
        
    }
    outfile.close();

}



int main(int argc, char const *argv[]) {
    std::vector<RenderableObject *> objects;
    
    Vec3f blue(0.1,0.1,0.8);
    Vec3f green(0.9, 1, 0.9);
    Vec3f red(0.8,0.1,0.1);

    Material      ivory(green, Vec3f(0.2,  10., 1.), 1500);
    Material red_rubber(blue, Vec3f(0.9,  1, 0.1), 100.);
    Material red_thingo(red, Vec3f(0.4, 0.3, 0.1), 100);
    
    objects.push_back(new Sphere(Vec3f(5,5,-8), 2, ivory));
    objects.push_back(new Sphere(Vec3f(-10,-10,10), 10, red_rubber));
    objects.push_back(new Sphere(Vec3f(0,0,-20), 10, red_rubber));
    objects.push_back(new Sphere(Vec3f(0,-20,-10), 30, red_thingo));


    std::vector<Light> lights;
    lights.push_back(Light(Vec3f(10,20,-5), 1.5));
    lights.push_back(Light(Vec3f(-10,20,-5), 1));


    render(objects, lights);

    for (size_t i = 0; i < objects.size(); i++)
    {
        free(objects[i]);
    }
    
    return 0;
    
}
