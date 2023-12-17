// Reference: https://www.shadertoy.com/view/7tBXDh
#version 410 core

// ===================================================== Uniforms =====================================================
uniform sampler2D u_diffuseMap;
uniform vec2 u_resolution;
uniform float u_time;
uniform float u_camSpin;

// ======================================================== In ========================================================
vec2 fragCoord = gl_FragCoord.xy;

// ======================================================== Out ========================================================
out vec4 fragColor;

#define PI 3.14159265359
#define SAMPLES_PER_PIXEL 10.0
#define MAX_RAY_BOUNCES 6
#define SPHERE_COUNT 24

float rand12(vec2 p) {
	vec3 p3  = fract(vec3(p.xyx) * 0.1031);
	p3 += dot(p3, p3.yzx + 33.33);
	return fract((p3.x + p3.y) * p3.z);
}

vec2 rand22(vec2 p) {
	vec3 p3 = fract(vec3(p.xyx) * vec3 (0.1031, 0.1030, 0.0973));
	p3 += dot(p3, p3.yzx + 33.33);
	return fract((p3.xx + p3.yz) * p3.zy);
}

vec3 rand32(vec2 p) {
	vec3 p3 = fract(vec3(p.xyx) * vec3 (0.1031, 0.1030, 0.0973));
	p3 += dot(p3, p3.yxz + 33.33);
	return fract((p3.xxy + p3.yzz) * p3.zyx);
}

vec3 random_in_unit_sphere(vec2 p) {
	vec3 rand = rand32(p);
	float phi = 2.0 * PI * rand.x;
	float cosTheta = 2.0 * rand.y - 1.0;
	float u = rand.z;

	float theta = acos(cosTheta);
	float r = pow(u, 1.0 / 3.0);

	float x = r * sin(theta) * cos(phi);
	float y = r * sin(theta) * sin(phi);
	float z = r * cos(theta);

	return vec3(x, y, z);
}

vec3 random_unit_vector(vec2 p) {
	return normalize(random_in_unit_sphere(p));
}

vec3 random_in_unit_disk(vec2 p) {
	return vec3(random_in_unit_sphere(p).xy, 0);
}

const int material_lambertian = 0;
const int material_metal = 1;
const int material_dielectric = 2;

struct ray {
	vec3 origin;
	vec3 dir;
};

struct material {
	int type;
	vec3 albedo;
	float metal_fuzz;
	float dielectric_index_of_refraction;
};

struct hit_record {
	vec3 p;
	vec3 normal;
	float t;
	material material;
};

struct sphere {
	vec3 center;
	float radius;
	material material;
};

