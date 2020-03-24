#include "embree3/rtcore.h"

#include <cmath>
#include <cstdio>

// pi/2
#define M_PI_2 1.57079632679489661923

#define RAY_OFFSET 0.001f

struct RTC_ALIGN(16) vertex_t {
    float x;
    float y;
    float z;
    float w;
};
struct RTC_ALIGN(16) triangle_t {
    unsigned int v0;
    unsigned int v1;
    unsigned int v2;
    unsigned int v3; //TODO get rid
};
struct RTC_ALIGN(16) quad_t {
    unsigned int v0;
    unsigned int v1;
    unsigned int v2;
    unsigned int v3;
};

struct RTC_ALIGN(16) shape_tri_t {
    vertex_t _v[4];
    triangle_t _tr[2];
    const int _n_v = 4;
    const int _n_tr = 2;
};
struct RTC_ALIGN(16) shape_quad_t {
    vertex_t _v[4];
    quad_t _q[1];
    const int _n_v = 4;
    const int _n_q = 1;
};

void InitRay(RTCRay& ray1, const float* const centr_ray_dir, const float theta) {
    ray1.org_x = 0.0f;
    ray1.org_y = 0.0f;
    ray1.org_z = 0.0f;
    ray1.tnear = RAY_OFFSET;
    ray1.dir_x = centr_ray_dir[0] * sinf(theta);
    ray1.dir_y = centr_ray_dir[1];
    ray1.dir_z = centr_ray_dir[2] * cosf(theta);
    ray1.time  = 0.0f;
    ray1.tfar  = 1.0f - RAY_OFFSET;
    ray1.mask  = 0xFFFFFFFF;
    ray1.id    = 0;
    ray1.flags = 0;
}
void InitRay8(RTCRay8& ray8, const float* const centr_ray_dir) {
    const float start_theta = 0.0f;
    const float delta_theta = (float)M_PI_2;
    const float step_theta = (delta_theta - start_theta) / (7.0f);
    for (int i = 0; i < 8; i++) {
        const float theta = start_theta + step_theta * (float)i;
        ray8.org_x[i] = 0.0f;
        ray8.org_y[i] = 0.0f;
        ray8.org_z[i] = 0.0f;
        ray8.tnear[i] = RAY_OFFSET;
        ray8.dir_x[i] = centr_ray_dir[0] * sinf(theta);
        ray8.dir_y[i] = centr_ray_dir[1];
        ray8.dir_z[i] = centr_ray_dir[2] * cosf(theta);
        ray8.time[i]  = 0.0f;
        ray8.tfar[i]  = 1.0f - RAY_OFFSET;
        ray8.mask[i]  = 0xFFFFFFFF;
        ray8.id[i]    = 0;
        ray8.flags[i] = 0;
    }
}

