#include <cstdio>

#include "../cls_stl_file.h"

extern FILE* yyin;
extern int yyparse(cls_stl_file*& myStlFile);

int main(int argc, char** argv)
{

    FILE* v_inFile = fopen("../input/humanoid.stl", "r");

    if (!v_inFile) {
    	fprintf(stderr, "Failed to open file.\n");
    	return 1;
    } else {
        yyin = v_inFile;
        cls_stl_file* v_stl_file;
        int res = yyparse(v_stl_file);
        fclose(v_inFile);
    }

    return 0;
}
