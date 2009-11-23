/* �w�b�_�t���O */
#define FLAG_HEAD_UNSYNC 0x80 
#define FLAG_HEAD_EXTENSION 0x40
#define FLAG_HEAD_TESTING 0x20
#define FLAG_HEAD_FOOTER 0x10


/* �t���[���̃t���O��v2.4�����B */
#define FLAG_FRAME_REMOVE_IF_TAG_MODIFY 0x4000 
#define FLAG_FRAME_REMOVE_IF_FILE_MODIFY 0x2000
#define FLAG_FRAME_READONLY 0x1000

#define FLAG_FRAME_GROUPED 0x0040
#define FLAG_FRAME_COMPRESSED 0x0008
#define FLAG_FRAME_CRYPTED 0x0004
#define FLAG_FRAME_UNSYNC 0x0002
#define FLAG_FRAME_DATALENGTH 0x0001


/* �o�[�W���� */
#define ID3V22 0x01
#define ID3V23 0x02
#define ID3V24 0x04
#define ALL_VER (ID3V22|ID3V23|ID3V24)


enum eEncode{ISO_8859_1 , UTF_16 , UTF_16_BE , UTF_8 ,UNDEF};
wchar_t ENCODE_STRING[][11]={_T("ISO(none)") , _T("UTF-16") , _T("UTF-16BE") , _T("UTF-8") , _T("-")};

enum MIME{JPG,PNG,GIF,BMP,BIN}; /* JPG��PNG���炢���낤���Ǘ\�񂵂Ƃ� */
char EXTENSION[][4]={"jpg" , "png" , "gif" , "bmp" , "bin" };

/* �t���[���̃^�C�v */
enum FRAME_TYPE{ FR_TXT , FR_TXT_PURE , FR_TXT_EX , FR_TXT_EX_LNG , FR_BIN , FR_BIN_EX , FR_APIC};

struct _frame_id{
	char name2[4];
	char name3[5];
	char name4[5];
	wchar_t *desc;
	char type;
	char ver;
};


