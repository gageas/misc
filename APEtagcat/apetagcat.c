#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char BYTE;

const char APETAG_MARK[] = "APETAGEX";
#define APETAG_FOOTER_SIZE (32)
#define ERROR(x) {printf("Error(%s) %s:%d\n",x,__FILE__,__LINE__);exit(1);}
#define EXITIFZERO(x) if((x) == 0)ERROR("unexpected zero")
#define EXITIFNONZERO(x) if((x) != 0)ERROR("unexpected non-zero")
#define EXITIFNEGATIVE(x) if((x) < 0)ERROR("unexpected negative")

int main(int argc,char* argv[]){
	if(argc <= 1)return 1;
	FILE *src,*dest;
	EXITIFZERO(src = fopen(argv[1],"rb"));
	if(argc <= 2){
		dest = stdout;
	}else{
		EXITIFZERO(dest = fopen(argv[2],"wb"));
	}

	EXITIFNEGATIVE(fseek(src,-APETAG_FOOTER_SIZE,SEEK_END));

	BYTE footer[APETAG_FOOTER_SIZE];
	EXITIFZERO(fread(&footer,APETAG_FOOTER_SIZE,1,src));
	EXITIFNONZERO(memcmp(footer,APETAG_MARK,strlen(APETAG_MARK)));

	// FIXME: if you use BE CPU
	size_t size = *(size_t*)(footer+12);
	BYTE* tagbody;
	EXITIFZERO(tagbody = malloc(size));
	EXITIFNEGATIVE(fseek(src,-size,SEEK_END));
	EXITIFZERO(fread(tagbody,size,1,src));
	
	EXITIFZERO(fwrite(footer,APETAG_FOOTER_SIZE,1,dest));
	EXITIFZERO(fwrite(tagbody,size,1,dest));
	fclose(src);
	fclose(dest);
	return 0;
}
