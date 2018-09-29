/**
 * Create a directory 'test' in the current directory.
 * If it already exists - do nothing.
 * If name 'test' already exists in the current directory
 * and it does not correspond to a directory - do nothing.
 */

#include <sys/stat.h>
#include <cstdio>

int main(int /*argc*/, char** /*argv*/)
{
	struct stat info;

	if( stat( "test", &info ) != 0 ) {
		mkdir("test", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		fprintf(stdout, "Created directory 'test'.\n");
	} else if( info.st_mode & S_IFDIR ) {
		fprintf(stdout, "Directory 'test' already exists.\n");
	} else {
		fprintf(stdout, "'test' already exists and it is not a directory.\n");
	}
	return 0;
}