const struct _frame_id FRAMES[]={
	{"","","",_T("?"),FR_BIN,ALL_VER}, /* �ǂ�ł��Ȃ� */
	{"CRA","AENC","AENC",_T("�Í���"),FR_BIN,ALL_VER},
	{"PIC","APIC","APIC",_T("�摜"),FR_APIC,ALL_VER},
	{""   ,""    ,"ASPI",_T("�I�[�f�B�I�V�[�P���X�|�C���g�C���f�b�N�X"),FR_BIN,ID3V24},
	{"COM","COMM","COMM",_T("�R�����g"),FR_TXT_EX_LNG,ALL_VER},
	{""   ,"COMR","COMR",_T("�R�}�[�V�����t���[��"),FR_BIN,ID3V23|ID3V24},
	{""   ,"ENCR","ENCR",_T("�Í����̎�@�̓o�^"),FR_BIN,ID3V23|ID3V24},
	{"EQU","EQUA","EQU2",_T("�ψꉻ"),FR_BIN,ALL_VER},
	{"ETC","ETCO","ETCO",_T("�C�x���g�^�C���R�[�h"),FR_BIN,ALL_VER},
	{"GEO","GEOB","GEOB",_T("�p�b�P�[�W�����ꂽ��ʓI�ȃI�u�W�F�N�g"),FR_BIN,ALL_VER},
	{""   ,"GRID","GRID",_T("�O���[�v���ʎq�̓o�^"),FR_BIN,ID3V23|ID3V24},
		{"IPL","IPLS","TIPL",_T("���͎�"),FR_TXT,ALL_VER}, //
		{"LINK","LINK","LINK",_T("�����N���"),FR_TXT_EX,ALL_VER}, //
	{"MCI","MCDI","MCDI",_T("���yCD���ʎq"),FR_BIN,ALL_VER},
	{"MLL","MLLT","MLLT",_T("MPEG���P�[�V�������b�N�A�b�v�e�[�u��"),FR_BIN,ALL_VER},

	{""   ,"PRIV","PRIV",_T("�v���C�x�[�g�t���[��"),FR_BIN_EX,ID3V23|ID3V24},
	{""   ,"GRID","GRID",_T("�O���[�v���ʎq�̓o�^"),FR_BIN,ID3V23|ID3V24},
	{"TAL","TALB","TALB",_T("�A���o��"),FR_TXT,ALL_VER},
	{"TBP","TBPM","TBPM",_T("BPM"),FR_TXT,ALL_VER},
	{"TCM","TCOM","TCOM",_T("��Ȏ�(Composer)"),FR_TXT,ALL_VER},
		{"TXT","TEXT","TEXT",_T("�쎌��/�����쐬��(TEXT)"),FR_TXT,ALL_VER},//
	{"TCO","TCON","TCON",_T("�W������"),FR_TXT,ALL_VER},
	{"TCR","TCOP","TCOP",_T("���쌠���"),FR_TXT,ALL_VER},
	{"TDA","TDAT","TDRC",_T("���t(�^��)"),FR_TXT,ALL_VER},
	{""   ,""    ,"TDEN",_T("���t(�G���R�[�h)"),FR_TXT,ID3V24},
	{"TOR","TORY","TDOR",_T("���t(�I���W�i�������[�X)"),FR_TXT,ALL_VER}, //TORY��TDOR�ł����������Ă��܂��܂�
	{""   ,""    ,"TDRL",_T("���t(�����[�X)"),FR_TXT,ID3V24},
	{""   ,""    ,"TDTG",_T("���t(�^�O�t��)"),FR_TXT,ID3V24},
	{"TEN","TENC","TENC",_T("�G���R�[�h�����l(Encoder)"),FR_TXT,ALL_VER},
	{"TFL","TFLT","TFLT",_T("�t�@�C���^�C�v"),FR_TXT,ALL_VER},
	{""   ,""    ,"TIPL",_T("�ւ�����l�X�̈ꗗ"),FR_TXT,ID3V24},
	{"TIM","TIME",""    ,_T("����(V2.4�ł͓��t(�^��)�̕���)"),FR_TXT,ID3V22|ID3V23},
	{"TT1","TIT1","TIT1",_T("���e�̑�����O���[�v"),FR_TXT,ALL_VER},
	{"TT2","TIT2","TIT2",_T("�^�C�g��"),FR_TXT,ALL_VER},
	{"TT3","TIT3","TIT3",_T("�T�u�^�C�g��"),FR_TXT,ALL_VER},
	{"TKE","TKEY","TKEY",_T("�n�߂̒�"),FR_TXT,ALL_VER},
	{"TLA","TLAN","TLAN",_T("����"),FR_TXT,ALL_VER},
	{"TLE","TLEN","TLEN",_T("����"),FR_TXT,ALL_VER},
	{""   ,""    ,"TMCL",_T("�~���[�W�V�����N���W�b�g���X�g"),FR_TXT,ID3V24},
	{"TMT","TMED","TMED",_T("���f�B�A�^�C�v"),FR_TXT,ALL_VER},
	{""   ,""    ,"TMOO",_T("���[�h"),FR_TXT,ID3V24},
	{"TOT","TOAL","TOAL",_T("�I���W�i���̃A���o��/�f��/�V���[�̃^�C�g��"),FR_TXT,ALL_VER},
	{"TOF","TOFN","TOFN",_T("�I���W�i���t�@�C����"),FR_TXT,ALL_VER},
	{"TOL","TOLY","TOLY",_T("�I���W�i���̍쎌��/�����쐬��"),FR_TXT,ALL_VER},
	{""   ,"TOWN","TOWN",_T("�t�@�C���̏��L��/���C�Z���V�["),FR_TXT,ID3V23|ID3V24},
	{"TP1","TPE1","TPE1",_T("�A�[�e�B�X�g"),FR_TXT,ALL_VER},
	{"TP2","TPE2","TPE2",_T("�o���h/�I�[�P�X�g��/���t"),FR_TXT,ALL_VER},
	{"TP3","TPE3","TPE3",_T("�w����/���t�ҏڍ׏��"),FR_TXT,ALL_VER},
	{"TP4","TPE4","TPE4",_T("�|���/���~�b�N�X/���̑��̏C��"),FR_TXT,ALL_VER},
	{"TPA","TPOS","TPOS",_T("�Z�b�g���̈ʒu"),FR_TXT,ALL_VER},
	{""   ,""    ,"TPRO",_T("Produced notice"),FR_TXT,ALL_VER},
	{"TPB","TPUB","TPUB",_T("�o�Ŏ�"),FR_TXT,ALL_VER},
	{"TRK","TRCK","TRCK",_T("�g���b�N�̔ԍ�/�Z�b�g���̈ʒu"),FR_TXT,ALL_VER},
	{"TRD","TRDA",""    ,_T("�^������"),FR_TXT,ID3V22|ID3V23}, //�Ƃ肠����
	{""   ,"TRSN","TRSN",_T("�C���^�[�l�b�g���W�I�ǂ̖��O"),FR_TXT,ID3V23|ID3V24},
	{""   ,"TRSO","TRSO",_T("�C���^�[�l�b�g���W�I�ǂ̏��L��"),FR_TXT,ID3V23|ID3V24},
	{""   ,""    ,"TSOA",_T("�A���o���̃\�[�g�I�[�_�["),FR_TXT,ID3V24},
	{""   ,""    ,"TSOP",_T("���t�҂̃\�[�g�I�[�_�["),FR_TXT,ID3V24},
	{""   ,""    ,"TSOT",_T("�^�C�g���̃\�[�g�I�[�_�["),FR_TXT,ID3V24},
	{"TSI","TSIZ",""    ,_T("�T�C�Y"),FR_TXT,ID3V22|ID3V23},
	{"TRC","TSRC","TSRC",_T("ISRC"),FR_TXT,ALL_VER},
	{"TSS","TSSE","TSSE",_T("�G���R�[�h�Ɏg�p�����\�t�g�E�F�A/�n�[�h�E�F�A�ƃZ�b�e�B���O"),FR_TXT,ALL_VER},
	{""   ,""    ,"TSST",_T("�Z�b�g�̃T�u�^�C�g��"),FR_TXT,ID3V24},
	{"TYE","TYER",""    ,_T("�N"),FR_TXT,ID3V22|ID3V23},
	{""   ,"TCMP","TCMP",_T("�R���s���[�V����(iTunes)"),FR_TXT,ID3V23|ID3V24},
	{"TXX","TXXX","TXXX",_T("���[�U�[��`�������t���[��"),FR_TXT_EX,ALL_VER},
	{"UFI","UFID","UFID",_T("��ӓI�ȃt�@�C�����ʎq"),FR_BIN_EX,ALL_VER},
	{""   ,"USER","USER",_T("�g�p����"),FR_TXT,ID3V23|ID3V24},
	{"ULT","USLT","USLT",_T("�񓯊��̎�/�����̃R�s�["),FR_TXT_EX_LNG,ALL_VER},
	{"WCM","WCOM","WCOM",_T("���Ə�̏��"),FR_TXT_PURE,ALL_VER},
	{"WCP","WCOP","WCOP",_T("���쌠/�@�I���"),FR_TXT_PURE,ALL_VER},
	{"WAF","WOAF","WOAF",_T("�I�[�f�B�I�t�@�C���̌���Web�y�[�W"),FR_TXT_PURE,ALL_VER},
	{"WAR","WOAR","WOAR",_T("�A�[�e�B�X�g/���t�҂̌���Web�y�[�W"),FR_TXT_PURE,ALL_VER},
	{"WAS","WOAS","WOAS",_T("�����̌���Web�y�[�W"),FR_TXT_PURE,ALL_VER},
	{""   ,"WORS","WORS",_T("�C���^�[�l�b�g���W�I�ǂ̌���Web�y�[�W"),FR_TXT_PURE,ID3V23|ID3V24},
	{""   ,"WPAY","WPAY",_T("�x����"),FR_TXT_PURE,ID3V23|ID3V24},
	{"WPB","WPUB","WPUB",_T("�o�Ŏ҂̌���Web�y�[�W"),FR_TXT_PURE,ALL_VER},
	{"WXX","WXXX","WXXX",_T("�o�Ŏ҂̌���Web�y�[�W"),FR_TXT_EX,ALL_VER},
	{"CRM",""    ,""    ,_T("�Í������^�t���[��"),FR_BIN,ID3V22},

	{"TDY","TDLY","TDLY",_T("�v���C���X�g�x������"),FR_TXT,ALL_VER},
	
	
};

struct id3v2header{
	char magick_number[4]; /* "ID3" */
	short version;
	unsigned char flag;
	size_t size;
};


struct frame{
	char id[5]; /* eg. TIT2 */
	char id_x;
	size_t size;
	int flag;
	unsigned char encode;
	unsigned char *value; /* �f�[�^�����́Bmalloc�Ŋm�ۂ����A�h���X */

	wchar_t *body; /* value��̖{���J�n�ʒu�B�ȉ��|�C���^�̂��͓̂��l */

	wchar_t *extid; /* for TXXX,COMM,etc... */
	wchar_t *extvalue;

	unsigned char mime; /* for APIC */
	unsigned char picture_type;
	unsigned char *image_description; /* �܂Ƃ��ɑΉ����Ă�\�t�g�ˁ[�� */
	unsigned char *imagebody;
};

struct id3tag{
	struct id3v2header head;
	unsigned char *frame_buf;
	size_t frame_buf_size;
	struct frame frame[1];
};

size_t de_unsync();
size_t en_unsync();

#define LENGTH(x) sizeof(x)/sizeof(x[0])

void alert(LPCWSTR mes){
	MessageBox(NULL,mes,_T(""),MB_OK);
}
