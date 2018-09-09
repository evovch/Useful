#ifndef BASECLASS_H
#define BASECLASS_H

template <typename T>
class baseClass
{
public:
	baseClass();
	~baseClass();
protected:
	T protMemBase;
private:
	T privMemBase;
};

// This concrete class declaration is necessary!
template class baseClass<int>;

#endif // BASECLASS_H
