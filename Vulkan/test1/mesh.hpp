#pragma once

// STD
#include <vector>

// PROJECT
#include "geo_str.hpp"
#include "mesh_size_info.hpp"

namespace vulovch {

class Mesh {
private:
	mesh_size_info_t _size_info;
	std::vector<geo_str::vertex_t> _vertices;
	std::vector<geo_str::triangle_t> _triangles;
public:
	explicit Mesh() {
	}
	~Mesh(void) {
		_vertices.clear();
		_triangles.clear();
	}
	Mesh(const Mesh& other) = delete;
	Mesh(Mesh&& other) noexcept = delete;
	Mesh& operator=(const Mesh& other) = delete;
	Mesh& operator=(Mesh&& other) noexcept = delete;
public:
	const mesh_size_info_t& GetSizeInfo(void) const { return _size_info; }
public:
	void GenerateBox(const float& hsx, const float& hsy, const float& hsz,
		const float& xc, const float& yc, const float& zc) {
		_size_info._n_v = 8;
		_size_info._n_tr = 12;
		_vertices.resize(_size_info._n_v);
		_triangles.resize(_size_info._n_tr);
		const geo_str::vertex_t v[8] = {
			{ { xc-hsx, yc-hsy, zc-hsz } },
			{ { xc+hsx, yc-hsy, zc-hsz } },
			{ { xc-hsx, yc+hsy, zc-hsz } },
			{ { xc+hsx, yc+hsy, zc-hsz } },
			{ { xc-hsx, yc-hsy, zc+hsz } },
			{ { xc+hsx, yc-hsy, zc+hsz } },
			{ { xc-hsx, yc+hsy, zc+hsz } },
			{ { xc+hsx, yc+hsy, zc+hsz } } };
		static constexpr geo_str::triangle_t tr[12] = {
			{0,5,4}, {0,1,5},
			{1,7,5}, {1,3,7},
			{3,6,7}, {3,2,6},
			{2,4,6}, {2,0,4},
			{0,3,1}, {0,2,3},
			{5,6,4}, {5,7,6} };
		for (unsigned int i = 0; i < _size_info._n_v; i++) {
			_vertices[i] = v[i]; }
		for (unsigned int i = 0; i < _size_info._n_tr; i++) {
			_triangles[i] = tr[i]; }
	}
};

} // end of namespace vulovch
