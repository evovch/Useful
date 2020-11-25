#include <iostream>
#include <map>

typedef unsigned short shape_id_t;

// ----------------------------------------------------------------------------

namespace basemath {

typedef struct vec3ui_
{
	//union {
		//unsigned int x;
		unsigned int v0;
	//};
	//union {
		//unsigned int y;
		unsigned int v1;
	//};
	//union {
		//unsigned int z;
		unsigned int v2;
	//};
} vec3ui;

template<typename T>
class Vec3
{
public:
	//union {
		T _data[3];
		//T x, y, z;
	//};
public:
	Vec3(void) : _data{ T(0.0), T(0.0), T(0.0) } {}
	Vec3(const T& ix, const T& iy, const T& iz) :
		_data{ ix, iy, iz } {}
	Vec3(const Vec3& other) :
		_data{ other._data[0], other._data[1], other._data[2] } {}
	Vec3(Vec3&& other) noexcept :
		_data{ other._data[0], other._data[1], other._data[2] } {}
	~Vec3(void) {}
public:
	__forceinline Vec3& operator=(const Vec3& rhs) {
		_data[0] = rhs._data[0]; _data[1] = rhs._data[1];
		_data[2] = rhs._data[2];
		return *this;
	}
	__forceinline Vec3& operator=(Vec3&& rhs) noexcept {
		_data[0] = rhs._data[0]; _data[1] = rhs._data[1];
		_data[2] = rhs._data[2];
		return *this;
	}
	__forceinline const T& operator[](const size_t idx) const {
		return _data[idx];
	}
	__forceinline Vec3& operator+=(const Vec3& rhs) {
		_data[0] += rhs._data[0]; _data[1] += rhs._data[1];
		_data[2] += rhs._data[2];
		return *this;
	}
	__forceinline Vec3 operator+(const Vec3& rhs) const {
		return Vec3(_data[0] + rhs._data[0], _data[1] + rhs._data[1],
			_data[2] + rhs._data[2]);
	}
	__forceinline Vec3& operator-=(const Vec3& rhs) {
		_data[0] -= rhs._data[0]; _data[1] -= rhs._data[1];
		_data[2] -= rhs._data[2];
		return *this;
	}
	__forceinline Vec3 operator-(const Vec3& rhs) const {
		return Vec3(_data[0] - rhs._data[0], _data[1] - rhs._data[1],
			_data[2] - rhs._data[2]);
	}
	__forceinline Vec3& operator*=(const T& k) {
		_data[0] *= k; _data[1] *= k; _data[2] *= k;
		return *this;
	}
	__forceinline Vec3 operator*(const T& k) const {
		return Vec3(_data[0] * k, _data[1] * k, _data[2] * k);
	}
	__forceinline Vec3& operator/=(const T& k) {
		_data[0] /= k; _data[1] /= k; _data[2] /= k;
		return *this;
	}
	__forceinline Vec3 operator/(const T& k) const {
		return Vec3(_data[0] / k, _data[1] / k, _data[2] / k);
	}
};

//template<typename T>
//class Vec4
//{
//public:
//	//union {
//		T _data[4];
//		//T x, y, z, w;
//	//};
//public:
//	Vec4(void) : _data{ T(0.0), T(0.0), T(0.0), T(0.0) } {}
//	Vec4(const float ix, const float iy, const float iz, const float iw) :
//		_data{ ix, iy, iz, iw } {}
//};

//template<typename T>
//class Mat44
//{
//public:
//	Mat44(void)
//		: _data{1.0f, 0.0f, 0.0f, 0.0f,
//				0.0f, 1.0f, 0.0f, 0.0f,
//				0.0f, 0.0f, 1.0f, 0.0f,
//				0.0f, 0.0f, 0.0f, 1.0f} {
//		std::cout << "constructing Mat44" << std::endl;
//	}
//	~Mat44(void) {
//		std::cout << "destructing Mat44" << std::endl;
//	}
//public:
//	void Shift(const float dx, const float dy, const float dz) {
//		_data[3*4+0] += dx;
//		_data[3*4+1] += dy;
//		_data[3*4+2] += dz;
//	}
//	// return M*v
//	/*Vec4 RightMultiply(const Vec4& v) const {
//		return Vec4(
//			_data[0*4+0]*v.x + _data[0*4+1]*v.y + _data[0*4+2]*v.z + _data[0*4+3]*v.w,
//			_data[1*4+0]*v.x + _data[1*4+1]*v.y + _data[1*4+2]*v.z + _data[1*4+3]*v.w,
//			_data[2*4+0]*v.x + _data[2*4+1]*v.y + _data[2*4+2]*v.z + _data[2*4+3]*v.w,
//			_data[3*4+0]*v.x + _data[3*4+1]*v.y + _data[3*4+2]*v.z + _data[3*4+3]*v.w);
//	}*/
//private:
//	float _data[16];
//};

template<typename T>
T dot(const Vec3<T>& op1, const Vec3<T>& op2) {
	return op1[0]*op2[0] + op1[1]*op2[1] + op1[2]*op2[2];
}

template<typename T>
Vec3<T> cross(const Vec3<T>& op1, const Vec3<T>& op2) {
	return Vec3<T>(
		op1[1]*op2[2] - op1[2]*op2[1],
		op1[2]*op2[0] - op1[0]*op2[2],
		op1[0]*op2[1] - op1[1]*op2[0]);
}

template class Vec3<float>;
//template class Vec4<float>;
//template class Mat44<float>;

typedef Vec3<float> vec3f;
//typedef Vec4<float> vec4f;
//typedef Mat44<float> mat44f;

}; // namespace basemath

