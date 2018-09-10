/**

	@class cls_cartesian_point

*/

#ifndef CLS_CARTESIAN_POINT_H
#define CLS_CARTESIAN_POINT_H

#include "base/cls_logger.h"

template <typename TYPE>
class cls_cartesian_point
{
public:
	cls_cartesian_point() {}

	cls_cartesian_point(TYPE x, TYPE y, TYPE z) {
	    mCoord[0] = x;
	    mCoord[1] = y;
	    mCoord[2] = z;
	}

	cls_cartesian_point(const cls_cartesian_point& src) {
		this->mCoord[0] = src.mCoord[0];
		this->mCoord[1] = src.mCoord[1];
		this->mCoord[2] = src.mCoord[2];
	}

	~cls_cartesian_point() {}

	void Dump(void) const {
		LOG(INFO) << "x=" << mCoord[0] << "\ty=" << mCoord[1] << "\tz=" << mCoord[2] << cls_logger::endl;
	}

	cls_cartesian_point& operator=(const cls_cartesian_point& other) {
		if (this != &other) {
			mCoord[0] = other.mCoord[0];
			mCoord[1] = other.mCoord[1];
			mCoord[2] = other.mCoord[2];
		}
		return *this;
	}

	cls_cartesian_point& operator+=(const cls_cartesian_point& rhs) {
		this->mCoord[0] += rhs.mCoord[0];
		this->mCoord[1] += rhs.mCoord[1];
		this->mCoord[2] += rhs.mCoord[2];
		return *this;
	}

	cls_cartesian_point& operator-=(const cls_cartesian_point& rhs) {
		this->mCoord[0] -= rhs.mCoord[0];
		this->mCoord[1] -= rhs.mCoord[1];
		this->mCoord[2] -= rhs.mCoord[2];
		return *this;
	}

	cls_cartesian_point& operator*=(const TYPE rhs) {
		this->mCoord[0] *= rhs;
		this->mCoord[1] *= rhs;
		this->mCoord[2] *= rhs;
		return *this;
	}

	TYPE &operator[](int i) {
		if (i>2) {
			exit(EXIT_FAILURE);
		}
		return this->mCoord[i];
	}

public:

	TYPE mCoord[3];
	
};

template <typename TYPE>
static cls_cartesian_point<TYPE> operator*(const TYPE lhs, const cls_cartesian_point<TYPE>& rhs)
{
	return cls_cartesian_point<TYPE>(rhs.mCoord[0] * lhs,
	                                 rhs.mCoord[1] * lhs,
	                                 rhs.mCoord[2] * lhs);
}

template <typename TYPE>
static cls_cartesian_point<TYPE> operator*(const cls_cartesian_point<TYPE>& lhs, const TYPE rhs)
{
	return cls_cartesian_point<TYPE>(lhs.mCoord[0] * rhs,
	                                 lhs.mCoord[1] * rhs,
	                                 lhs.mCoord[2] * rhs);
}

template <typename TYPE>
static cls_cartesian_point<TYPE> operator+(const cls_cartesian_point<TYPE>& lhs, const cls_cartesian_point<TYPE>& rhs)
{
	return cls_cartesian_point<TYPE>(lhs.mCoord[0] + rhs.mCoord[0],
	                                 lhs.mCoord[1] + rhs.mCoord[1],
	                                 lhs.mCoord[2] + rhs.mCoord[2]);
}

template <typename TYPE>
static cls_cartesian_point<TYPE> operator-(const cls_cartesian_point<TYPE>& lhs, const cls_cartesian_point<TYPE>& rhs)
{
	return cls_cartesian_point<TYPE>(lhs.mCoord[0] - rhs.mCoord[0],
	                                 lhs.mCoord[1] - rhs.mCoord[1],
	                                 lhs.mCoord[2] - rhs.mCoord[2]);
}

template class cls_cartesian_point<float>;

#endif // CLS_CARTESIAN_POINT_H
