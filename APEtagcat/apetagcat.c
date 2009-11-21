#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char BYTE;

const char* APETAG_MARK = "APETAGEX";
#define APETAG_FOOTER_SIZE (32)
#define ERROR(x) {printf("Error(%s) %s:%d\n", x, __FILE__, __LINE__); exit(1);}
#define EXITIFZERO(x) if ((x) == 0)ERROR("unexpected zero")
#define EXITIFNONZERO(x) if ((x) != 0)ERROR("unexpected non-zero")
#define EXITIFNEGATIVE(x) if ((x) < 0)ERROR("unexpected negative")

int main (int argc,char* argv[]){
	FILE *src,*dest;

	// check arguments and open file
	if (argc <= 1) return 1;
	EXITIFZERO (src = fopen (argv[1],"rb"));
	if (argc <= 2){
		dest = stdout;
	} else {
		EXITIFZERO (dest = fopen (argv[2],"wb"));
	}

	// read & check apetag footer
	EXITIFNEGATIVE (fseek (src, -APETAG_FOOTER_SIZE, SEEK_END));
	BYTE footer [APETAG_FOOTER_SIZE];
	EXITIFZERO (fread (&footer, APETAG_FOOTER_SIZE, 1, src));
	EXITIFNONZERO (memcmp (footer, APETAG_MARK, strlen (APETAG_MARK)));

	// read tag body
	size_t tagsize = *(size_t*) (footer+12); // FIXME: if you use BE CPU
	BYTE* tagbody;
	EXITIFZERO (tagbody = malloc (tagsize));
	EXITIFNEGATIVE (fseek (src, -tagsize, SEEK_END));
	EXITIFZERO (fread (tagbody, tagsize, 1, src));
	fclose (src);
	
	// write tag
	EXITIFZERO (fwrite (footer, APETAG_FOOTER_SIZE, 1, dest));
	EXITIFZERO (fwrite (tagbody, tagsize, 1, dest));
	fclose (dest);

	return 0;
}
