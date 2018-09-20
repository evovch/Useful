/**

	@class cls_bezier_surface

*/

#ifndef CLS_BEZIER_SURFACE_H
#define CLS_BEZIER_SURFACE_H

#include "cls_surface_base.h" // mother class

template <typename STORETYPE, typename COMPUTETYPE>
class cls_bezier_surface : public cls_surface_base<STORETYPE, COMPUTETYPE>
{
public:
	cls_bezier_surface();
	/*~cls_bezier_surface();*/

	void Generate(unsigned int p_orderU,    unsigned int p_orderV,
	              unsigned int p_nCPsU = 0, unsigned int p_nCPsV = 0);

	COMPUTETYPE GetBasisValueU(COMPUTETYPE u, unsigned int i) const;

	COMPUTETYPE GetBasisValueV(COMPUTETYPE v, unsigned int j) const;

};

template class cls_bezier_surface<float, float>;

#endif // CLS_BEZIER_SURFACE_H
