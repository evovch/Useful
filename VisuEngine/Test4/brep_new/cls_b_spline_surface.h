/**

	@class cls_b_spline_surface

*/

#ifndef CLS_B_SPLINE_SURFACE_H
#define CLS_B_SPLINE_SURFACE_H

#include "cls_surface_base.h" // mother class

template <typename STORETYPE, typename COMPUTETYPE>
class cls_b_spline_surface : public cls_surface_base<STORETYPE, COMPUTETYPE>
{
public:
	cls_b_spline_surface();
	~cls_b_spline_surface();

	void Generate(unsigned int p_orderU, unsigned int p_orderV,
	              unsigned int p_nCPsU,  unsigned int p_nCPsV);

	COMPUTETYPE GetBasisValueU(COMPUTETYPE u, unsigned int i) const;

	COMPUTETYPE GetBasisValueV(COMPUTETYPE v, unsigned int i) const;

private:

	void GenerateKnotVector(STORETYPE* p_knotVector,
	                        unsigned int p_order,
	                        unsigned int p_nCPs,
	                        unsigned int* p_paramMin,
	                        unsigned int* p_paramMax);

private:

	/**
	 * Knot vector in U-parameter direction
	 */
	STORETYPE* mKnotsU;

	/**
	 * Knot vector in V-parameter direction
	 */
	STORETYPE* mKnotsV;

};

template class cls_b_spline_surface<float, float>;

#endif // CLS_B_SPLINE_SURFACE_H