const sphere spheres[SPHERE_COUNT] = sphere[](
sphere(vec3( 0.0, -1000.0, -1.0), 1000.0, material(material_lambertian, vec3(0.5, 0.5, 0.5), 0.0, 0.0)),
sphere(vec3(-4.0, 1.0, 2.0), 	  1.0, 	  material(material_dielectric, vec3(0.0, 0.0, 0.0), 0.0, 1.5)),
sphere(vec3( 0.0, 1.0, 0.0), 	  1.0, 	  material(material_metal, 	    vec3(0.7, 0.6, 0.5), 0.0, 0.0)),
sphere(vec3( 4.0, 1.0, 2.0), 	  1.0, 	  material(material_lambertian, vec3(0.7, 0.3, 0.3), 0.0, 0.0)),
sphere(vec3(-6.0, 0.2, 2.8), 	  0.2, 	  material(material_dielectric, vec3(0.0, 0.0, 0.2), 0.0, 1.5)),
sphere(vec3(1.6, 0.2, -0.9), 	  0.2, 	  material(material_dielectric, vec3(0.0, 0.0, 0.0), 0.0, 1.5)),
sphere(vec3(-5.7, 0.2, -2.7), 	  0.2, 	  material(material_lambertian, vec3(0.8, 0.3, 0.3), 0.0, 0.0)),
sphere(vec3(-3.6, 0.2, -4.4), 	  0.2, 	  material(material_lambertian, vec3(0.9, 0.3, 0.2), 0.0, 0.0)),
sphere(vec3(0.8, 0.2, 2.3), 	  0.2, 	  material(material_lambertian, vec3(0.2, 0.0, 0.5), 0.0, 0.0)),
sphere(vec3(3.8, 0.2, 4.2), 	  0.2, 	  material(material_lambertian, vec3(0.4, 0.3, 0.7), 0.0, 0.0)),
sphere(vec3(-0.1, 0.2, -1.9), 	  0.2, 	  material(material_lambertian, vec3(0.4, 0.0, 0.4), 0.0, 0.0)),
sphere(vec3(-2.5, 0.2, 5.4), 	  0.2, 	  material(material_metal, 	    vec3(0.3, 0.7, 0.9), 0.3, 0.0)),
sphere(vec3(-3.9, 0.2, -0.3), 	  0.2, 	  material(material_lambertian, vec3(0.9, 0.8, 0.5), 0.0, 0.0)),
sphere(vec3(-6.0, 0.2, 4.0), 	  0.2, 	  material(material_lambertian, vec3(0.9, 0.9, 0.5), 0.0, 0.0)),
sphere(vec3(4.4, 0.2, -0.5), 	  0.2, 	  material(material_lambertian, vec3(0.5, 0.4, 0.8), 0.0, 0.0)),
sphere(vec3(3.4, 0.2, 5.3), 	  0.2, 	  material(material_lambertian, vec3(0.1, 0.6, 0.2), 0.0, 0.0)),
sphere(vec3(4.6, 0.2, -3.8), 	  0.2, 	  material(material_lambertian, vec3(0.2, 0.2, 0.2), 0.0, 0.0)),
sphere(vec3(0.7, 0.2, -2.5), 	  0.2, 	  material(material_metal, 	    vec3(0.0, 0.2, 0.1), 0.0, 0.0)),
sphere(vec3(2.4, 0.2, -4.3), 	  0.2, 	  material(material_lambertian, vec3(0.8, 0.9, 0.0), 0.0, 0.0)),
sphere(vec3(4.4, 0.2, 4.9), 	  0.2, 	  material(material_lambertian, vec3(0.8, 0.8, 0.0), 0.0, 0.0)),
sphere(vec3(-4.7, 0.2, 4.6), 	  0.2, 	  material(material_lambertian, vec3(0.8, 0.8, 0.7), 0.0, 0.0)),
sphere(vec3(4.2, 0.2, -3.5), 	  0.2, 	  material(material_lambertian, vec3(0.8, 0.8, 0.6), 0.0, 0.0)),
sphere(vec3(-5.2, 0.2, 0.5), 	  0.2, 	  material(material_lambertian, vec3(0.2, 0.7, 0.9), 0.0, 0.0)),
sphere(vec3(5.7, 0.2, -0.8), 	  0.2, 	  material(material_lambertian, vec3(0.3, 0.0, 0.7), 0.0, 0.0))
);

void hit_sphere(sphere sph, ray r, inout hit_record rec, inout bool hit_anything) {
	float closest_so_far = rec.t;
	vec3 oc = r.origin - sph.center;
	float a = dot(r.dir, r.dir);
	float half_b = dot(oc, r.dir);
	float c = dot(oc, oc) - sph.radius * sph.radius;
	float discriminant = half_b * half_b - a * c;
	if (discriminant < 0.0) {
		return;
	}

	float sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range
	float root = (-half_b - sqrtd) / a; // the t.0 from -b - sqrt(dis) / 2a
	if (root < 0.001 || closest_so_far < root) {
		root = (-half_b + sqrtd) / a;
		if (root < 0.001 || closest_so_far < root) {
			return;
		}
	}

	hit_anything = true;
	vec3 p = r.origin + r.dir * root;
	rec = hit_record(p, (p - sph.center) / sph.radius, root, sph.material);
}

bool hit(ray r, out hit_record rec) {
	bool hit = false;
	// Set initial hit distance to max
	rec = hit_record(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 9999.0, material(material_lambertian, vec3(0.0, 0.0, 0.0), 0.0, 0.0));

	// unrolling this loop gave 4x perf boost...
	hit_sphere(spheres[0],  r, rec, hit);
	hit_sphere(spheres[1],  r, rec, hit);
	hit_sphere(spheres[2],  r, rec, hit);
	hit_sphere(spheres[3],  r, rec, hit);
	hit_sphere(spheres[4],  r, rec, hit);
	hit_sphere(spheres[5],  r, rec, hit);
	hit_sphere(spheres[6],  r, rec, hit);
	hit_sphere(spheres[7],  r, rec, hit);
	hit_sphere(spheres[8],  r, rec, hit);
	hit_sphere(spheres[9],  r, rec, hit);
	hit_sphere(spheres[10], r, rec, hit);
	hit_sphere(spheres[11], r, rec, hit);
	hit_sphere(spheres[12], r, rec, hit);
	hit_sphere(spheres[13], r, rec, hit);
	hit_sphere(spheres[14], r, rec, hit);
	hit_sphere(spheres[15], r, rec, hit);
	hit_sphere(spheres[16], r, rec, hit);
	hit_sphere(spheres[17], r, rec, hit);
	hit_sphere(spheres[18], r, rec, hit);
	hit_sphere(spheres[19], r, rec, hit);
	hit_sphere(spheres[21], r, rec, hit);
	hit_sphere(spheres[22], r, rec, hit);
	hit_sphere(spheres[23], r, rec, hit);

	return hit;
}

