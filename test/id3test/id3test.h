/* ヘッダフラグ */
#define FLAG_HEAD_UNSYNC 0x80 
#define FLAG_HEAD_EXTENSION 0x40
#define FLAG_HEAD_TESTING 0x20
#define FLAG_HEAD_FOOTER 0x10


/* フレームのフラグはv2.4準拠。 */
#define FLAG_FRAME_REMOVE_IF_TAG_MODIFY 0x4000 
#define FLAG_FRAME_REMOVE_IF_FILE_MODIFY 0x2000
#define FLAG_FRAME_READONLY 0x1000

#define FLAG_FRAME_GROUPED 0x0040
#define FLAG_FRAME_COMPRESSED 0x0008
#define FLAG_FRAME_CRYPTED 0x0004
#define FLAG_FRAME_UNSYNC 0x0002
#define FLAG_FRAME_DATALENGTH 0x0001


/* バージョン */
#define ID3V22 0x01
#define ID3V23 0x02
#define ID3V24 0x04
#define ALL_VER (ID3V22|ID3V23|ID3V24)


enum eEncode{ISO_8859_1 , UTF_16 , UTF_16_BE , UTF_8 ,UNDEF};
wchar_t ENCODE_STRING[][11]={_T("ISO(none)") , _T("UTF-16") , _T("UTF-16BE") , _T("UTF-8") , _T("-")};

enum MIME{JPG,PNG,GIF,BMP,BIN}; /* JPGとPNGぐらいだろうけど予約しとく */
char EXTENSION[][4]={"jpg" , "png" , "gif" , "bmp" , "bin" };

