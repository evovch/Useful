#ifndef STL_STRUCTS_H
#define STL_STRUCTS_H

struct vertex_t {
	double mX;
	double mY;
	double mZ;
	vertex_t(double p_x, double p_y, double p_z) {
		mX = p_x; mY = p_y; mZ = p_z;
	}
};

struct loop_t {
	struct vertex_t* mV1;
	struct vertex_t* mV2;
	struct vertex_t* mV3;
	loop_t(struct vertex_t* p_v1, struct vertex_t* p_v2, struct vertex_t* p_v3) {
		mV1 = p_v1; mV2 = p_v2; mV3 = p_v3;
	}
};

struct normal_t {
	double mX;
	double mY;
	double mZ;
	normal_t(double p_x, double p_y, double p_z) {
		mX = p_x; mY = p_y; mZ = p_z;
	}
};

struct facet_t {
	struct normal_t* mNormal;
	struct loop_t* mLoop;
	struct facet_t* mNext;
	facet_t(struct normal_t* p_normal, struct loop_t* p_loop) {
		mNormal = p_normal; mLoop = p_loop; mNext = nullptr;
	}
	void SetNext(struct facet_t* p_next) {
		mNext = p_next;
	}
	struct facet_t* GetNext(void) {
		return mNext;
	}
};

#endif // STL_STRUCTS_H
