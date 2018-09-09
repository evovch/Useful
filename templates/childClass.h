#ifndef CHILDCLASS_H
#define CHILDCLASS_H

#include "baseClass.h" // mother class

template <typename T>
class childClass : public baseClass<T>
{
public:
	childClass();
	~childClass();

	// Overloaded method
	T method1(T param) const;

protected:
	T protMemChild;
private:
	T privMemChild;
};

// This concrete class declaration is necessary!
template class childClass<int>;

#endif // CHILDCLASS_H
