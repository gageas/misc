/* wpp2raw 確実動作版　ワーニングでまくりエディション。 */
#include <stdio.h>
#include <stdlib.h>
char *wppfile;
char expfile[255];
FILE *fp_wpp;
FILE *fp_exp;

struct wppheader{   /*wppファイルヘッダの構造体宣言*/
	int width;
	int height;
	int bytepp;
	int bitpp;
	unsigned long int fsize;
};

struct pixel {  /*画素情報の構造体*/
	unsigned int red;
	unsigned int green;
	unsigned int blue;
};

struct wppheader read_header_info(FILE *read_fp){

	unsigned char header[25];
	struct wppheader thisheader;

	if(fgets(header, 25, read_fp) == NULL){printf("Error can't read file header");return;}

	thisheader.width  = header[0x15]*256+header[0x14];
	thisheader.height = header[0x17]*256+header[0x16];
	thisheader.bytepp = header[0x00];
	thisheader.bitpp  = header[0x0C];
	thisheader.fsize  = ((header[0x13]*256+header[0x12])*256+header[0x11])*256+header[0x10];

	printf("[ \"%s\" Infomation]\n\n",wppfile);
	printf("Byte/pixel : %d\n",thisheader.bytepp);
	printf("Bit/pixel  : %d\n",thisheader.bitpp);
	printf("Resolution : %d X %d\n" , thisheader.width , thisheader.height);
	printf("DataSize   : %lu (Byte)\n",thisheader.fsize); 

	printf("PhotoShop-info : %d * %d 3ch 8bit Interlieved",thisheader.width,thisheader.height);

	return thisheader;
}



struct pixel get_px(struct wppheader header,int x,int y){
	struct pixel thispx;
	unsigned char red[1];
	unsigned char blue[1];

	blue[0]=getc(fp_wpp);
	red[0] =getc(fp_wpp);


	fputc((red[0]|0x07),fp_exp);
	fputc((((red[0]&0x07)<<5)|((blue[0]>>3)&0x1C)|0x03),fp_exp);
	fputc((((blue[0]&0x1F)<<3)|0x07),fp_exp);


	
	return thispx;
}

int main(int argc, char** argv){
	int i,j=0;
	int width=0;
	int height=0;
	struct wppheader myheader;
	struct pixel thispx;
    char *datastr;
	struct pixel *image;

	
	if (argc!=2){
		printf("Usage\n\n>wpp2htm hoge.wpp\n\n");
		return 0;
	}
	wppfile=argv[1];
	i=0;while(expfile[i]=argv[1][i]){i++;}	expfile[i-3]='r'; 	expfile[i-2]='a'; 	expfile[i-1]='w';
	if((fp_wpp=fopen(wppfile,"rb")) == NULL) {printf ("Error can't open file \"%s\"",wppfile);return 0;}
	if((fp_exp=fopen (expfile,"wb")) == NULL) {printf ("Error can't open file \"%s\"",wppfile);return 0;}
	
	myheader = read_header_info(fp_wpp);


   /* データ部分のためのメモリを確保 */
/*
   datastr = (char *)malloc(2);
   datastr = (char *)malloc(myheader.fsize);
   if(datastr == NULL) {
      printf("メモリが確保できません\n");
      return 0;
   }
*/

   /* 画像情報保存のためのメモリを確保 */
/*
   image = (struct pixel *)malloc(myheader.width*myheader.height*sizeof(struct pixel));
   image = (struct pixel *)malloc(6);

   if(image == NULL) {
      printf("メモリが確保できません\n");
      return 0;
   }

*/

	if(fgets(datastr, myheader.fsize, fp_wpp) == NULL){printf("Error can't read file data structure");return;}

	
	printf("Writing \"%s\"...\n",expfile);
	

	for (i=0;i<myheader.height;i++){
		for (j=0;j<myheader.width;j++){
			get_px(myheader,i,j);
		}
	}

	free(datastr);
	free(image);
	fclose (fp_wpp);
	fclose (fp_exp);
	printf("done.\n");
	return 0;
}
/******************************************************************************
EOF
2004.04.09
********************************************************************************/