typedef basemath::vec3f vertex_t;
//typedef basemath::mat44f Matrix44f;
typedef basemath::vec3ui triangle_t;
typedef basemath::vec3f point_t;
typedef basemath::vec3f direction_t;

typedef struct ray_t_ {
	point_t _origin;
	direction_t _dir;
	ray_t_(const float& x, const float& y, const float& z,
		const float& dx, const float& dy, const float& dz)
		: _origin(x, y, z), _dir(dx, dy, dz) {}
} ray_t;

// ----------------------------------------------------------------------------

bool CalculateIntersection(const ray_t& p_ray,
	const vertex_t& p_v0, const vertex_t& p_v1, const vertex_t& p_v2,
	//point_t& o_inter,
	float& o_t) {
	const float EPSILON = 0.0000001f;
	const direction_t edge1 = p_v1 - p_v0;
	const direction_t edge2 = p_v2 - p_v0;
	const direction_t h = basemath::cross(p_ray._dir, edge2);
	const float a = basemath::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON) return false;
	const float f = 1.0f / a;
	const direction_t s = p_ray._origin - p_v0;
	const float u = f * basemath::dot(s, h);
	if (u < 0.0f || u > 1.0f) return false;
	const direction_t q = basemath::cross(s, edge1);
	const float v = f * basemath::dot(p_ray._dir, q);
	if (v < 0.0f || u + v > 1.0f) return false;
	o_t = f * basemath::dot(edge2, q);
	if (o_t <= EPSILON) return false;
	//o_inter = p_ray._origin + p_ray._dir * t;
	return true;
}

// ----------------------------------------------------------------------------

class Shape;

class Mesh
{
	friend class MeshBuilder;
	friend class Scene;
private:
	Mesh(void)
		: _n_vertices(0), _n_triangles(0),
		_vertices(nullptr), _triangles(nullptr),
		_shape(nullptr) {
		std::cout << "constructing Mesh" << std::endl;
	}
	~Mesh(void) {
		std::cout << "destructing Mesh" << std::endl;
		if (_vertices != nullptr) delete[] _vertices;
		if (_triangles != nullptr) delete[] _triangles;
	}
	Mesh(const Mesh& other) = delete;
	Mesh(Mesh&& other) noexcept = delete;
	Mesh& operator=(const Mesh& other) = delete;
	Mesh& operator=(Mesh&& other) noexcept = delete;
public:
	const Shape* GetAssociatedShape(void) const {
		return _shape;
	}
	bool CalculateFirstIntersection(const ray_t& p_ray,
		//point_t& o_inter,
		float& o_t) {
		bool intersects = false;
		float t = 999999.0f; // TODO plus dohua
		for (unsigned int iTr = 0; iTr < _n_triangles; iTr++) {
			const triangle_t& curTr = _triangles[iTr];
			const vertex_t& curV0 = _vertices[curTr.v0];
			const vertex_t& curV1 = _vertices[curTr.v1];
			const vertex_t& curV2 = _vertices[curTr.v2];
			float curT;
			bool res = CalculateIntersection(p_ray, curV0, curV1, curV2, curT);
			if (res) {
				intersects = true;
				t = (curT < t) ? curT : t; // t = min(curT, t)
			}
		}
		//o_inter = p_ray._origin + p_ray._dir * t;
		o_t = t;
		return intersects;
	}
private:
	unsigned int _n_vertices;
	unsigned int _n_triangles;
	vertex_t* _vertices;
	triangle_t* _triangles;
	const Shape* _shape; // non-owning
};

