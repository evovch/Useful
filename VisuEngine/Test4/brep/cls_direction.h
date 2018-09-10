/**

	@class cls_direction

*/

#ifndef CLS_DIRECTION_H
#define CLS_DIRECTION_H

template <typename TYPE>
class cls_direction
{
public:
	cls_direction() {}

	cls_direction(TYPE x, TYPE y, TYPE z) {
	    mComponent[0] = x; mComponent[1] = y; mComponent[2] = z;
	}

	~cls_direction() {}

public:

	TYPE mComponent[3];
	
};

#endif // CLS_DIRECTION_H
