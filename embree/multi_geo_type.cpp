#include "embree3/rtcore.h"

#include <cstdio>
#include <vector>

#define RAY_OFFSET 0.001f

struct point_t {
    float x;
    float y;
    float z;
};
struct vertex_t {
    float x;
    float y;
    float z;
};
struct triangle_t {
    unsigned int v0;
    unsigned int v1;
    unsigned int v2;
};
struct quad_t {
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

void InitRay(RTCRay& ray1, const point_t& pt1, const point_t& pt2) {
    ray1.org_x = pt1.x;
    ray1.org_y = pt1.y;
    ray1.org_z = pt1.z;
    ray1.tnear = RAY_OFFSET;
    ray1.dir_x = pt2.x - pt1.x;
    ray1.dir_y = pt2.y - pt1.y;
    ray1.dir_z = pt2.z - pt1.z;
    ray1.time  = 0.0f;
    ray1.tfar  = 1.0f - RAY_OFFSET;
    ray1.mask  = 0xFFFFFFFF;
    ray1.id    = 0;
    ray1.flags = 0;
}
void InitRay8(RTCRay8& ray8, const point_t& pt1, const point_t& pt2) {
    for (int i = 0; i < 8; i++) {
        ray8.org_x[i] = pt1.x;
        ray8.org_y[i] = pt1.y;
        ray8.org_z[i] = pt1.z;
        ray8.tnear[i] = RAY_OFFSET;
        ray8.dir_x[i] = pt2.x - pt1.x;
        ray8.dir_y[i] = pt2.y - pt1.y;
        ray8.dir_z[i] = pt2.z - pt1.z;
        ray8.time[i]  = 0.0f;
        ray8.tfar[i]  = 1.0f - RAY_OFFSET;
        ray8.mask[i]  = 0xFFFFFFFF;
        ray8.id[i]    = 0;
        ray8.flags[i] = 0;
    }
}

void generate_shape_tri(shape_tri_t& shape, const float z, const unsigned int v_offset) {
    shape._v.push_back({ -0.5f, -0.5f, z });
    shape._v.push_back({  0.5f, -0.5f, z });
    shape._v.push_back({  0.5f,  0.5f, z });
    shape._v.push_back({ -0.5f,  0.5f, z });
    shape._tr.push_back({ v_offset + 0, v_offset + 1, v_offset + 2 });
    shape._tr.push_back({ v_offset + 0, v_offset + 2, v_offset + 3 });
}
void generate_shape_quad(shape_quad_t& shape, const float z, const unsigned int v_offset) {
    shape._v.push_back({ -0.5f, -0.5f, z });
    shape._v.push_back({  0.5f, -0.5f, z });
    shape._v.push_back({  0.5f,  0.5f, z });
    shape._v.push_back({ -0.5f,  0.5f, z });
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

int main(int argc, char** argv)
{
	RTCDevice device = rtcNewDevice(NULL);
	RTCScene scene = rtcNewScene(device);

    shape_tri_t shape1;
    shape_quad_t shape2;
    generate_shape_tri(shape1, 0.0f, 0);
    generate_shape_quad(shape2, 1.0f, 4);

    submit_shape_tri(device, scene, shape1);
    submit_shape_quad(device, scene, shape2);

    rtcCommitScene(scene);

    const point_t pt1 = { 0.0f, 0.0f, -1.0f };
    const point_t pt2 = { 0.0f, 0.0f,  1.0f };

    {
        RTCRay ray1;
        InitRay(ray1, pt1, pt2);

        RTCIntersectContext ctx1;
        rtcInitIntersectContext(&ctx1);
        rtcOccluded1(scene, &ctx1, &ray1);
        if (ray1.tfar > 0.0f) { printf("not occluded\n"); }
        else { printf("occluded\n"); }
    }

    {
        RTCRay8 ray8;
        InitRay8(ray8, pt1, pt2);

        const int valid[] = { -1, -1, -1, -1, -1, -1, -1, -1 };
        RTCIntersectContext ctx8;
        rtcInitIntersectContext(&ctx8);
        rtcOccluded8(valid, scene, &ctx8, &ray8); // <----------------- crash here
        for (int i = 0; i < 8; i++) {
            if (ray8.tfar[i] > 0.0f) { printf("not occluded\n"); }
            else { printf("occluded\n"); }
        }
    }

    while (getchar() != 'q') {
    }

	return 0;
}
