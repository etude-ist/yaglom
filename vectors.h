#include <math.h>
#include <stdio.h>

// 3D vectors

typedef struct {
  float x;
  float y;
  float z;
} vec_3d;

void vec3_print(const vec_3d v) {
  printf("vec_3d: x=%f, y=%f, z=%f\n", v.x, v.y, v.z);
}

static inline vec_3d vec3(float x, float y, float z) {
  return (vec_3d){x, y, z};
}

static inline float vec3_length(const vec_3d v) {
  return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

static inline float vec3_squared_length(const vec_3d v) {
  return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline void vec3_make_unit_vector(vec_3d *v) {
  float k = 1.0 / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
  v->x *= k;
  v->y *= k;
  v->z *= k;
}

static inline vec_3d vec3_add(const vec_3d a, const vec_3d b) {
  return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline vec_3d vec3_sub(const vec_3d a, const vec_3d b) {
  return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline vec_3d vec3_mul(const vec_3d v1, const vec_3d v2) {
  return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

static inline vec_3d vec3_div(const vec_3d v1, const vec_3d v2) {
  return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

static inline vec_3d vec3_mul_s(const vec_3d v, float t) {
  return vec3(v.x * t, v.y * t, v.z * t);
}

static inline vec_3d vec3_div_s(const vec_3d v, float t) {
  return vec3(v.x / t, v.y / t, v.z / t);
}

static inline float vec3_dot(const vec_3d v1, const vec_3d v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

static inline vec_3d vec3_cross(const vec_3d v1, const vec_3d v2) {
  float x = v1.y * v2.z - v1.z * v2.y;
  float y = -(v1.x * v2.z - v1.z * v2.x);
  float z = v1.x * v2.y - v1.y * v2.x;
  return vec3(x, y, z);
}

static inline vec_3d vec3_unit_vector(vec_3d v) {
  return vec3_div_s(v, vec3_length(v));
}
