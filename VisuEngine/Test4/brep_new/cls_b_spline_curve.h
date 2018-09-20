/**

	@class cls_b_spline_curve

*/

#ifndef CLS_B_SPLINE_H
#define CLS_B_SPLINE_H

#include "cls_spline_base.h" // mother class

template <typename STORETYPE, typename COMPUTETYPE>
class cls_b_spline_curve : public cls_spline_base<STORETYPE, COMPUTETYPE>
{
public:
	cls_b_spline_curve();
	~cls_b_spline_curve();

	void Generate(unsigned int p_order, unsigned int p_nCPs);

	COMPUTETYPE GetBasisValue(COMPUTETYPE t, unsigned int i) const;

private:

	STORETYPE* mKnotsT;

};

template class cls_b_spline_curve<float, float>;

#endif // CLS_B_SPLINE_H
