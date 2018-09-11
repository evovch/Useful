#include <iostream>
using std::cout;
using std::endl;

#include <map>
#include <ctime>

class myClass
{
public:
	myClass(unsigned int p_n) {
		std::srand(std::time(nullptr));
		fN = p_n;
		for (unsigned int i=0; i<fN; i++) {
			fMap.insert(std::pair<unsigned int, unsigned int>(std::rand(), std::rand()));
		}
	}

	~myClass() {}

	unsigned int fN;
	std::map<unsigned int, unsigned int> fMap;

	void Print(void) {
		unsigned int i=0;
		std::map<unsigned int, unsigned int>::const_iterator iter = fMap.begin();
		for (; iter != fMap.end(); ++iter) {
			cout << i << ": " << iter->first << ", " << iter->second << endl;
			i++;
		}
	}

	unsigned int GetItemKey(unsigned int index) {
		unsigned int i=0;
		std::map<unsigned int, unsigned int>::const_iterator iter = fMap.begin();
		for (; iter != fMap.end(); ++iter) {
			if (i == index) return iter->first;
			i++;
		}
		return iter->first; //TODO
	}

	unsigned int GetItem1(unsigned int key) {
		return fMap.at(key);
	}

	unsigned int GetItem2(unsigned int key) {
		std::map<unsigned int, unsigned int>::const_iterator iter = fMap.find(key);
		return iter->second;
	}

};

void erroneousCode(void)
{
	unsigned int arraySize = 100;

	// Just to demonstrate memory leakage by executing new without delete
	unsigned int* dynamicMem = new unsigned int[arraySize];

	// Obviously write beyond the dynamic array size
	// The problem is that the program will not crash
	// every time you do that.
	for (unsigned int i=0; i<arraySize*2; i++) {
		dynamicMem[i] = std::rand();
	}


	// Read the values we wrote outside of the array size
/*	double sum = 0.;
	for (unsigned int i=0; i<arraySize*2; i++) {
		sum += (double)dynamicMem[i];
	}
	cout << "sum=" << sum << endl;
*/
}

int main(int /*argc*/, char** /*argv*/)
{
	myClass obj1(100);
	//obj1.Print();

	for (unsigned int iIter=0; iIter<100; iIter++) {
		unsigned int rndItemIndex = std::rand() % obj1.fN;
		unsigned int rndItemKey = obj1.GetItemKey(rndItemIndex);
		unsigned int foundVal1 = obj1.GetItem1(rndItemKey);
		unsigned int foundVal2 = obj1.GetItem2(rndItemKey);

		/*cout << "Searching for item " << rndItemIndex 
		     << "\twhich has the key\t" << rndItemKey << ":\t";
		cout << foundVal1 << ",\t" << foundVal2 << endl;*/
	}

	erroneousCode();

	return 0;
}
