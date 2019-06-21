// Usage: g++ delete_this.cpp -o prog; ./prog

#include <iostream>

class A {
public:
	A(int val):
		member(val)
	{ }

	~A()
	{
		member = 99; // this makes no sense...
	}

	void method1() {
		delete this; // really bad practice...
	}

	void Print(void) {
		std::cout << member << std::endl;
	}
private:
	int member;
};

int main(int argc, char** argv)
{
	// Example 1 - dynamic memory (heap) - new/delete - all fine
	A* obj4 = new A(40); // new - dynamic allocation
	obj4->Print();
	delete obj4; // delete
	// You can not use a deleted object - undefined behavior
	obj4->Print();

	// Example 2 - dynamic memory (heap) - new/delete - all fine
	A* obj3 = new A(30); // new - dynamic allocation
	obj3->Print();
	obj3->method1(); // 'delete this;' inside the method
	// You can not use a deleted object - undefined behavior
	obj3->Print();

/*
	// Example 3 - static memory (stack) - delete is not allowed
	// You can not free stack (not heap) memory
	A obj2(10); // static allocation
	obj2.Print();
	// The program will crash here
	delete &obj2; // delete
	// You can not use a deleted object - undefined behavior
	obj2.Print();
*/
/*
	// Example 4 - static memory (stack) - delete is not allowed
	// You can not free stack (not heap) memory
	A obj1(20); // static allocation
	obj1.Print();
	// The program will crash here
	obj1.method1(); // 'delete this;' inside the method
	// You can not use a deleted object - undefined behavior
	obj1.Print();
*/
}
