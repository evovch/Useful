#pragma once

namespace vulovch {
namespace geo_str {

typedef struct triangle_t_ {
	unsigned int v0;
	unsigned int v1;
	unsigned int v2;
} triangle_t;

typedef struct point_t_ {
	float _data[3];
} point_t;

typedef struct vertex_t_ {
	point_t coords;
} vertex_t;

} // end of geo_str
} // end of namespace vulovch
