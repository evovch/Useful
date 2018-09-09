/**
 * g++ main2.cpp -o prog2
 */

#include <iostream>
using std::cout;
using std::endl;

// ==============================================
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

// ==============================================
template <typename T>
baseClass<T>::baseClass()
{
	cout << "baseClass constructor" << endl;
	protMemBase = 1;
	privMemBase = 1;
}

template <typename T>
baseClass<T>::~baseClass()
{
	cout << "baseClass destructor" << endl;
}

// ==============================================
template <typename T>
class childClass : public baseClass<T>
{
public:
	childClass();
	~childClass();
protected:
	T protMemChild;
private:
	T privMemChild;
};

// ==============================================
template <typename T>
childClass<T>::childClass() :
	baseClass<T>()
{
	cout << "childClass constructor" << endl;
	// You HAVE TO use 'this' (try without it!) 
	this->protMemBase = 2;
	// You can't do this because privMemBase is private in the baseClass
	//this->privMemBase = 2;
}

template <typename T>
childClass<T>::~childClass()
{
	cout << "childClass destructor" << endl;
}

// ==============================================
int main(int /*argc*/, char** /*argv*/)
{
	childClass<int> obj1;
}