// ----------------------------------------------------------------------------

class Box;
class Orb;

class MeshBuilder
{
public:
	static void VisitBuildMesh(const Box& p_box, Mesh& o_mesh);
	static void VisitBuildMesh(const Orb& p_orb, Mesh& o_mesh);
public:
	static void BuildMeshBox(Mesh& o_mesh,
		const float p_hsx, const float p_hsy, const float p_hsz,
		const point_t& p_tr);
	static void BuildMeshOrb(Mesh& o_mesh,
		const float p_r,
		const point_t& p_tr);
};

// ----------------------------------------------------------------------------

enum class shape_type_t
{
	BOX,
	ORB
};

enum class shape_color_t
{
	BLACK = 0,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE
};

const char* ShapeColorToEscCode(const shape_color_t& color) {
	switch (color) {
	case shape_color_t::BLACK:   return "\033[40m"; break;
	case shape_color_t::RED:     return "\033[41m"; break;
	case shape_color_t::GREEN:   return "\033[42m"; break;
	case shape_color_t::YELLOW:  return "\033[43m"; break;
	case shape_color_t::BLUE:    return "\033[44m"; break;
	case shape_color_t::MAGENTA: return "\033[45m"; break;
	case shape_color_t::CYAN:    return "\033[46m"; break;
	case shape_color_t::WHITE:   return "\033[47m"; break;
	default: return "\033[0m"; break;
	}
}

#define DEFAULTSHAPECOLOR shape_color_t::WHITE

class Shape
{
	friend class MeshBuilder;
	friend class Scene;
protected:
	Shape(const shape_id_t id, const shape_type_t type)
		: _id(id), _type(type), _color(DEFAULTSHAPECOLOR), _mesh(nullptr) {
		std::cout << "constructing Shape" << std::endl;
	}
	virtual ~Shape(void) = 0 {
		std::cout << "destructing Shape" << std::endl;
	}
	Shape(const Shape& other) = delete;
	Shape(Shape&& other) noexcept = delete;
	Shape& operator=(const Shape& other) = delete;
	Shape& operator=(Shape&& other) noexcept = delete;
public:
	virtual void AcceptBuildMesh(Mesh& mesh) const = 0;
public:
	void Shift(const float dx, const float dy, const float dz) {
		//transformation.Shift(dx, dy, dz);
		_translation += direction_t(dx, dy, dz);
	}
	void SetColor(const shape_color_t& color) {
		_color = color;
	}
	void SetMesh(const Mesh* const mesh) {
		_mesh = mesh;
	}
private:
	shape_id_t _id;
	shape_type_t _type;
	shape_color_t _color;
	//Matrix44f _transformation;
	point_t _translation;
	const Mesh* _mesh; // non-owning
};

class Box : public Shape
{
	friend class MeshBuilder;
	friend class Scene;
private:
	Box(const shape_id_t id, const float hsx, const float hsy, const float hsz)
		: Shape(id, shape_type_t::BOX),
		_hsx(hsx), _hsy(hsy), _hsz(hsz) {
		std::cout << "constructing Box" << std::endl;
	}
	virtual ~Box(void) override {
		std::cout << "destructing Box" << std::endl;
	}
	Box(const Box& other) = delete;
	Box(Box&& other) noexcept = delete;
	Box& operator=(const Box& other) = delete;
	Box& operator=(Box&& other) noexcept = delete;
private:
	virtual void AcceptBuildMesh(Mesh& mesh) const override {
		return MeshBuilder::VisitBuildMesh(*this, mesh);
	}
private:
	float _hsx;
	float _hsy;
	float _hsz;
};

