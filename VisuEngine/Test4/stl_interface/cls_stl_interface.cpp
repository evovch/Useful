#include "cls_stl_interface.h"

// STD
#include <cstdio>

// Project
#include "cls_stl_file.h"

extern FILE* yyin;
extern int yyparse(cls_stl_file*& myStlFile);

cls_stl_interface::cls_stl_interface()
{
}

cls_stl_interface::~cls_stl_interface()
{
}

cls_stl_file* cls_stl_interface::Import(const char* p_filename)
{
    FILE* v_inFile = fopen(p_filename, "r");

    if (!v_inFile) {
    	fprintf(stderr, "[ERROR] Failed to open file '%s'\n", p_filename);
    	return nullptr;
    } else {
        yyin = v_inFile;
        cls_stl_file* v_stl_file;
        /*int res = */yyparse(v_stl_file);
        fclose(v_inFile);
        return v_stl_file;
    }
}
