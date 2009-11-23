//#include <stdio.h>
//#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
//#include <string.h>
#include "id3test.h"
#define PATHMAX 500


int read_header(unsigned char *headerstr,struct id3v2header *header){
	strncpy_s(header->magick_number,4,headerstr,3); /* ID3 */
	header->magick_number[3]='\0';
	if(strcmp(header->magick_number,"ID3") != 0){MessageBox(NULL,_T("no ID3 header\n"),_T("Info"),MB_OK);return 1;}

	switch((headerstr[3]<<8)+headerstr[4]){
		case 0x0200:
			header->version=2;
			if(headerstr[5]&0x40){printf("Error : Compressed tag!!\n");return 1;}
			header->flag=headerstr[5]&0x80;
			break;
		case 0x0300:
			header->version=3;
			header->flag=headerstr[5]&(0x80+0x40+0x20);
			break;
		case 0x0301:
		case 0x0400:
			header->version=4;
			header->flag=headerstr[5]&(0x80+0x40+0x20+0x10);
			break;
		default:
			MessageBox(NULL,_T("Unknown version"),_T("Info"),MB_OK);
			return 1;
	}
	header->size=(headerstr[6]<<21)+(headerstr[7]<<14)+(headerstr[8]<<7)+headerstr[9];

	return 0;  /*����I����0��Ԃ�*/
}

