/**

	@class cls_bezier_spline

	A Bezier spline has some specific characteristics:

	1) The number of control points is equal to the order+1

*/

#ifndef CLS_BEZIER_CURVE_H
#define CLS_BEZIER_CURVE_H

#include "cls_spline_base.h" // mother class

template <typename STORETYPE, typename COMPUTETYPE>
class cls_bezier_spline : public cls_spline_base<STORETYPE, COMPUTETYPE>
{
public:
	cls_bezier_spline();
	/*~cls_bezier_spline();*/

	void Generate(unsigned int p_order, unsigned int p_nCPs = 0);

	COMPUTETYPE GetBasisValue(COMPUTETYPE t, unsigned int i) const;
};

template class cls_bezier_spline<float, float>;

#endif // CLS_BEZIER_CURVE_H