int main(int argc, char** argv)
{
    RTCDevice device = rtcNewDevice(NULL);
    RTCScene scene = rtcNewScene(device);

    shape_tri_t shape1;
    shape_quad_t shape2;

    shape1._v[0] = { -0.5f, -0.5f, 0.5f, 1.0f };
    shape1._v[1] = {  0.5f, -0.5f, 0.5f, 1.0f };
    shape1._v[2] = {  0.5f,  0.5f, 0.5f, 1.0f };
    shape1._v[3] = { -0.5f,  0.5f, 0.5f, 1.0f };
    shape1._tr[0] = { 0 + 0, 0 + 1, 0 + 2 };
    shape1._tr[1] = { 0 + 0, 0 + 2, 0 + 3 };
    shape2._v[0] = { -0.5f, -0.5f, 0.75f, 1.0f };
    shape2._v[1] = {  0.5f, -0.5f, 0.75f, 1.0f };
    shape2._v[2] = {  0.5f,  0.5f, 0.75f, 1.0f };
    shape2._v[3] = { -0.5f,  0.5f, 0.75f, 1.0f };
    shape2._q[0] = { 4 + 0, 4 + 1, 4 + 2, 4 + 3 };

    RTCGeometry geom_tri = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    rtcSetSharedGeometryBuffer(geom_tri, RTC_BUFFER_TYPE_VERTEX, 0,
        RTC_FORMAT_FLOAT3, &shape1._v[0], 0, sizeof(vertex_t), shape1._n_v);
    rtcSetSharedGeometryBuffer(geom_tri, RTC_BUFFER_TYPE_INDEX, 0,
        RTC_FORMAT_UINT3, &shape1._tr[0], 0, sizeof(triangle_t), shape1._n_tr);
    rtcCommitGeometry(geom_tri);
    rtcAttachGeometry(scene, geom_tri);
    rtcReleaseGeometry(geom_tri);

    RTCGeometry geom_quad = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_QUAD);
    rtcSetSharedGeometryBuffer(geom_quad, RTC_BUFFER_TYPE_VERTEX, 0,
        RTC_FORMAT_FLOAT3, &shape2._v[0], 0, sizeof(vertex_t), shape2._n_v);
    rtcSetSharedGeometryBuffer(geom_quad, RTC_BUFFER_TYPE_INDEX, 0,
        RTC_FORMAT_UINT4, &shape2._q[0], 0, sizeof(quad_t), shape2._n_q);
    rtcCommitGeometry(geom_quad);
    rtcAttachGeometry(scene, geom_quad);
    rtcReleaseGeometry(geom_quad);

    rtcCommitScene(scene);

    const float central_ray_dir[3] = { 0.0f, 0.0f, 1.0f };

    printf("==== Scalar ====\n");
    const float start_theta = 0.0f;
    const float delta_theta = (float)M_PI_2;
    const float step_theta = (delta_theta - start_theta) / (7.0f);
    for (int i = 0; i < 8; i++) {
        const float theta = start_theta + step_theta * (float)i;
        RTCRay ray1;
        InitRay(ray1, &central_ray_dir[0], theta);
        RTCIntersectContext ctx1;
        rtcInitIntersectContext(&ctx1);
        rtcOccluded1(scene, &ctx1, &ray1);
        if (ray1.tfar > 0.0f) { printf("not occluded\t"); }
        else { printf("occluded\t"); }

        RTCRayHit rayhit1;
        InitRay(rayhit1.ray, &central_ray_dir[0], theta);
        rayhit1.hit.geomID = RTC_INVALID_GEOMETRY_ID;
        rtcIntersect1(scene, &ctx1, &rayhit1);
        if (rayhit1.hit.geomID == RTC_INVALID_GEOMETRY_ID) {
            printf("not intersected\n");
        } else { printf("intersected\n"); }
    }

    printf("==== Vector ====\n");
    RTCRay8 ray8;
    InitRay8(ray8, &central_ray_dir[0]);
    const int valid[] = { -1, -1, -1, -1, -1, -1, -1, -1 };
    RTCIntersectContext ctx8;
    rtcInitIntersectContext(&ctx8);
    rtcOccluded8(valid, scene, &ctx8, &ray8); // <--------------------- crash here
    for (int i = 0; i < 8; i++) {
        if (ray8.tfar[i] > 0.0f) { printf("not occluded\n"); }
        else { printf("occluded\n"); }
    }

    RTCRayHit8 rayhit8;
    InitRay8(rayhit8.ray, &central_ray_dir[0]);
    for (int i = 0; i < 8; i++) {
        rayhit8.hit.geomID[i] = RTC_INVALID_GEOMETRY_ID;
    }
    rtcIntersect8(valid, scene, &ctx8, &rayhit8);
    for (int i = 0; i < 8; i++) {
        if (rayhit8.hit.geomID[i] == RTC_INVALID_GEOMETRY_ID) {
            printf("not occluded\n");
        } else { printf("occluded\n"); }
    }
    return 0;
}