long read_frame(struct frame *frame,unsigned char *in_buf,unsigned char *out_buf,size_t out_buf_size,int version){/* FILE *fp */
	/* �ǂݍ��񂾃o�C�g����Ԃ��i�܂�:�t���[���w�b�_��10�o�C�g�j�G���[��0��Ԃ��B */
	size_t readsize;
	size_t headsize;
	unsigned char *tmp;
	size_t offset=0;
	int i;

	headsize=(version==2)?6:10;
	frame->id_x=0; /* �ǂ�ł��Ȃ� */
	switch(version){
		case 2:
			strncpy_s(frame->id,5,in_buf,3);
			frame->id[3]='\0';
			frame->size=(in_buf[3]*256+in_buf[4])*256+in_buf[5];
			frame->flag=0;

			for(i=0;i<LENGTH(FRAMES);i++){
				if(strcmp(frame->id,FRAMES[i].name2)==0){
					frame->id_x=i;
					break;
				}
			}
			break;
		case 3:
			strncpy_s(frame->id,5,in_buf,4);
			frame->id[4]='\0';
			frame->size=((in_buf[4]*256+in_buf[5])*256+in_buf[6])*256+in_buf[7];
			frame->flag = (in_buf[8]<<7) + ((in_buf[9]>>4)&0x0F) + ((in_buf[9]<<1)&0x40);

			for(i=0;i<LENGTH(FRAMES);i++){
				if(strcmp(frame->id,FRAMES[i].name3)==0){
					frame->id_x=i;
					break;
				}
			}
			break;
		case 4:
			strncpy_s(frame->id,5,in_buf,4);
			frame->id[4]='\0';
			frame->size=((in_buf[4]*128+in_buf[5])*128+in_buf[6])*128+in_buf[7];
			frame->flag = (in_buf[8]<<8)+in_buf[9];

			for(i=0;i<LENGTH(FRAMES);i++){
				if(strcmp(frame->id,FRAMES[i].name4)==0){
					frame->id_x=i;
					break;
				}
			}
			break;
	}
	if(strcmp(frame->id,"")==0){return (0);} /* ID��""�Ȃ�I�� */

	readsize=headsize+frame->size;
	//if(frame->size > totalsize){MessageBox(NULL,_T("�j�����Ă���ۂ�"),_T(""),MB_OK);return 0;}

	/* size����2�o�C�g�]�v�Ɋm�ۂ��ăP�c��\0\0�������܂��i�\���j�E�E�E */
	/* Wchar(UTF16)�̂Ƃ��A�I�[��\0��2�K�v������ */
	if((tmp=(unsigned char *)malloc(frame->size+2)) == NULL){alert(_T("malloc err"));exit(1);};

	if(frame->flag&FLAG_FRAME_UNSYNC){ /* �񓯊������� */
		frame->size = de_unsync(tmp,in_buf+headsize,frame->size);
	}else{
		memcpy(tmp,in_buf+headsize,frame->size);
	}

	tmp[frame->size  ]='\0'; /* �����I�[ */
	tmp[frame->size+1]='\0';

	if(frame->flag&FLAG_FRAME_DATALENGTH){ /* ���T�C�Y�����΂� */
		offset=4;
		frame->size-=4;
	}

	if(frame->size+2 > out_buf_size){MessageBox(NULL,_T("Too Short Memory"),_T(""),MB_OK);return (out_buf_size-frame->size-3);}

	frame->value=out_buf;
	switch(FRAMES[frame->id_x].type){
		case FR_APIC:
			memcpy(frame->value,tmp+offset,frame->size);
			frame->body=NULL;
			frame->encode=frame->value[0];
			/* ID3V2.2�����̂܂�3.3�Ƃ��ɕϊ�����z������悤�Ȃ̂� */
			if(  ((frame->value[1]=='J')&&(frame->value[2]=='P')&&(frame->value[3]=='G'))  ||
				 ((frame->value[1]=='P')&&(frame->value[2]=='N')&&(frame->value[3]=='G'))){
				 //JPG�̂Ƃ�
				if((frame->value[1]=='J')&&(frame->value[2]=='P')&&(frame->value[3]=='G')){
					frame->mime=JPG;
				}else if((frame->value[1]=='P')&&(frame->value[2]=='N')&&(frame->value[3]=='G')){
					frame->mime=PNG;
				}
				frame->picture_type=frame->value[4];
				frame->image_description=frame->value+5;
				frame->imagebody=strchr(frame->image_description,'\0')+1;
				frame->size-=(frame->imagebody) - (frame->value);
			}else{ //mime�����̂Ƃ�
				if(strcmp(frame->value+1,"image/jpeg")==0 || strcmp(frame->value+1,"image/jpg")==0){
					frame->mime=JPG;
				}else if(strcmp(frame->value+1,"image/png")==0){
					frame->mime=PNG;
				}else{
					frame->mime=BIN;
				}
				frame->picture_type=*(strchr(frame->value+1,'\0')+1);
				frame->image_description = strchr(frame->value+1,'\0')+2; //mime�̏I�[��NULL�ʒu+picture_type1�o�C�g+1
				frame->imagebody = strchr(frame->image_description,'\0')+1;
				frame->size-=(frame->imagebody) - (frame->value);
			}
			break;
		default:
			frame->imagebody=NULL;
			frame->mime='\0';
			frame->picture_type='\0';

			if(tmp[0]<0x04){
				frame->encode=tmp[0];
				switch(frame->encode){
					case ISO_8859_1:
						frame->size=MultiByteToWideChar(CP_ACP,0,tmp+1,frame->size,(LPWSTR)frame->value,out_buf_size);
						break;
					case UTF_16:
						memcpy(frame->value,tmp+1,frame->size+1);
						frame->size=frame->size/2;
						break;
					case UTF_16_BE:
						break;
					case UTF_8:
						frame->size=MultiByteToWideChar(CP_UTF8,0,tmp+1,frame->size,(LPWSTR)frame->value,out_buf_size);
						break;
				}
				frame->body=(wchar_t *)frame->value;
				frame->size--; /* encode���̕�1�o�C�g�}�C�i�X */
			}else{
				frame->size=MultiByteToWideChar(CP_ACP,0,tmp,frame->size,(LPWSTR)frame->value,out_buf_size);
				frame->encode=UNDEF;
				frame->body = (wchar_t *)frame->value;
			}

			if(FRAMES[frame->id_x].type==FR_TXT_EX ||  FRAMES[frame->id_x].type==FR_TXT_EX_LNG){ /* TXXX,COMM�Ȃǂ̂Ƃ� */
				frame->extid=frame->body;
				frame->extvalue=wcschr(frame->body,'\0')+1;
			}else{
				frame->extid=frame->extvalue=NULL;
			}
			break;
	}

	free(tmp);
	return (readsize); /* ����I�� */
}

