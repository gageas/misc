#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 1024

typedef unsigned char uchar;
typedef unsigned long ulong;
typedef enum { JPG , PNG , GIF , BMP , NA } ftype;
typedef struct {
	ulong width;
	ulong height;
	ftype type;
} IMAGE;

typedef (*imgSubProc)(FILE*,IMAGE*,uchar*);
imgSubProc imgProcs[];
ftype detect_type(uchar*);
void show_usage(void);
int analyze_file(uchar*,IMAGE*);

int main(int argc,char* argv[]){
	IMAGE img;
	int i = 0;
	uchar buffer[BUFSIZE];

	if(argc==1){
		while( fgets(buffer,BUFSIZE,stdin) != NULL ){
			if( buffer[strlen(buffer)-1] == '\n' )buffer[strlen(buffer)-1]='\0';
			showit(buffer,&img);
		}
	}else{
		if( (strcmp(argv[1],  "-h"  ) == 0) ||
		    (strcmp(argv[1],"--help") == 0) ){
			show_usage();
			exit(1);
		} 
		for(i=1;i<argc;i++){
			showit(argv[i],&img);
		}
	}
	return 0;
}

int showit(uchar* fname,IMAGE* img){
	int err = 0;
	err = analyze_file(fname,img);
	if(err || (img->type == NA)){
		printf("%s : Could not get size\n",fname);
	}else{
		printf("%s : %dx%d\n",fname,img->width,img->height);
	}
	return err;
}

/*
	analyze_file
	
	ファイル名fnameのファイルを解析し、その情報をimgにセットする。

	引数 ファイル名 fname,IMAGE構造体* img
	返値 成功:0 エラー:not 0
	
	fp,img,およびファイルの先頭32バイトをファイルタイプごとの関数に渡し解析させる
*/
int analyze_file(uchar* fname,IMAGE* img){
	FILE *fp = NULL;
	int err = 0;
	uchar head32[32];

	if((fp=fopen(fname,"rb")) == NULL)return 1;
	if(fread(head32,1,32,fp) < 32)return 1;
	
	img->type = detect_type(head32);
	fseek(fp,0,SEEK_SET);
	err = imgProcs[img->type](fp,img,head32);
	fclose(fp);
	return err;
}

/*
	detect_type
	
	ファイルタイプを検出。

	引数 ファイルの先頭32バイト buffer
	返値 ftype列挙体 失敗でftype.NAを返す
*/
ftype detect_type(uchar* buffer){
    if( (buffer[0] == 0xFF) && (buffer[1] == 0xD8) ){
		return JPG;
	}else if( (buffer[0] == 0x89) && (buffer[1] == 'P') && (buffer[2] == 'N') && (buffer[3] == 'G') ){
		return PNG;
	}else if( (buffer[0] == 'G') && (buffer[1] == 'I') && (buffer[2] == 'F') ){
		return GIF;
	}else if( (buffer[0] == 'B') && (buffer[1] == 'M') ){
		return BMP;
	}
	return NA;
}

int get_jpeg(FILE* fp,IMAGE* img,uchar* head32){
	uchar buffer[10];
	if( fseek(fp,2,SEEK_SET) == -1 )return 1;
	while( 1 ){
		if(fread(buffer,1,4,fp) < 4)return 1;
		if( buffer[0] != 0xFF ){return 1;}
        if( (buffer[1] < 0xC0) || (buffer[1] > 0xC3) ){
            if( fseek(fp,(buffer[2]<<8)+buffer[3]-2,SEEK_CUR) == -1)return 1;
            continue;
        }
		if(fread(buffer+4,1,5,fp) < 5)return 1;
		img->height = (buffer[5]<<8)+buffer[6];
		img->width  = (buffer[7]<<8)+buffer[8];
		return 0; // success
	}
	return 1; // fail
}

int get_png(FILE* fp,IMAGE* img,uchar* head32){
	head32+=16;
	img->width  = (head32[0]<<24) + (head32[1]<<16) + (head32[2]<<8) + head32[3];
	img->height = (head32[4]<<24) + (head32[5]<<16) + (head32[6]<<8) + head32[7];
	return 0;
}

int get_bmp(FILE* fp,IMAGE* img,uchar* head32){
	head32+=14;
	if(head32[3] == 12){ // OS/2
		img->width  = (head32[5]<<8) + head32[4];
		img->height = (head32[7]<<8) + head32[6];
	}else{ // Win
		img->width  = (head32[ 7]<<24) + (head32[ 6]<<16) + (head32[5]<<8) + head32[4];
		img->height = (head32[11]<<24) + (head32[10]<<16) + (head32[9]<<8) + head32[8];
	}
	return 0;
}

int get_gif(FILE* fp,IMAGE* img,uchar* head32){
	img->width  = (head32[7]<<8) + head32[6];
	img->height = (head32[9]<<8) + head32[8];
	return 0;
}
int get_na(FILE* fp,IMAGE* img,uchar* head32){
	return 1;
}

void show_usage(){
	printf ("imgsize :: get width & height of image file\n\n"
	        "   Usage: \n"
	        "      imgsize a.jpg b.jpg c.png d.bmp ...\n");
	return;
}
imgSubProc imgProcs[5] = { &get_jpeg, &get_png, &get_gif, &get_bmp, &get_na };