bool near_zero(vec3 p) {
	float s = 1e-8;
	return p.x < s && p.y < s && p.z < s;
}

float reflectance(float cosine, float ref_idx) {
	// Use Schlick's approximation for reflectance
	float r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.);
}

void scatter(hit_record rec, ray r, vec2 seed, inout vec3 attenuation, inout ray scattered) {
	material m = rec.material;

	if (m.type == material_lambertian) {
		vec3 scatter_direction = normalize(rec.normal + random_unit_vector(seed));

		// catch degenerate scatter direction
		if (near_zero(scatter_direction)) {
			scatter_direction = rec.normal;
		}

		scattered = ray(rec.p, scatter_direction);
		attenuation = m.albedo;
	} else if (m.type == material_metal) {
		vec3 reflected = reflect(r.dir, rec.normal);
		ray scattered_ = ray(rec.p, normalize(reflected + m.metal_fuzz * random_in_unit_sphere(seed)));
		if (dot(scattered_.dir, rec.normal) > 0.0) {
			scattered = scattered_;
			attenuation = m.albedo;
		}
	} else if (m.type == material_dielectric) {
		bool front_face = dot(r.dir, rec.normal) < 0.;
		vec3 adjusted_normal = front_face ? rec.normal : -rec.normal;
		float ref = m.dielectric_index_of_refraction;
		float refraction_ratio = front_face ? 1.0/ref : ref;

		float cos_theta = min(dot(-r.dir, adjusted_normal), 1.0);
		float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > rand12(seed)) {
			direction = reflect(r.dir, adjusted_normal);
		} else {
			direction = refract(r.dir, adjusted_normal, refraction_ratio);
		}

		scattered = ray(rec.p, direction);
		attenuation = vec3(1);
	}
}

vec3 ray_color(in ray r, vec2 seed) {
	vec3 color = vec3(1.0, 1.0, 1.0);
	hit_record rec;
	int depth;
	for (depth = 0; depth < MAX_RAY_BOUNCES; depth++) {
		if (hit(r, rec)) {
			ray scattered;
			vec3 attenuation;
			scatter(rec, r, seed * 999.0 + float(depth), attenuation, scattered);
			r = scattered;
			color *= attenuation;
		} else {
			// hit bg, aka nothing
			float t = 0.5 * (r.dir.y + 1.0);
			color *= mix(vec3(1.0, 1.0, 1.0), vec3(0.5, 0.7, 1.0), t);
			break;
		}
	}
	if (depth == MAX_RAY_BOUNCES) {
		return vec3(0.0, 0.0, 0.0);
	}
	return color;
}


void main()
{
	// camera
	vec3 lookfrom = vec3(cos(u_time * u_camSpin) * 13.0, 2.0, sin(u_time * u_camSpin) * 10.0);
	vec3 lookat = vec3(0, 0, 0);
	vec3 vup = vec3(0, 1, 0);
	float vfov = 30.0;
	float aspect_ratio = u_resolution.x / u_resolution.y;
	float aperture = 0.1;
	float focus_dist = 10.0;

	float theta = radians(vfov);
	float h = tan(theta / 2.0);
	float viewport_height = 2.0 * h;
	float viewport_width = aspect_ratio * viewport_height;

	vec3 w = normalize(lookfrom - lookat);
	vec3 u = normalize(cross(vup, w));
	vec3 v = cross(w, u);

	vec3 origin = lookfrom;
	vec3 horizontal = focus_dist * viewport_width * u;
	vec3 vertical = focus_dist * viewport_height * v;
	vec3 lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - focus_dist * w;

	float lens_radius = aperture / 2.0;

	// render
	vec3 color = vec3(0);
	for (float s = 0.0; s < SAMPLES_PER_PIXEL; s++) {
		vec2 rand = rand22(fragCoord * 999.0 + s + u_time);

		vec2 normalizedCoord = (fragCoord + rand) / u_resolution.xy;
		vec3 rd = lens_radius * random_in_unit_disk(normalizedCoord * 999.0 + s + u_time);
		vec3 offset = u * rd.x + v * rd.y;
		ray r = ray(
			origin + offset,
			normalize(lower_left_corner + normalizedCoord.x * horizontal + normalizedCoord.y * vertical - origin - offset)
		);
		color += ray_color(r, normalizedCoord);
	}
	fragColor = vec4(sqrt(color / SAMPLES_PER_PIXEL), 1.0);
}
