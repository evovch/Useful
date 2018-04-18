/**

	@class cls_aneu_file

**/

#ifndef CLS_ANEU_FILE_H
#define CLS_ANEU_FILE_H

// STD
#include <string>

class cls_model;

class cls_aneu_file
{
	friend class cls_aneu_interface;

public:
	cls_aneu_file();
	~cls_aneu_file();

	void Export(std::string p_filename) const;

	void BuildModel(cls_model* p_model) const;

private:
	unsigned int mNnodes;
	unsigned int mNelements;
	unsigned int mNtriangles;

	float* mNodes; // [mNnodes*3]
	unsigned int* mElements; //TODO
	unsigned int* mTriangles; // [mNtriangles*3]
	unsigned int* mTriangAttr; // [mNtriangles*3]


};

#endif // CLS_ANEU_FILE_H
