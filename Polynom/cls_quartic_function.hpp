/**

	@class cls_quartic_function

	//TODO Think further - which intermediate computed values should be 'cached'?

*/

#ifndef CLS_QUARTIC_FUNCTION_H
#define CLS_QUARTIC_FUNCTION_H

#include <cmath>

template <typename T>
class cls_quartic_function
{
public:
	cls_quartic_function(T p_a, T p_b, T p_c, T p_d, T p_e) :
	    a(p_a),
	    b(p_b),
	    c(p_c),
	    d(p_d),
	    e(p_e)
	{
	}

	~cls_quartic_function() {}

	T Evaluate(T p_x) const {
		return a*p_x*p_x*p_x*p_x + b*p_x*p_x*p_x + c*p_x*p_x + d*p_x + e;
	}

	/**
	 * Return number of roots
	 //TODO implement
	 */
	unsigned short Solve(T* o_roots)
	{
		T v_Delta = this->Discriminant();

		//TODO take into account tolerances!
		if (v_Delta < static_cast<T>(0)) {

			p = this->Coefficientp();
			q = this->Coefficientq();
			S = this->CoefficientS();

			//TODO check, which roots are the two correct ones
			o_roots[0] = this->Root1();
			o_roots[1] = this->Root2();
			o_roots[2] = this->Root3();
			o_roots[3] = this->Root4();

			return 2;
		}
		else if (v_Delta > static_cast<T>(0)) {
			T v_P = this->PolynomialP();
			T v_D = this->PolynomialD();

			if (v_P < static_cast<T>(0) && v_D < static_cast<T>(0)) {

				p = this->Coefficientp();
				q = this->Coefficientq();
				S = this->CoefficientS();

				// All 4 roots
				o_roots[0] = this->Root1();
				o_roots[1] = this->Root2();
				o_roots[2] = this->Root3();
				o_roots[3] = this->Root4();

				return 4;

			} else if (v_P > static_cast<T>(0) || v_D > static_cast<T>(0)) {
				//TODO is this case just simply an 'else' case?
				return 0;
			}

			// This should not happen!
			return 0;

		}
		else if (v_Delta == static_cast<T>(0)) {
			T v_P = this->PolynomialP();
			T v_D = this->PolynomialD();
			T v_Delta0 = this->PolynomialDelta0();

			//TODO within some tolerance!
			if (v_P < static_cast<T>(0) && v_D < static_cast<T>(0) && v_Delta0 != static_cast<T>(0)) {
				//TODO check, which roots are the three correct ones
				return 3; // x1=x2, x3, x4
			}
			// ... //TODO implement all cases

			//TODO temporary solution
			// Not all cases are implemented
			return 0;
		}

		// This should not happen!
		return 0;
	}

private:

	/**
	 *
	 */
	T Discriminant(void)
	{
		return
		  static_cast<T>(256) * a*a*a*e*e*e
		- static_cast<T>(192) * a*a*b*d*e*e
		- static_cast<T>(128) * a*a*c*c*e*e
		+ static_cast<T>(144) * a*a*c*d*d*e
		- static_cast<T>(27)  * a*a*d*d*d*d
		+ static_cast<T>(144) * a*b*b*c*e*e
		- static_cast<T>(6)   * a*b*b*d*d*e
		- static_cast<T>(80)  * a*b*c*c*d*e
		+ static_cast<T>(18)  * a*b*c*d*d*d
		+ static_cast<T>(16)  * a*c*c*c*c*e
		- static_cast<T>(4)   * a*c*c*c*d*d
		- static_cast<T>(27)  * b*b*b*b*e*e
		+ static_cast<T>(18)  * b*b*b*c*d*e
		- static_cast<T>(4)   * b*b*b*d*d*d
		- static_cast<T>(4)   * b*b*c*c*c*e
		+ static_cast<T>(1)   * b*b*c*c*d*d;
	}

	/**
	 * +
	 */
	T PolynomialP(void)
	{
		return static_cast<T>(8) *a*c - static_cast<T>(3) *b*b;
	}

	/**
	 * +
	 */
	T Coefficientp(void)
	{
		return this->PolynomialP() / ( static_cast<T>(8) *a*a );
	}

	/**
	 * +
	 */
	T PolynomialR(void)
	{
		return b*b*b + static_cast<T>(8) *d*a*a - static_cast<T>(4) *a*b*c;
	}

	/**
	 * +
	 */
	T Coefficientq(void)
	{
		return this->PolynomialR() / ( static_cast<T>(8) *a*a*a );
	}

	/**
	 * +
	 */
	T PolynomialDelta0(void)
	{
		return c*c - static_cast<T>(3) *b*d + static_cast<T>(12) *a*e;
	}

	/**
	 * +
	 */
	T PolynomialDelta1(void)
	{
		return static_cast<T>(2)  *c*c*c - static_cast<T>(9)  *b*c*d
		     + static_cast<T>(27) *b*b*e + static_cast<T>(27) *a*d*d
		     - static_cast<T>(72) *a*c*e;
	}

	/**
	 * +
	 */
	T PolynomialD(void)
	{
		return static_cast<T>(64) *a*a*a*e - static_cast<T>(16) *a*a*c*c
		     + static_cast<T>(16) *a*b*b*c - static_cast<T>(16) *a*a*b*d
		     - static_cast<T>(3)  *b*b*b*b;
	}

	/**
	 * +
	 */
	T CoefficientS(void)
	{
		T v_Q = this->CoefficientQ();
		T v_Delta0 = this->PolynomialDelta0();
		T v_p = this->Coefficientp();

		return static_cast<T>(0.5) * std::sqrt( -(static_cast<T>(2) * v_p) / static_cast<T>(3)
		                                        +(v_Q + v_Delta0/v_Q)/(static_cast<T>(3)*a) );
	}

	/**
	 * +
	 */
	T CoefficientQ(void)
	{
		T v_Delta0 = this->PolynomialDelta0();
		T v_Delta1 = this->PolynomialDelta1();

		return std::cbrt((v_Delta1 + std::sqrt(v_Delta1*v_Delta1 - static_cast<T>(4)*v_Delta0*v_Delta0*v_Delta0))
		                 / static_cast<T>(2));
	}

private:

	T Root1(void) const
	{
		return
		  -b/(static_cast<T>(4)*a) - S + static_cast<T>(0.5) *
		  std::sqrt(-static_cast<T>(4)*S*S - static_cast<T>(2)*p + q/S);
	}

	T Root2(void) const
	{
		return
		  -b/(static_cast<T>(4)*a) - S - static_cast<T>(0.5) *
		  std::sqrt(-static_cast<T>(4)*S*S - static_cast<T>(2)*p + q/S);
	}

	T Root3(void) const
	{
		return
		  -b/(static_cast<T>(4)*a) + S + static_cast<T>(0.5) *
		  std::sqrt(-static_cast<T>(4)*S*S - static_cast<T>(2)*p - q/S);
	}

	T Root4(void) const
	{
		return
		  -b/(static_cast<T>(4)*a) + S - static_cast<T>(0.5) *
		  std::sqrt(-static_cast<T>(4)*S*S - static_cast<T>(2)*p - q/S);
	}

private:

	T a;
	T b;
	T c;
	T d;
	T e;

private:

	// Derived
	T p;
	T q;
	T S;

};

#endif // CLS_QUARTIC_FUNCTION_H
