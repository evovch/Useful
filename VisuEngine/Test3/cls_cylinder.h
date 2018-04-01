/**

	@class cls_cylinder

**/

#ifndef CLS_CYLINDER_H
#define CLS_CYLINDER_H

#include "myTypes.h"

class cls_axis2_placement_3d;

class cls_cylinder
{
public:
	cls_cylinder();
	~cls_cylinder();

	void BuildMatrix(void);

	// return number of found intersections
	unsigned int Intersect(point_t p_start, direction_t p_ray, point_t& inter1, point_t& inter2);

public:
	double mR;

	cls_axis2_placement_3d* mPosition;

	matrix4x4_t mM;
	matrix4x4_t mMinv; // Inversed
	bool mMatrixBuilt;

};

#endif // CLS_CYLINDER_H
