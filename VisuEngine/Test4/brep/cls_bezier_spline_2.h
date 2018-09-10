/**

	#class cls_bezier_spline_2

*/

#ifndef CLS_BEZIER_SPLINE_2_H
#define CLS_BEZIER_SPLINE_2_H

#include "cls_spline_base.h" // mother class

template <typename STORETYPE, typename COMPUTETYPE>
class cls_bezier_spline_2 : public cls_spline_base<STORETYPE, COMPUTETYPE>
{
public:

	virtual COMPUTETYPE GetBasisFval() const;

};

#endif // CLS_BEZIER_SPLINE_2_H
