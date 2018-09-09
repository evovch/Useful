#include <iostream>
using std::cout;
using std::endl;

// ==============================================

class baseClass
{
public:
	baseClass();
	~baseClass();

	virtual int method1(int param) const = 0;

	void method2(int param);

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


void baseClass::method2(int param)
{
	this->method1(param);
}

// ==============================================

class childClass : public baseClass
{
public:
	childClass();
	~childClass();

	// Overloaded method
	int method1(int param) const;

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


int childClass::method1(int param) const
{
	return param;
}

// ==============================================
int main(int /*argc*/, char** /*argv*/)
{
	childClass obj1;
	obj1.method1(1);
	obj1.method2(1);
}
