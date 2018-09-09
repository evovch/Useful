#ifndef BASECLASS_H
#define BASECLASS_H

template <typename T>
class baseClass
{
public:
	baseClass();
	~baseClass();

	virtual T method1(T param) const = 0;

	void method2(T param);

protected:
	T protMemBase;
private:
	T privMemBase;
};

// This concrete class declaration is necessary!
template class baseClass<int>;

#endif // BASECLASS_H
