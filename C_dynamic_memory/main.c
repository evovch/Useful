#include <stdio.h>
#include <stdlib.h>

struct stc_A
{
	char fCrateName[64];
	unsigned short fCrateProcid;
	unsigned short fAddr;
	char fElblock[64];
	unsigned short fStartelectrch;
	unsigned short fNelectrch;
	unsigned short fStepelecrtch;
	char fDetector[64];
	unsigned short fStartdetch;
	char fFolder[64];
};

int main(int argc, char** argv)
{
	unsigned int nObjects;

	printf("MALLOC =====================================================================\n");

	nObjects = 4;
	struct stc_A* objArray = (struct stc_A*)malloc(nObjects*sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray);
	free(objArray);
	printf("%u:\t%p\n", nObjects, objArray);

	nObjects = 0;
	struct stc_A* objArray2 = (struct stc_A*)malloc(nObjects*sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray2);
	free(objArray2);
	printf("%u:\t%p\n", nObjects, objArray2);

	printf("CALLOC =====================================================================\n");

	nObjects = 4;
	struct stc_A* objArray3 = (struct stc_A*)calloc(nObjects, sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray3);
	free(objArray3);
	printf("%u:\t%p\n", nObjects, objArray3);

	nObjects = 0;
	struct stc_A* objArray4 = (struct stc_A*)calloc(nObjects, sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray4);
	free(objArray4);
	printf("%u:\t%p\n", nObjects, objArray4);


	printf("MALLOC & REALLOC ===========================================================\n");

	nObjects = 4;
	struct stc_A* objArray5 = (struct stc_A*)malloc(nObjects*sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray5);
	nObjects = 8;
	objArray5 = (struct stc_A*)realloc(objArray5, nObjects*sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray5);
	free(objArray5);
	printf("%u:\t%p\n", nObjects, objArray5);

	nObjects = 0;
	struct stc_A* objArray6 = (struct stc_A*)malloc(nObjects*sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray6);
	nObjects = 8;
	objArray6 = (struct stc_A*)realloc(objArray6, nObjects*sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray6);
	free(objArray6);
	printf("%u:\t%p\n", nObjects, objArray6);

	printf("CALLOC & REALLOC ===========================================================\n");

	nObjects = 4;
	struct stc_A* objArray7 = (struct stc_A*)calloc(nObjects, sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray7);
	nObjects = 8;
	objArray7 = (struct stc_A*)realloc(objArray7, nObjects*sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray7);
	free(objArray7);
	printf("%u:\t%p\n", nObjects, objArray7);

	nObjects = 0;
	struct stc_A* objArray8 = (struct stc_A*)calloc(nObjects, sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray8);
	nObjects = 8;
	objArray8 = (struct stc_A*)realloc(objArray8, nObjects*sizeof(struct stc_A));
	printf("%u:\t%p\n", nObjects, objArray8);
	free(objArray8);
	printf("%u:\t%p\n", nObjects, objArray8);

	return 0;
}
