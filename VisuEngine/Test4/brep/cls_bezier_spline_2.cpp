#include "cls_bezier_spline_2.h"

// Project
#include "base/support.h"

template <typename STORETYPE, typename COMPUTETYPE>
COMPUTETYPE cls_bezier_spline_2<STORETYPE, COMPUTETYPE>::GetBasisFval() const
{
	return BernsteinCoef(p_param, mOrder, iT);
}