class Orb : public Shape
{
	friend class MeshBuilder;
	friend class Scene;
private:
	Orb(const shape_id_t id, const float r)
		: Shape(id, shape_type_t::ORB),
		_r(r) {
		std::cout << "constructing Orb" << std::endl;
	}
	virtual ~Orb(void) override {
		std::cout << "destructing Orb" << std::endl;
	}
	Orb(const Orb& other) = delete;
	Orb(Orb&& other) noexcept = delete;
	Orb& operator=(const Orb& other) = delete;
	Orb& operator=(Orb&& other) noexcept = delete;
public:
	virtual void AcceptBuildMesh(Mesh& mesh) const override {
		return MeshBuilder::VisitBuildMesh(*this, mesh);
	}
private:
	float _r;
};

// ----------------------------------------------------------------------------

//static
void MeshBuilder::VisitBuildMesh(const Box& p_box, Mesh& o_mesh) {
	const float& hsx = p_box._hsx;
	const float& hsy = p_box._hsy;
	const float& hsz = p_box._hsz;
	MeshBuilder::BuildMeshBox(o_mesh, hsx, hsy, hsz, p_box._translation);
	o_mesh._shape = &p_box; // store the lick back to the original shape
}
//static
void MeshBuilder::VisitBuildMesh(const Orb& p_orb, Mesh& o_mesh) {
	const float& r = p_orb._r;
	MeshBuilder::BuildMeshOrb(o_mesh, r, p_orb._translation);
	o_mesh._shape = &p_orb; // store the lick back to the original shape
}
//static
void MeshBuilder::BuildMeshBox(Mesh& o_mesh,
	const float p_hsx, const float p_hsy, const float p_hsz,
	const point_t& p_tr) {
	o_mesh._n_vertices = 8;
	o_mesh._n_triangles = 12;
	if (o_mesh._vertices != nullptr) delete[] o_mesh._vertices;
	if (o_mesh._triangles != nullptr) delete[] o_mesh._triangles;
	o_mesh._vertices = new vertex_t[o_mesh._n_vertices];
	o_mesh._triangles = new triangle_t[o_mesh._n_triangles];
	o_mesh._vertices[0] = {  p_hsx+p_tr[0], -p_hsy+p_tr[1], -p_hsz+p_tr[2] };
	o_mesh._vertices[1] = {  p_hsx+p_tr[0],  p_hsy+p_tr[1], -p_hsz+p_tr[2] };
	o_mesh._vertices[2] = { -p_hsx+p_tr[0],  p_hsy+p_tr[1], -p_hsz+p_tr[2] };
	o_mesh._vertices[3] = { -p_hsx+p_tr[0], -p_hsy+p_tr[1], -p_hsz+p_tr[2] };
	o_mesh._vertices[4] = {  p_hsx+p_tr[0], -p_hsy+p_tr[1],  p_hsz+p_tr[2] };
	o_mesh._vertices[5] = {  p_hsx+p_tr[0],  p_hsy+p_tr[1],  p_hsz+p_tr[2] };
	o_mesh._vertices[6] = { -p_hsx+p_tr[0],  p_hsy+p_tr[1],  p_hsz+p_tr[2] };
	o_mesh._vertices[7] = { -p_hsx+p_tr[0], -p_hsy+p_tr[1],  p_hsz+p_tr[2] };
	o_mesh._triangles[0]  = { 3, 0, 4 };
	o_mesh._triangles[1]  = { 3, 4, 7 };
	o_mesh._triangles[2]  = { 0, 1, 5 };
	o_mesh._triangles[3]  = { 0, 5, 4 };
	o_mesh._triangles[4]  = { 1, 2, 6 };
	o_mesh._triangles[5]  = { 1, 6, 5 };
	o_mesh._triangles[6]  = { 2, 3, 7 };
	o_mesh._triangles[7]  = { 2, 7, 6 };
	o_mesh._triangles[8]  = { 3, 2, 1 };
	o_mesh._triangles[9]  = { 3, 1, 0 };
	o_mesh._triangles[10] = { 4, 5, 6 };
	o_mesh._triangles[11] = { 4, 6, 7 };
}
//static
void MeshBuilder::BuildMeshOrb(Mesh& o_mesh,
	const float p_r,
	const point_t& p_tr) {

}

// ----------------------------------------------------------------------------

