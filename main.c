#include "elf.h"
#include "acpi.h"
//void jump_to_os(int entry_point);


int strncmp(char *s1, char *s2, int len);
void memory_transfer(char *from, char *to, int size);

typedef struct _EFI_TIME{
	UINT16 Year;
	UINT8  Month;
	UINT8  Day;
	UINT8  Hour;
	UINT8  Minute;
	UINT8  Second;
	UINT8  Pad1;
	UINT32 Nanosecond;
	INT16  TimeZone;
	UINT8  Daylight;
	UINT8 Pad2;
}EFI_TIME;

typedef struct _EFI_FILE_INFO{
	UINT64 Size;
	UINT64 FileSize;
	UINT64 PhysicalSize;
	EFI_TIME CreateTime;
	EFI_TIME LastAccessTime;
	EFI_TIME ModificationTime;
	UINT64 Attribute;
	CHAR16 FileName[];
}EFI_FILE_INFO;

//FILE関連
#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID {0x964e5b22,0x6459,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

//P176参照
//128bit
typedef struct _EFI_GUID{
	UINT32 Data1;//4byte
	UINT16 Data2;//2byte
	UINT16 Data3;//2byte
	UINT8 Data4[8];//8byte
}EFI_GUID;

EFI_GUID SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

typedef struct _EFI_TABLE_HEADER{
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
}EFI_TABLE_HEADER;

typedef EFI_STATUS (*EFI_LOCATE_PROTOCOL)(EFI_GUID *Protocol, VOID *Registration, VOID **Interface);

