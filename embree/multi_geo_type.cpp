#include "embree3/rtcore.h"

#include <cmath>
#include <cstdio>
#include <vector>

#define M_2PI        6.28318530717958647692 // 2*pi
#define M_PI         3.14159265358979323846 // pi
#define M_PI_2       1.57079632679489661923 // pi/2

#define RAY_OFFSET 0.001f

struct RTC_ALIGN(16) vec3fa {
    float x;
    float y;
    float z;
    float w;
};
typedef vec3fa direction_t;
typedef vec3fa vertex_t;
struct /*RTC_ALIGN(16)*/ triangle_t {
    unsigned int v0;
    unsigned int v1;
    unsigned int v2;
};
struct /*RTC_ALIGN(16)*/ quad_t {
    unsigned int v0;
    unsigned int v1;
    unsigned int v2;
    unsigned int v3;
};

struct shape_tri_t {
    std::vector<vertex_t> _v;
    std::vector<triangle_t> _tr;
};
struct shape_quad_t {
    std::vector<vertex_t> _v;
    std::vector<quad_t> _q;
};

void generate_shape_tri(shape_tri_t& shape, const float z, const unsigned int v_offset) {
    shape._v.resize(4);
    shape._tr.resize(2);
    shape._v.push_back({ -0.5f, -0.5f, z, 0.0f });
    shape._v.push_back({  0.5f, -0.5f, z, 0.0f });
    shape._v.push_back({  0.5f,  0.5f, z, 0.0f });
    shape._v.push_back({ -0.5f,  0.5f, z, 0.0f });
    shape._tr.push_back({ v_offset + 0, v_offset + 1, v_offset + 2 });
    shape._tr.push_back({ v_offset + 0, v_offset + 2, v_offset + 3 });
}
void generate_shape_quad(shape_quad_t& shape, const float z, const unsigned int v_offset) {
    shape._v.resize(4);
    shape._q.resize(1);
    shape._v.push_back({ -0.5f, -0.5f, z, 0.0f });
    shape._v.push_back({  0.5f, -0.5f, z, 0.0f });
    shape._v.push_back({  0.5f,  0.5f, z, 0.0f });
    shape._v.push_back({ -0.5f,  0.5f, z, 0.0f });
    shape._q.push_back({ v_offset + 0, v_offset + 1, v_offset + 2, v_offset + 3 });
}

void submit_shape_tri(RTCDevice& device, RTCScene& scene, const shape_tri_t& shape) {
    RTCGeometry geom_tri = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    rtcSetSharedGeometryBuffer(geom_tri, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
        shape._v.data(), 0, sizeof(vertex_t), shape._v.size());
    rtcSetSharedGeometryBuffer(geom_tri, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
        shape._tr.data(), 0, sizeof(triangle_t), shape._tr.size());
    rtcCommitGeometry(geom_tri);
    rtcAttachGeometry(scene, geom_tri);
    rtcReleaseGeometry(geom_tri);
}
void submit_shape_quad(RTCDevice& device, RTCScene& scene, const shape_quad_t& shape) {
    RTCGeometry geom_quad = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_QUAD);
    rtcSetSharedGeometryBuffer(geom_quad, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
        shape._v.data(), 0, sizeof(vertex_t), shape._v.size());
    rtcSetSharedGeometryBuffer(geom_quad, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT4,
        shape._q.data(), 0, sizeof(quad_t), shape._q.size());
    rtcCommitGeometry(geom_quad);
    rtcAttachGeometry(scene, geom_quad);
    rtcReleaseGeometry(geom_quad);
}

void InitRay(RTCRay& ray1, const direction_t& dir, const float theta) {
    ray1.org_x = 0.0f;
    ray1.org_y = 0.0f;
    ray1.org_z = 0.0f;
    ray1.tnear = RAY_OFFSET;
    ray1.dir_x = dir.x * sinf(theta);
    ray1.dir_y = dir.y;
    ray1.dir_z = dir.z * cosf(theta);
    ray1.time  = 0.0f;
    ray1.tfar  = 1.0f - RAY_OFFSET;
    ray1.mask  = 0xFFFFFFFF;
    ray1.id    = 0;
    ray1.flags = 0;
}
void InitRay8(RTCRay8& ray8, const direction_t& dir) {
    const float start_theta = 0.0f;
    const float delta_theta = (float)M_PI_2;
    const float step_theta = (delta_theta - start_theta) / (7.0f);
    for (int i = 0; i < 8; i++) {
        const float theta = start_theta + step_theta * (float)i;
        ray8.org_x[i] = 0.0f;
        ray8.org_y[i] = 0.0f;
        ray8.org_z[i] = 0.0f;
        ray8.tnear[i] = RAY_OFFSET;
        ray8.dir_x[i] = dir.x * sinf(theta);
        ray8.dir_y[i] = dir.y;
        ray8.dir_z[i] = dir.z * cosf(theta);
        ray8.time[i]  = 0.0f;
        ray8.tfar[i]  = 1.0f - RAY_OFFSET;
        ray8.mask[i]  = 0xFFFFFFFF;
        ray8.id[i]    = 0;
        ray8.flags[i] = 0;
    }
}

struct RTC_ALIGN(32) valid_t {
    int _valid[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
};

void TestOcclusion1(RTCScene& scene, const direction_t& central_ray_dir) {
    printf("==== Scalar ====\n");
    const float start_theta = 0.0f;
    const float delta_theta = (float)M_PI_2;
    const float step_theta = (delta_theta - start_theta) / (7.0f);
    for (int i = 0; i < 8; i++) {
        const float theta = start_theta + step_theta * (float)i;
        RTCRay ray1;
        InitRay(ray1, central_ray_dir, theta);
        RTCIntersectContext ctx1;
        rtcInitIntersectContext(&ctx1);
        rtcOccluded1(scene, &ctx1, &ray1);
        if (ray1.tfar > 0.0f) { printf("not occluded\n"); }
        else { printf("occluded\n"); }
    }
}
void TestOcclusion8(RTCScene& scene, const direction_t& central_ray_dir) {
    printf("==== Vector ====\n");
    RTCRay8 ray8;
    InitRay8(ray8, central_ray_dir);
    const valid_t valid;
    RTCIntersectContext ctx8;
    rtcInitIntersectContext(&ctx8);
    rtcOccluded8(&valid._valid[0], scene, &ctx8, &ray8);
    for (int i = 0; i < 8; i++) {
        if (ray8.tfar[i] > 0.0f) { printf("not occluded\n"); }
        else { printf("occluded\n"); }
    }
}

int main(int argc, char** argv)
{
    RTCDevice device = rtcNewDevice(NULL);
    RTCScene scene = rtcNewScene(device);
    shape_tri_t shape1;
    shape_quad_t shape2;
    generate_shape_tri(shape1, 0.5f, 0);
    generate_shape_quad(shape2, 0.75f, 4);
    submit_shape_tri(device, scene, shape1);
    submit_shape_quad(device, scene, shape2);
    rtcCommitScene(scene);

    const direction_t central_ray_dir = { 0.0f, 0.0f, 1.0f, 0.0f };
    TestOcclusion1(scene, central_ray_dir);
    TestOcclusion8(scene, central_ray_dir);

    return 0;
}
