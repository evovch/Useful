/**

	@class cls_axis2_placement_3d

**/

#include "myTypes.h"

class cls_axis2_placement_3d
{
public:
	cls_axis2_placement_3d();
	~cls_axis2_placement_3d();

public:
	// Basic
	point_t mLocation; // Origin
	direction_t mAxis; // Z axis
	direction_t mRefDirection; // Approximate X axis

	// Derived
	direction_t mXaxis; // Exact X axis
	direction_t mYaxis;

};