/* フレームのタイプ */
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
	{"","","",_T("?"),FR_BIN,ALL_VER}, /* どれでもない */
	{"CRA","AENC","AENC",_T("暗号化"),FR_BIN,ALL_VER},
	{"PIC","APIC","APIC",_T("画像"),FR_APIC,ALL_VER},
	{""   ,""    ,"ASPI",_T("オーディオシーケンスポイントインデックス"),FR_BIN,ID3V24},
	{"COM","COMM","COMM",_T("コメント"),FR_TXT_EX_LNG,ALL_VER},
	{""   ,"COMR","COMR",_T("コマーシャルフレーム"),FR_BIN,ID3V23|ID3V24},
	{""   ,"ENCR","ENCR",_T("暗号化の手法の登録"),FR_BIN,ID3V23|ID3V24},
	{"EQU","EQUA","EQU2",_T("均一化"),FR_BIN,ALL_VER},
	{"ETC","ETCO","ETCO",_T("イベントタイムコード"),FR_BIN,ALL_VER},
	{"GEO","GEOB","GEOB",_T("パッケージ化された一般的なオブジェクト"),FR_BIN,ALL_VER},
	{""   ,"GRID","GRID",_T("グループ識別子の登録"),FR_BIN,ID3V23|ID3V24},
		{"IPL","IPLS","TIPL",_T("協力者"),FR_TXT,ALL_VER}, //
		{"LINK","LINK","LINK",_T("リンク情報"),FR_TXT_EX,ALL_VER}, //
	{"MCI","MCDI","MCDI",_T("音楽CD識別子"),FR_BIN,ALL_VER},
	{"MLL","MLLT","MLLT",_T("MPEGロケーションルックアップテーブル"),FR_BIN,ALL_VER},

	{""   ,"PRIV","PRIV",_T("プライベートフレーム"),FR_BIN_EX,ID3V23|ID3V24},
	{""   ,"GRID","GRID",_T("グループ識別子の登録"),FR_BIN,ID3V23|ID3V24},
	{"TAL","TALB","TALB",_T("アルバム"),FR_TXT,ALL_VER},
	{"TBP","TBPM","TBPM",_T("BPM"),FR_TXT,ALL_VER},
	{"TCM","TCOM","TCOM",_T("作曲者(Composer)"),FR_TXT,ALL_VER},
		{"TXT","TEXT","TEXT",_T("作詞家/文書作成者(TEXT)"),FR_TXT,ALL_VER},//
	{"TCO","TCON","TCON",_T("ジャンル"),FR_TXT,ALL_VER},
	{"TCR","TCOP","TCOP",_T("著作権情報"),FR_TXT,ALL_VER},
	{"TDA","TDAT","TDRC",_T("日付(録音)"),FR_TXT,ALL_VER},
	{""   ,""    ,"TDEN",_T("日付(エンコード)"),FR_TXT,ID3V24},
	{"TOR","TORY","TDOR",_T("日付(オリジナルリリース)"),FR_TXT,ALL_VER}, //TORYとTDORですが統合してしまいます
	{""   ,""    ,"TDRL",_T("日付(リリース)"),FR_TXT,ID3V24},
	{""   ,""    ,"TDTG",_T("日付(タグ付け)"),FR_TXT,ID3V24},
	{"TEN","TENC","TENC",_T("エンコードした人(Encoder)"),FR_TXT,ALL_VER},
	{"TFL","TFLT","TFLT",_T("ファイルタイプ"),FR_TXT,ALL_VER},
	{""   ,""    ,"TIPL",_T("関わった人々の一覧"),FR_TXT,ID3V24},
	{"TIM","TIME",""    ,_T("時間(V2.4では日付(録音)の方へ)"),FR_TXT,ID3V22|ID3V23},
	{"TT1","TIT1","TIT1",_T("内容の属するグループ"),FR_TXT,ALL_VER},
	{"TT2","TIT2","TIT2",_T("タイトル"),FR_TXT,ALL_VER},
	{"TT3","TIT3","TIT3",_T("サブタイトル"),FR_TXT,ALL_VER},
	{"TKE","TKEY","TKEY",_T("始めの調"),FR_TXT,ALL_VER},
	{"TLA","TLAN","TLAN",_T("言語"),FR_TXT,ALL_VER},
	{"TLE","TLEN","TLEN",_T("長さ"),FR_TXT,ALL_VER},
	{""   ,""    ,"TMCL",_T("ミュージシャンクレジットリスト"),FR_TXT,ID3V24},
	{"TMT","TMED","TMED",_T("メディアタイプ"),FR_TXT,ALL_VER},
	{""   ,""    ,"TMOO",_T("ムード"),FR_TXT,ID3V24},
	{"TOT","TOAL","TOAL",_T("オリジナルのアルバム/映画/ショーのタイトル"),FR_TXT,ALL_VER},
	{"TOF","TOFN","TOFN",_T("オリジナルファイル名"),FR_TXT,ALL_VER},
	{"TOL","TOLY","TOLY",_T("オリジナルの作詞家/文書作成者"),FR_TXT,ALL_VER},
	{""   ,"TOWN","TOWN",_T("ファイルの所有者/ライセンシー"),FR_TXT,ID3V23|ID3V24},
	{"TP1","TPE1","TPE1",_T("アーティスト"),FR_TXT,ALL_VER},
	{"TP2","TPE2","TPE2",_T("バンド/オーケストラ/伴奏"),FR_TXT,ALL_VER},
	{"TP3","TPE3","TPE3",_T("指揮者/演奏者詳細情報"),FR_TXT,ALL_VER},
	{"TP4","TPE4","TPE4",_T("翻訳者/リミックス/その他の修正"),FR_TXT,ALL_VER},
	{"TPA","TPOS","TPOS",_T("セット中の位置"),FR_TXT,ALL_VER},
	{""   ,""    ,"TPRO",_T("Produced notice"),FR_TXT,ALL_VER},
	{"TPB","TPUB","TPUB",_T("出版者"),FR_TXT,ALL_VER},
	{"TRK","TRCK","TRCK",_T("トラックの番号/セット中の位置"),FR_TXT,ALL_VER},
	{"TRD","TRDA",""    ,_T("録音日時"),FR_TXT,ID3V22|ID3V23}, //とりあえず
	{""   ,"TRSN","TRSN",_T("インターネットラジオ局の名前"),FR_TXT,ID3V23|ID3V24},
	{""   ,"TRSO","TRSO",_T("インターネットラジオ局の所有者"),FR_TXT,ID3V23|ID3V24},
	{""   ,""    ,"TSOA",_T("アルバムのソートオーダー"),FR_TXT,ID3V24},
	{""   ,""    ,"TSOP",_T("演奏者のソートオーダー"),FR_TXT,ID3V24},
	{""   ,""    ,"TSOT",_T("タイトルのソートオーダー"),FR_TXT,ID3V24},
	{"TSI","TSIZ",""    ,_T("サイズ"),FR_TXT,ID3V22|ID3V23},
	{"TRC","TSRC","TSRC",_T("ISRC"),FR_TXT,ALL_VER},
	{"TSS","TSSE","TSSE",_T("エンコードに使用したソフトウェア/ハードウェアとセッティング"),FR_TXT,ALL_VER},
	{""   ,""    ,"TSST",_T("セットのサブタイトル"),FR_TXT,ID3V24},
	{"TYE","TYER",""    ,_T("年"),FR_TXT,ID3V22|ID3V23},
	{""   ,"TCMP","TCMP",_T("コンピレーション(iTunes)"),FR_TXT,ID3V23|ID3V24},
	{"TXX","TXXX","TXXX",_T("ユーザー定義文字情報フレーム"),FR_TXT_EX,ALL_VER},
	{"UFI","UFID","UFID",_T("一意的なファイル識別子"),FR_BIN_EX,ALL_VER},
	{""   ,"USER","USER",_T("使用条件"),FR_TXT,ID3V23|ID3V24},
	{"ULT","USLT","USLT",_T("非同期歌詞/文書のコピー"),FR_TXT_EX_LNG,ALL_VER},
	{"WCM","WCOM","WCOM",_T("商業上の情報"),FR_TXT_PURE,ALL_VER},
	{"WCP","WCOP","WCOP",_T("著作権/法的情報"),FR_TXT_PURE,ALL_VER},
	{"WAF","WOAF","WOAF",_T("オーディオファイルの公式Webページ"),FR_TXT_PURE,ALL_VER},
	{"WAR","WOAR","WOAR",_T("アーティスト/演奏者の公式Webページ"),FR_TXT_PURE,ALL_VER},
	{"WAS","WOAS","WOAS",_T("音源の公式Webページ"),FR_TXT_PURE,ALL_VER},
	{""   ,"WORS","WORS",_T("インターネットラジオ局の公式Webページ"),FR_TXT_PURE,ID3V23|ID3V24},
	{""   ,"WPAY","WPAY",_T("支払い"),FR_TXT_PURE,ID3V23|ID3V24},
	{"WPB","WPUB","WPUB",_T("出版者の公式Webページ"),FR_TXT_PURE,ALL_VER},
	{"WXX","WXXX","WXXX",_T("出版者の公式Webページ"),FR_TXT_EX,ALL_VER},
	{"CRM",""    ,""    ,_T("暗号化メタフレーム"),FR_BIN,ID3V22},

	{"TDY","TDLY","TDLY",_T("プレイリスト遅延時間"),FR_TXT,ALL_VER},
	
	
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
	unsigned char *value; /* データ部実体。mallocで確保したアドレス */

	wchar_t *body; /* value上の本文開始位置。以下ポインタのものは同様 */

	wchar_t *extid; /* for TXXX,COMM,etc... */
	wchar_t *extvalue;

	unsigned char mime; /* for APIC */
	unsigned char picture_type;
	unsigned char *image_description; /* まともに対応してるソフトねーで */
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
