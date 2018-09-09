/**
 * g++ main1.cpp -o prog1
 */

#include <iostream>
using std::cout;
using std::endl;

// ==============================================

class baseClass
{
public:
	baseClass();
	~baseClass();
protected:
	int protMemBase;
private:
	int privMemBase;
};

// ==============================================

baseClass::baseClass()
{
	cout << "baseClass constructor" << endl;
	protMemBase = 1;
	privMemBase = 1;
}


baseClass::~baseClass()
{
	cout << "baseClass destructor" << endl;
}

// ==============================================

class childClass : public baseClass
{
public:
	childClass();
	~childClass();
protected:
	int protMemChild;
private:
	int privMemChild;
};

// ==============================================

childClass::childClass() :
	baseClass()
{
	cout << "childClass constructor" << endl;
	// You can skip 'this' here
	/*this->*/protMemBase = 2;
	// You can't do this because privMemBase is private in the baseClass
	//privMemBase = 2;
}


childClass::~childClass()
{
	cout << "childClass destructor" << endl;
}

// ==============================================
int main(int /*argc*/, char** /*argv*/)
{
	childClass obj1;
}