int read_id3tag(FILE *fp,struct id3tag **ptag){
	unsigned char headerstr[11];/* ID3�^�O�w�b�_�i�[�p */
	unsigned char ext_header[5];/* �g���w�b�_�p */
	unsigned char *id3info;
	size_t offset=0;
	size_t filesize;
	long tmp=0;
	int count=0;
	struct id3tag *tag;
	tag=*ptag;

	fseek(fp,0,SEEK_END);
	filesize=ftell(fp);
	fseek(fp,0,SEEK_SET);
	if(filesize < 10)return -1;
	if(fread(headerstr, 10,1, fp) != 1){printf("Error can't read file header");return -1;}
	if(read_header(headerstr,&tag->head)){return -1;}
	if(tag->head.size > filesize){printf("Too small file");return -1;}
	
	if(tag->head.flag&FLAG_HEAD_EXTENSION){/* �g���w�b�_������Ƃ�������X�L�b�v */
		if(fread(ext_header, 4,1, fp) != 1){printf("Error can't read Ext. header");return -1;}
		fseek(fp,(ext_header[0]<<24)+(ext_header[1]<<16)+(headerstr[2]<<8)+headerstr[3],SEEK_CUR);
	}

	/* frame */
	tag->frame_buf_size=tag->head.size*2;
	if((id3info=(unsigned char *)malloc(tag->head.size)) == NULL){printf("couldn't alloc buffer\n");return -1;}/* �f�B�X�N����ǂݍ��ރo�b�t�@ */
	if((tag->frame_buf=(unsigned char *)malloc(tag->frame_buf_size)) == NULL){printf("couldn't alloc buffer\n");return -1;} /* �ǂݍ��񂾂��̂𕶎��R�[�h�ϊ��Ȃǂɂ������f�[�^������o�b�t�@ */

	if(fread(id3info, tag->head.size,1, fp) != 1){printf("Error can't read frame header\n");return -1;}
	while(offset < tag->head.size){
		
		if((tag = (struct id3tag *)realloc(tag,sizeof(struct id3tag)+sizeof(struct frame)*count)) == NULL){printf("couldn't realloc memory : myid3tag\n");return -1;}

		tmp = read_frame(&tag->frame[count],&id3info[offset],tag->frame_buf+offset*2,tag->frame_buf_size-offset*2,tag->head.version);
		if(tmp == 0)break; //����
		if(tmp <0){ //�������s��
			tag->frame_buf_size-=tmp;
			if((tag->frame_buf=(unsigned char *)realloc(tag->frame_buf,tag->frame_buf_size)) == NULL){printf("couldn't alloc buffer\n");return -1;} /* �ǂݍ��񂾂��̂𕶎��R�[�h�ϊ��Ȃǂɂ������f�[�^������o�b�t�@ */
		}else{
			offset+=tmp;
			count++;
		}
	}
	free(id3info);
	*ptag=tag;
	return count;

}

size_t de_unsync(unsigned char *dest,unsigned char *src,size_t size){/* �񓯊������������� */
	/* �����F�񓯊�����ő�������e�ʂ͒P���Ȋm���ōl����0.4%���x�B */
	/* �@�@�@�Ȃ̂ŁA1�`2%�]���Ɋm�ۂ���΃������s���ɂ͂Ȃ�Ȃ��Ǝv���B */
	size_t i=0,pos=0;
	for(i=0;i<size;i++,pos++){
		dest[pos]=src[i];
		if( (src[i] == 0xFF) && (src[i+1] == 0x00) ){i++;}
	}
	printf("UNSYNC Ratio : %f %%\n",(double)size/pos);
	return pos;/* ������̃o�C�g����Ԃ� */
}

size_t en_unsync(unsigned char *dest,unsigned char *src,size_t src_size,size_t dest_size){
	size_t i=0,pos=0;
	for(i=0;i<src_size;i++,pos++){
		if(pos>=dest_size){return 0;}/* �T�C�Y������Ȃ��ꍇ */
		dest[pos]=src[i];
		if(dest[pos]==0xFF){dest[pos+1]=0x00;pos++;}
	}
	return pos; /* �񓯊�����̃o�C�g����Ԃ� */
}

