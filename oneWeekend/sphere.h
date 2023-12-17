#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable{
	public:
		sphere(point3 _center, double _radius, shared_ptr<material> _material) : center(_center), radius(_radius), mat(_material) {}
		
		bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
			vec3 oc = r.origin() - center;
			auto a = r.direction().length_squared();
			auto h_b = dot(oc, r.direction());
			auto c = oc.length_squared() - radius*radius;
			auto disc = h_b*h_b - a*c;

			if(disc<0) return false;
			auto sqrtd = sqrt(disc);

			auto root = (-h_b - sqrtd)/a;

			if(!ray_t.surrounds(root)){
				root = (-h_b + sqrtd)/a;
				if(!ray_t.surrounds(root)){
					return false;
				}
			}
			rec.t = root;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			rec.mat = mat;

			return true;
		}
	private:
		point3 center;
		double radius;
		shared_ptr<material> mat;
};

#endif
