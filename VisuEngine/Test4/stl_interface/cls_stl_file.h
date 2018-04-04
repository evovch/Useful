/**

	@class cls_stl_file

**/

#ifndef CLS_STL_FILE_H
#define CLS_STL_FILE_H

// STD
#include <string>

// Project
#include "stl_structs.h"

class cls_model;

class cls_stl_file
{
public:
	cls_stl_file();
	~cls_stl_file();

	void SetName(const char* p_name) { mName = p_name; } //TODO check
	void SetFirstFacet(struct facet_t* p_firstfacet) { mFirstFacet = p_firstfacet; }

	void Export(std::string p_filename) const;

	void BuildModel(cls_model* p_model) const;

private:
	std::string mName;
	struct facet_t* mFirstFacet;

};

#endif // CLS_STL_FILE_H
