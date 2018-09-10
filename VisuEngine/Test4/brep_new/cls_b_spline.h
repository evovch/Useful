/**

	@class cls_b_spline

*/

#ifndef CLS_B_SPLINE_H
#define CLS_B_SPLINE_H

#include "cls_spline_base.h" // mother class

template <typename STORETYPE, typename COMPUTETYPE>
class cls_b_spline : public cls_spline_base<STORETYPE, COMPUTETYPE>
{
public:
	cls_b_spline();
	/*~cls_b_spline();*/

	void Generate(unsigned int p_order, unsigned int p_nCPs);

	COMPUTETYPE GetBasisValue(COMPUTETYPE t, unsigned int i) const;

private:
	COMPUTETYPE GetBasis(unsigned int i, unsigned int p, COMPUTETYPE u) const;

private:

	STORETYPE* mKnots;

};

template class cls_b_spline<float, float>;

#endif // CLS_B_SPLINE_H