void print_frame_all(struct id3tag *tag,int cnt,char *path){
	FILE *fp_apic;
	char tmp_mbyte[501];
	wchar_t *tmp=NULL;
	wchar_t buf[8001]=_T("");
	wchar_t buf_t[1001];
	wchar_t id[5];
	int len;
	char apic_filename[PATHMAX+13];
	int i,apiccnt=1;
	struct frame *frm;

	for(i=0;i<cnt;i++){
		frm=&tag->frame[i];
		MultiByteToWideChar(CP_ACP,(DWORD)0,frm->id,-1,id,5);
		_stprintf_s(buf_t,1000,_T("%s %s[%u byte][%s][flag=0x%04X] = "),FRAMES[frm->id_x].desc,id , frm->size,ENCODE_STRING[frm->encode] , frm->flag);
		_tcsncat_s(buf,8000,buf_t,1000);
		switch(FRAMES[frm->id_x].type){
			case FR_APIC:
				sprintf_s(apic_filename,513,"%s%d.%s",path,apiccnt++,EXTENSION[frm->mime]);

				len = sprintf_s(tmp_mbyte,501,"=>> %s \"%s\" \n",EXTENSION[frm->mime],apic_filename);
				if((tmp=(wchar_t *)realloc(tmp,(len+1)*2)) == NULL){printf("!!!!!!");}
				len=MultiByteToWideChar(CP_ACP,(DWORD)0,tmp_mbyte,-1,tmp,(frm->size+1)*2);
				_tcsncat_s(buf,8000,tmp,len);

				if(fopen_s(&fp_apic,apic_filename,"wb")!=0){MessageBox(NULL,_T("coundn't open APIC output file"),_T(""),MB_OK);continue;}
				fwrite(frm->imagebody,frm->size,1,fp_apic);
				fclose(fp_apic);
				break;
			case FR_TXT_EX:
			case FR_TXT_EX_LNG:
				_stprintf_s(buf_t,1000,_T(" %s : %s\n"),frm->extid,frm->extvalue);
				_tcsncat_s(buf,8000,buf_t,1000);
				break;
			case FR_TXT:
			case FR_TXT_PURE:
				_stprintf_s(buf_t,1000,_T(" %s\n"),frm->body);
				_tcsncat_s(buf,8000,buf_t,1000);
				break;
			default:
				_stprintf_s(buf_t,1000,_T(" (�o�C�i���A�\���s��)\n"));
				_tcsncat_s(buf,8000,buf_t,1000);
				break;
		}
	}
	MessageBox(NULL,buf,(LPCWSTR)_T(""),MB_OK);

	return;
}


int WINAPI WinMain( HINSTANCE p_hInstance, HINSTANCE p_hPreInst, LPSTR p_pchCmdLine, int p_iCmdShow ){
	FILE *fp;
	enum eEncode encode;

	char path[PATHMAX]="";
	char mbox_tmp[5000];

	int cnt=0,i=0,j=0,apiccnt=1;
	struct id3tag *myid3tag=NULL;

	if(strcmp(p_pchCmdLine,"")==0){
		MessageBox(NULL,_T("argument required!\n"),_T(""),MB_OK);
		exit(1);
	}

	if(p_pchCmdLine[0]=='"'){
		p_pchCmdLine++;
		if(strchr(p_pchCmdLine,'"')){*strchr(p_pchCmdLine,'"')='\0';}
	}
	if(strrchr(p_pchCmdLine,'.')-p_pchCmdLine>=PATHMAX){
		printf("too long path\n");exit(1);
	}
	strncpy_s(path,PATHMAX,p_pchCmdLine,strrchr(p_pchCmdLine,'.')-p_pchCmdLine);

	if((myid3tag=(struct id3tag *)malloc(sizeof(struct id3tag))) == NULL){printf("couldn't alloc memory : myid3tag");return 0;}

	if(fopen_s(&fp,p_pchCmdLine,"rb") != 0){MessageBox (NULL,_T("open error"),_T(""),MB_OK);return 0;}
	if((cnt=read_id3tag(fp,&myid3tag)) <0){return 1;}

	sprintf_s(mbox_tmp,5000,"version:id3v2.%d\n" "header size:%uKB\n" "flag �񓯊���:%s\n" "flag �g��:%s\n" "flag ������:%s\n" "flag �t�b�^:%s\n",
		myid3tag->head.version,
		myid3tag->head.size/1024,
		(myid3tag->head.flag&FLAG_HEAD_UNSYNC?"yes":"no"),
		(myid3tag->head.flag&FLAG_HEAD_EXTENSION?"yes":"no"),
		(myid3tag->head.flag&FLAG_HEAD_TESTING?"yes":"no"),
		(myid3tag->head.flag&FLAG_HEAD_FOOTER?"yes":"no")
	);
	MessageBoxA(NULL,mbox_tmp,p_pchCmdLine,MB_OK);

	print_frame_all(myid3tag,cnt,path);

	free(myid3tag->frame_buf);
	free(myid3tag);
	return 0;
}
