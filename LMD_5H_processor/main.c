#include <stdio.h>

// These includes are missing somewhere deep in the MbsAPI code
//#include <netdb.h>
//#include <netinet/in.h>

#include "fLmd.h"

int main(int argc, char** argv)
{
    /*fprintf(stderr, "Usage: ./prog [input lmd file]\n");
    return 1;*/
    char v_filename[256];
    if (argc == 1) {
        const char* v_fn = "/home/evovch/Documents/h5_02_0005.lmd";
        strcpy(v_filename, v_fn);
    } else {
        strcpy(v_filename, argv[1]);
    }

    fprintf(stderr, "Trying to open file '%s'.\n", v_filename);

    void* fControl = fLmdAllocateControl();
    uint32_t fLastError;

    fLastError = fLmdGetOpen((sLmdControl*) fControl,
                             v_filename,
                             LMD__INTERNAL_HEADER,
                             1024,
                             LMD__NO_INDEX);

    if (fLastError == LMD__SUCCESS) {
        fprintf(stderr, "File '%s' successfully read.\n", argv[1]);
    } else {
    	return 1;
    }

    return 0;
}