class Scene
{
public:
	Scene(void) {
		std::cout << "constructing Scene" << std::endl;
	}
	~Scene(void) {
		std::cout << "destructing Scene" << std::endl;
		for (auto shp : _shapes) {
			delete shp.second;
		}
		_shapes.clear();
		for (auto mesh : _meshes) {
			delete mesh.second;
		}
		_meshes.clear();
	}
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) noexcept = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) noexcept = delete;
public:
	Shape& CreateBox(const float hsx, const float hsy, const float hsz) {
		Shape* const shp = new Box(_shape_id_counter, hsx, hsy, hsz);
		_shapes.insert({ _shape_id_counter, shp });
		_shape_id_counter++;
		return *shp;
	}
	Shape& CreateOrb(const float r) {
		Shape* const shp = new Orb(_shape_id_counter, r);
		_shapes.insert({ _shape_id_counter, shp });
		_shape_id_counter++;
		return *shp;
	}
	void BuildMeshes(void) {
		for (auto shp : _shapes) {
			Mesh* const mesh = new Mesh();
			_meshes.insert({ shp.first, mesh });
			shp.second->AcceptBuildMesh(*mesh);
			shp.second->SetMesh(mesh);
		}
	}
	bool CalculateFirstIntersection(const ray_t& p_ray,
		//point_t& o_inter,
		const Shape*& o_shape) {
		bool intersects = false;
		float t = 999999.0f; // TODO plus dohua
		for (auto mesh : _meshes) {
			float curT;
			bool res = mesh.second->CalculateFirstIntersection(p_ray, curT);
			if (res) {
				intersects = true;
				//t = (curT < t) ? curT : t; // t = min(curT, t)
				if (curT < t) {
					t = curT;
					o_shape = mesh.second->GetAssociatedShape();
				}
			}
		}
		//o_inter = p_ray._origin + p_ray._dir * t;
		return intersects;
	}
	void DrawInConsole(void) {
		const float z_near = 1.0f;
		const unsigned int w = 60;
		const unsigned int h = 60;
		for (unsigned int ix = 0; ix < w + 2; ix++) std::cout << "--"; // frame
		std::cout << std::endl; // frame
		for (unsigned int iy = 0; iy < h; iy++) {
			std::cout << "| "; // frame
			for (unsigned int ix = 0; ix < w; ix++) {
				const float x = static_cast<float>(ix)/static_cast<float>(w) - 0.5f;
				const float y = static_cast<float>(iy)/static_cast<float>(h) - 0.5f;
				const ray_t ray(0.0f, 0.0f, 0.0f, x, y, z_near);
				//point_t inters;
				const Shape* hit_shape = nullptr;
				const bool intersects = CalculateFirstIntersection(ray, /*inters*/ hit_shape);
				if (intersects) {
					std::cout << ShapeColorToEscCode(hit_shape->_color)
						<< "  " << "\033[0m"; // hit
				} else {
					std::cout << "  "; // miss
				}
			}
			std::cout << " |" << std::endl; // frame, newline
		}
		for (unsigned int ix = 0; ix < w + 2; ix++) std::cout << "--"; // frame
		std::cout << std::endl; // frame
	}
private:
	static shape_id_t _shape_id_counter;
	std::map<shape_id_t, Shape* const> _shapes;
	std::map<shape_id_t, Mesh* const> _meshes;
};

//static
shape_id_t Scene::_shape_id_counter = 0;

// ----------------------------------------------------------------------------

int main(int argc, char** argv)
{
	Scene scene1;
	Shape& shp1 = scene1.CreateBox(0.1f, 0.1f, 0.1f);
	shp1.Shift(-0.3f, -0.4f, 2.0f);
	shp1.SetColor(shape_color_t::CYAN);
	Shape& shp2 = scene1.CreateBox(0.1f, 0.1f, 0.1f);
	shp2.Shift(0.4f, 0.3f, 2.0f);
	shp2.SetColor(shape_color_t::RED);
	Shape& shp3 = scene1.CreateBox(0.2f, 0.2f, 0.1f);
	shp3.Shift(0.6f, 0.6f, 1.3f);
	shp3.SetColor(shape_color_t::MAGENTA);
	scene1.BuildMeshes();
	scene1.DrawInConsole();
}