//P.94(UEFI仕様書)
typedef struct _EFI_BOOT_SEVICES{
	EFI_TABLE_HEADER Hdr;
	int buf[37];
	EFI_LOCATE_PROTOCOL LocateProtocol;
}EFI_BOOT_SERVICES;

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL{
		unsigned int _buf;
		unsigned int (*OutputString)(
		struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
		unsigned short *String);
		unsigned int _buf2[4];
		unsigned int (*ClearScreen)(
		struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
};

typedef struct _EFI_CONFIGURATION_TABLE{
	EFI_GUID VendorGuid;
	VOID *VendorTable;
}EFI_CONFIGURATION_TABLE;

typedef struct _EFI_SYSTEM_TABLE {
	char buf1[44];
	struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
	char bur2[12];
	EFI_BOOT_SERVICES *BootServices;
	UINTN NumberOfTableEntries;
 	EFI_CONFIGURATION_TABLE *ConfigurationTable;
}EFI_SYSTEM_TABLE;

//P507
typedef struct EFI_FILE_PROTOCOL{
	UINT64 Revision;
	EFI_STATUS (*OPEN)(struct _EFI_FILE_PROTOCOL *This, struct _EFI_FILE_PROTOCOL **NewHandle, CHAR16 *FileName, INT64 OpenMode, INT64 Attributes);
	INTN bur1[2];
	EFI_STATUS (*READ)(struct _EFI_FILE_PROTOCOL *This, UINTN *BufferSize, VOID *Buffer);
	INTN buf2[10];
}EFI_FILE_PROTOCOL;
 EFI_FILE_PROTOCOL *root;

//Open Mode
#define EFI_FILE_MODE_READ   0x0000000000000001
#define EFI_FILE_MODE_WRITE  0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000
//File Attribute
#define EFI_FILE_READ_ONLY   0x0000000000000001
#define EFI_FILE_HIDDEN      0x0000000000000002
#define EFI_FILE_SYSTEM      0x0000000000000004
#define EFI_FILE_RESERVED    0x0000000000000008
#define EFI_FILE_DIRECTORY   0x0000000000000010
#define EFI_FILE_ARCHIVE     0x0000000000000020
#define EFI_FILE_VALID_ATTR  0x0000000000000037

typedef EFI_STATUS (*EFI_FILE_OPEN)(EFI_FILE_PROTOCOL *This, EFI_FILE_PROTOCOL **NewHandle, CHAR16 *FileName, UINT64 OpenMode, UINT64 Attributes);
typedef EFI_STATUS (*EFI_FILE_READ)(EFI_FILE_PROTOCOL *This, UINTN *BufferSize, VOID *Buffer);


typedef EFI_STATUS (*EFI_LOCATE_PROTOCOL)(EFI_GUID *Protocol, VOID *Registration, VOID **Interface);

typedef EFI_STATUS (*EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME)(struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This, EFI_FILE_PROTOCOL **Root);

typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL{
	UINT64 Revision;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME OpenVolume;//関数ポインタ
}EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *ESFSP;

typedef struct  _PIXEL{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reserved;
}PIXEL;

typedef enum _EFI_GRAPHICS_PIXEL_FORMAT{
	PixelRedGreenBlueReserved8BitPerColor,
	PixelBlueGreenRedReserved8BitPerColor,
	PixelBitMask,
	PixelBltOnly,
	PixelFormatMax
}EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct _EFI_PIXEL_BITMASK{
	UINT32 BlueMask;
	UINT32 GreenMask;
	UINT32 RedMask;
	UINT32 ReservedMask;
}EFI_PIXEL_BITMASK;

typedef struct _EFI_GRAPHICS_OUTPUT_MODE_INFORMATION{
	UINT32 Version;
	UINT32 HorizontalResolution;
	UINT32 VerticalResolution;
	EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
	EFI_PIXEL_BITMASK PixelInformation;
	UINT32 PixelsPerScanLine;
}EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct  _EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE{
	UINT32 MaxMode;
	UINT32 Mode;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
	UINTN SizeOfInfo;
	EFI_PHYSICAL_ADDRESS FrameBufferBase;
	UINTN FrameBufferSize;
}EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct _EFI_GRAPHICS_OUTPUT_PROTCOL{
	int buff1[3];
	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *MODE;
}EFI_GRAPHICS_OUTPUT_PROTCOL;
EFI_GRAPHICS_OUTPUT_PROTCOL *EGOP;

//EFI_FILE_OPEN
typedef EFI_STATUS (*EFI_FILE_OPEN)(EFI_FILE_PROTOCOL *This,EFI_FILE_PROTOCOL **NewHandle,CHAR16 *FileName,UINT64 OpenMode,UINT64 Attributes);
typedef EFI_STATUS (*EFI_FILE_CLOSE)(EFI_FILE_PROTOCOL *This);
typedef EFI_STATUS (*EFI_FILE_READ)(EFI_FILE_PROTOCOL *This,UINTN *BufferSize,VOID *Buffer);

typedef struct _GRAPHIC_MANAGER{
	int x;
	int y;
	int fram_baseaddr;
}GRAPHIC_MANAGER;

typedef struct _StandardHeader{
	char signature[4];
	unsigned int length;
	unsigned char revision;
	unsigned char checksum;
	char oem_id[6];
	char oem_table_id[8];
	unsigned int oem_revision;
	unsigned int creator_id;
	unsigned int creator_revision;
}StandardHeader;

typedef struct _XSDT{
	StandardHeader standardheader;
}XSDT;

typedef struct _FADT{
	char Signature[4];
	int Length;
	char buf1[68];
	unsigned int PM_TMR_BLK;
	char buf2[42];
	unsigned int flags;
}FADT;
int search_acpi(EFI_SYSTEM_TABLE *SystemTable, EFI_GUID eatg_guid);
unsigned int in32(unsigned short addr);

void efi_main(int ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{

	SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.Data1    = 0x964e5b22;
	SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.Data2    = 0x6459;
	SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.Data3    = 0x11d2;
	SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.Data4[0] = 0x8e;
	SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.Data4[1] = 0x39;
	SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.Data4[2] = 0x00;
	SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.Data4[3] = 0xa0;
	SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.Data4[4] = 0xc9;
	SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.Data4[5] = 0x69;
	SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.Data4[6] = 0x72;
	SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.Data4[7] = 0x3b;

	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Hello UEFI!\n");
	//SystemTable->BootServices->LocateProtocol(&SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, 0, ESFSP);
	EFI_GUID sfsp_guid = {0x0964e5b22, 0x6459, 0x11d2,{0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
	EFI_GUID gfsp_guid = {0x9042a9de,0x23dc,0x4a38,{0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}};
	//P101に記載されている。ACPI関連のGUID
	EFI_GUID eatg_guid = {0x8868e871,0xe4f1,0x11d3,{0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}};

	SystemTable->BootServices->LocateProtocol(&sfsp_guid, 0, &ESFSP);
	SystemTable->BootServices->LocateProtocol(&gfsp_guid, 0, &EGOP);

	//OpenVolumeを実行する。
	
	//fileバッファ
	EFI_FILE_INFO *file_entry;

	unsigned short filename[12];
	int i;
	for(i=0; i < 12; i++){
		filename[i] =L' ';//スペースキーを入れる。
	}
	filename[13] =L'\0';//終了コード

	//filename_listの初期化を行う。
	unsigned long long buf_size=0;
	ESFSP->OpenVolume(ESFSP, &root);
	while(1){
		root->READ(root, &buf_size, file_entry);
		if(buf_size==0){
			break;
		}
		for(i=0; i < 12; i++){
			filename[i] = file_entry->FileName[i];
		}
		print_unicode(SystemTable, filename);
		print_unicode(SystemTable, L"  ");
	}
	//niwakaos.binを読み込む
	EFI_FILE_PROTOCOL *file;
	char *osfile_buff;
	unsigned int osfile_size=100000;
	root->OPEN(root, &file, L"niwaka-os.bin", EFI_FILE_MODE_READ, 0);
	file->READ(file, &osfile_size, osfile_buff);
	unsigned int entry_point;//エントリポインと
	unsigned int data_linearaddr;//dataセグメントのトップリニアアドレス
	unsigned int bss_linearaddr; //bssセグメントのトップリニアアドレス
	unsigned int code_filesize;  //codeセグメントのファイル上のサイズ
	unsigned int data_filesize;  //dataセグメントのファイル上のサイズ
	unsigned int code_fileoffset;//コードセグメントのファイルの先頭からのオフセット
	unsigned int data_fileoffset;//データセグメントのファイルの先頭からのオフセット

	ELF_HEADER *elf_header=osfile_buff;
	PH_HEADER  *ph_header=(osfile_buff+elf_header->e_phoff);
	entry_point     = elf_header->e_entry;
	data_linearaddr = ((PH_HEADER*)((unsigned int)ph_header+32))->p_vaddr;

	code_filesize   = ph_header->p_filesz;
	data_filesize   = ((PH_HEADER*)((unsigned int)ph_header+32))->p_filesz;
	code_fileoffset = ((PH_HEADER*)((unsigned int)ph_header))->p_offset;
	data_fileoffset = ((PH_HEADER*)((unsigned int)ph_header+32))->p_offset;

	//まずは、コードセグメントの転送から
	//memory_transfer(osfile_buff,0x0c000000, code_filesize+code_fileoffset); 
	memory_transfer(osfile_buff,0xa0000000, code_filesize+code_fileoffset); 

	//dataセグメントの転送
	memory_transfer(osfile_buff+data_fileoffset, data_linearaddr, data_filesize);

	//ELF形式のファイルを展開する。

	//描画する。
	int j,k;
	PIXEL *vram=EGOP->MODE->FrameBufferBase;
	PIXEL *vram_now;
	unsigned int *test10=0x1000;
	unsigned int *test11=0x1008;
	unsigned int *test12=0x1010;
	unsigned int *test13=0x1018;

	*(test10)   = EGOP->MODE->Info->PixelsPerScanLine;
	*(test11) = EGOP->MODE->Info->VerticalResolution;
	*(test12) = EGOP->MODE->FrameBufferBase;
	*(test13) = EGOP->MODE->FrameBufferSize;

	for(i=0; i < EGOP->MODE->Info->HorizontalResolution; i++){
		for(j=0; j < EGOP->MODE->Info->VerticalResolution; j++){

			vram_now = vram+(EGOP->MODE->Info->HorizontalResolution*j)+i;
			vram_now->blue=0x0;
			vram_now->red=0xff;
			vram_now->green=0x0;
		}
	}

	//jmp(elf_header->e_entry);
	RSDP *rsdp;
	EFI_GUID a = {0x8868e871,0xe4f1,0x11d3,{0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}};
	rsdp  = search_acpi(SystemTable, a);
	//int *t=0x100;
	//*t = rsdp;//0xbff8a014に配置されている。
	//xsdtは、0xbff890e8に配置されている。

	XSDT* xsdt_addr=0x100;
	xsdt_addr = rsdp->xsdt_address;//xsdt_addressを書き込むkakikomu

	//テーブルの数を求める。
	int table_num;
	table_num = (xsdt_addr->standardheader.length-36)/(8);
	FADT *entry;
	unsigned short pm_timer_block;
	for(i=0; i < table_num; i++){
		int *tt=0x2000;
		entry = *(int*)((int)xsdt_addr+36+8*i);
		*(tt+i) = entry->Signature[0];
		if(strncmp("FACP", entry->Signature, 4)==1){//あった
			FADT *fadt=entry;//FADT構造体あった。
			if((fadt->flags & (1<<8)) == 1){

			}

			pm_timer_block = fadt->PM_TMR_BLK;
			break;
		}

	}
	unsigned int *memo1=0x2000;
	unsigned int *memo2=0x2008;
	*memo1 = pm_timer_block;
	*memo2 = 3579545;

	jmp(elf_header->e_entry);

	while(1){

	}
}

//addr番地にジャンプする。
//インラインアセンブラ
void jmp(unsigned int addr){
	
	//unsigned int sp = 0x0c000000;
	unsigned int sp=0xa0000000;
	asm("movl %0, %%esp"
		:
		:"r"(sp)
		:
	);
	asm("movl %0, %%eax"
		:
		:"r"(addr)
		:
	);
	asm("JMP %eax");
}

//指定された秒数だけ待機する。
void wait(int sec, unsigned short addr){
	unsigned int start_time;
	unsigned int end_time;

	start_time = in32(addr);
	end_time   = start_time + 3579545*sec;
	end_time   = end_time & 0x00ffffff;

	if(end_time < start_time){
		while(in32(addr) >= start_time){
		
		}
	}
	while(in32(addr) < end_time){
	
	}
}


unsigned int in32(unsigned short addr)
{
    unsigned int time;
    asm volatile ("inl %[addr], %[time]"
    			: [time]"=a"(time) : [addr]"d"(addr));
    return time;
}

//文字を表示する。
void print_unicode(EFI_SYSTEM_TABLE *SystemTable, unsigned short *s){
	SystemTable->ConOut->OutputString(SystemTable->ConOut, s);
	return;
}

//データを転送元から転送先へコピーする
void memory_transfer(char *from, char *to, int size){
	int i;

	for(i=0; i < size; i++){
		*(to+i) = *(from+i);
	}

	return;
}

//グラフィック関連の初期化を行う。
void init_graphic(EFI_SYSTEM_TABLE *SystemTable){
	return;
}

//acpiテーブルを探す
int search_acpi(EFI_SYSTEM_TABLE *SystemTable, EFI_GUID eatg_guid){
	unsigned int i;
	VOID* acpi;
	EFI_GUID a = {0x8868e871,0xe4f1,0x11d3,{0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}};
	EFI_GUID *t=0x100;
	EFI_GUID *t1 = 0x100000;
	*t1 = a;
	//unsigned int *t1 = 0x100;
	for(i=0; i < SystemTable->NumberOfTableEntries; i++){
		*(t+i) = SystemTable->ConfigurationTable[i].VendorGuid;
		//*t1 = SystemTable->NumberOfTableEntries;
		if(compare_guid(a, SystemTable->ConfigurationTable[i].VendorGuid)==1){
			acpi =  SystemTable->ConfigurationTable[i].VendorTable;
			return acpi;
		}
	}
	return 0;//失敗
}

//2つのGUIDを比較する。
int compare_guid(EFI_GUID guid1, EFI_GUID guid2){
	
	/***
	typedef struct _EFI_GUID{
		UINT32 Data1;
		UINT16 Data2;
		UINT16 Data3;
		UINT8 Data4[8];
	}EFI_GUID;
	***/

	EFI_GUID a = {0x8868e871,0xe4f1,0x11d3,{0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}};

	int i;
	char *t1;

	char *t2;

	t1 = &a;
	t2 = &guid2;
	for(i = 0; i < 16; i++){
		if(*(t1+i) != *(t2+i)){
			return -1;
		}
	}
	return 1;//失敗
}

//文字列の比較を行う。
int strncmp(char *s1, char *s2, int len){

	int i;

	for(i=0; i < len; i++){
		if(s1[i]!=s2[i]){
			return -1;//失敗
		}
	}
	return 1;//成功
}