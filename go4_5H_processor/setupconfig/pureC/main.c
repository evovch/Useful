#include <stdio.h> // for fprintf

#include "structs.h"
#include "functions.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		fprintf(stderr, "Please, specify input setup config XML file.\n");
		return 1;
	}

	stc_setup_config obj1;
	InitStcSetupConfig(&obj1);

	ImportXML(&obj1, argv[1]);

	DumpStcSetupConfig(&obj1);

	DestructStcSetupConfig(&obj1);

	return 0;
}
