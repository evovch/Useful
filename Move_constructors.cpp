#include <vector>
#include <iostream>

class Base
{
private:
	std::vector<int> _data;
public:
	Base(const int& d1, const int& d2, const int d3);
	~Base(void);
	Base(const Base& other) = delete;
	Base& operator=(const Base& other) = delete;
	Base(Base&& other) noexcept;
	Base& operator=(Base&& other) noexcept = delete;
private:
	void Print(void) const {
		std::cout << "[" << _data.size() << "]" << "\t";
		for (const int& d : _data) std::cout << d << "\t";
	}
};

Base::Base(const int& d1, const int& d2, const int d3) {
	_data.push_back(d1);
	_data.push_back(d2);
	_data.push_back(d3);
	std::cout << "Base constructor: ";
	this->Print(); std::cout << std::endl;
}

Base::~Base(void) {
	std::cout << "Base destructor: ";
	this->Print(); std::cout << std::endl;
}

// Move

Base::Base(Base&& other) noexcept {
	std::cout << "Base move constructor: " << std::endl;
	std::cout << "Before: "; other.Print(); this->Print(); std::cout << std::endl;
	std::swap(_data, other._data);
	std::cout << "After: ";  other.Print(); this->Print(); std::cout << std::endl;
}

int main(int argc, char** argv)
{
	std::cout << "-----------------------------------" << std::endl;
	Base base1(1, 2, 3);
	std::cout << "-----------------------------------" << std::endl;
	Base base2(4, 5, 6);
	std::cout << "-----------------------------------" << std::endl;
	std::vector<Base> bases;
	bases.push_back(Base(7, 8, 9));
	std::cout << "-----------------------------------" << std::endl;
}
