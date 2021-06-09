#define DELETECOPY

#include <utility>
#include <iostream>
#include <vector>

// ============================================================================

class Base
{
private:
	bool _valid;
private:
	int _base_private_x;
protected:
	int _base_protected_y;
public:
	int _base_public_z;
public:
	explicit Base(const int& x, const int& y, const int& z);
	virtual ~Base(void) = 0;
#ifdef DELETECOPY
	Base(const Base& other) = delete;
	Base& operator=(const Base& other) = delete;
#else
	Base(const Base& other);
	Base& operator=(const Base& other);
#endif
	Base(Base&& other) noexcept;
	Base& operator=(Base&& other) noexcept;
};

Base::Base(const int& x, const int& y, const int& z)
	: _valid(true)
	, _base_private_x(x)
	, _base_protected_y(y)
	, _base_public_z(z)
{
	std::cout << "Base  constructor: "
		<< _base_private_x << "\t" << _base_protected_y << "\t" << _base_public_z << std::endl;
}

Base::~Base(void) {
	std::cout << "Base  destructor: "
		<< _base_private_x << "\t" << _base_protected_y << "\t" << _base_public_z << "\t"
		<< " valid=" << (_valid ? "1" : "0" ) << std::endl;
}

// Copy

#ifndef DELETECOPY

Base::Base(const Base& other)
	: _valid           (other._valid)
	, _base_private_x  (other._base_private_x)
	, _base_protected_y(other._base_protected_y)
	, _base_public_z   (other._base_public_z)
{
	std::cout << "Base  copy constructor" << std::endl;
}

Base& Base::operator=(const Base& other) {
	_valid            = other._valid;
	_base_private_x   = other._base_private_x;
	_base_protected_y = other._base_protected_y;
	_base_public_z    = other._base_public_z;
	std::cout << "Base  copy operator=" << std::endl;
	return *this;
}

#endif // DELETECOPY

// Move

Base::Base(Base&& other) noexcept {
	_valid = true; other._valid = false;
	std::swap(_base_private_x,   other._base_private_x);
	std::swap(_base_protected_y, other._base_protected_y);
	std::swap(_base_public_z,    other._base_public_z);
	std::cout << "Base  move constructor" << std::endl;
}

Base& Base::operator=(Base&& other) noexcept {
	_valid = true; other._valid = false;
	std::swap(_base_private_x,   other._base_private_x);
	std::swap(_base_protected_y, other._base_protected_y);
	std::swap(_base_public_z,    other._base_public_z);
	std::cout << "Base  move operator=" << std::endl;
	return *this;
}

// ============================================================================

class Child : public Base
{
private:
	int _child_private_p;
protected:
	int _child_protected_q;
public:
	int _child_public_r;
public:
	explicit Child(const int& x, const int& y, const int& z,
		const int& p, const int& q, const int& r);
	virtual ~Child(void) override;
#ifdef DELETECOPY
	Child(const Child& other) = delete;
	Child& operator=(const Child& other) = delete;
#else
	Child(const Child& other);
	Child& operator=(const Child& other);
#endif
	Child(Child&& other) noexcept;
	Child& operator=(Child&& other) noexcept;
};

Child::Child(const int& x, const int& y, const int& z,
	const int& p, const int& q, const int& r)
	: Base(x, y, z)
	, _child_private_p(p)
	, _child_protected_q(q)
	, _child_public_r(r)
{
	std::cout << "Child constructor: "
		<< "(" << x << ")" << "\t" << _base_protected_y << "\t" << _base_public_z << "\t"
		<< _child_private_p << "\t" << _child_protected_q << "\t" << _child_public_r << std::endl;
}

Child::~Child(void) {
	std::cout << "Child destructor: "
		<< "x" << "\t" << _base_protected_y << "\t" << _base_public_z << "\t"
		<< _child_private_p << "\t" << _child_protected_q << "\t" << _child_public_r << std::endl;
}

// Copy

#ifndef DELETECOPY

Child::Child(const Child& other)
	: Base(other)
	, _child_private_p  (other._child_private_p)
	, _child_protected_q(other._child_protected_q)
	, _child_public_r   (other._child_public_r)
{
	std::cout << "Child copy constructor" << std::endl;
}

Child& Child::operator=(const Child& other) {
	_child_private_p   = other._child_private_p;
	_child_protected_q = other._child_protected_q;
	_child_public_r    = other._child_public_r;
	std::cout << "Child copy operator=" << std::endl;
	return *this;
}

#endif // DELETECOPY

// Move

Child::Child(Child&& other) noexcept
	: Base(std::move(other))
{
	std::swap(_child_private_p,   other._child_private_p);
	std::swap(_child_protected_q, other._child_protected_q);
	std::swap(_child_public_r,    other._child_public_r);
	std::cout << "Child move constructor" << std::endl;
}

Child& Child::operator=(Child&& other) noexcept {
	std::swap(_child_private_p,   other._child_private_p);
	std::swap(_child_protected_q, other._child_protected_q);
	std::swap(_child_public_r,    other._child_public_r);
	std::cout << "Child move operator=" << std::endl;
	return *this;
}

// ============================================================================

int main(int argc, char** argv) {
	std::cout << "-----------------------------------" << std::endl;
	Child ch1(1, 2, 3, 4, 5, 6);
	std::cout << "-----------------------------------" << std::endl;
#ifndef DELETECOPY
	Child ch2(ch1);
	std::cout << "-----------------------------------" << std::endl;
	Child ch3(11, 12, 13, 14, 15, 16);
	std::cout << "-----------------------------------" << std::endl;
	Child ch4 = ch3;
	std::cout << "-----------------------------------" << std::endl;
#endif // DELETECOPY
	//Func(Child(11, 12, 13, 14, 15, 16));
	std::vector<Child> children;
	children.push_back(Child(91, 92, 93, 94, 95, 96));
	std::cout << "-----------------------------------" << std::endl;
}
