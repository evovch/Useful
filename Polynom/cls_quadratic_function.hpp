/**

	@class cls_quadratic_function

*/

#ifndef CLS_QUADRATIC_FUNCTION_H
#define CLS_QUADRATIC_FUNCTION_H

#include <cmath>

template <typename T>
class cls_quadratic_function
{
public:
	cls_quadratic_function(T p_a, T p_b, T p_c) :
	    a(p_a),
	    b(p_b),
	    c(p_c)
	{
	}

	~cls_quadratic_function() {}

	T Evaluate(T p_x) const {
		return a*p_x*p_x + b*p_x + c;
	}

	/**
	 * Return number of roots
	 //TODO implement
	 */
	unsigned short Solve(T* o_roots)
	{
		Delta = this->Discriminant();

		//TODO take into account tolerances!
		if (Delta == static_cast<T>(0)) {
			o_roots[0] = this->RootOnly();
			return 1;
		} else if (Delta > static_cast<T>(0)) {
			o_roots[0] = this->Root1();
			o_roots[1] = this->Root2();
			return 2;
		} else {
			// Delta < 0
			return 0;
		}
	}

private:

	/**
	 *
	 */
	T Discriminant(void)
	{
		return b*b - static_cast<T>(4)*a*c;
	}

	T Root1(void) const
	{
		return (-b + std::sqrt(Delta)) / (static_cast<T>(2)*a);
	}

	T Root2(void) const
	{
		return (-b - std::sqrt(Delta)) / (static_cast<T>(2)*a);
	}

	T RootOnly(void) const
	{
		return -b / (static_cast<T>(2)*a);
	}

private:

	T a;
	T b;
	T c;

private:

	// Derived
	T Delta;

};

#endif // CLS_QUADRATIC_FUNCTIONH