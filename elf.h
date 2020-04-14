//RV32
#define BOOlEAN char
#define INTN    int
#define UINTN   unsigned int
#define INT8    char
#define UINT8   unsigned char 
#define INT16   short
#define UINT16  unsigned short
#define INT32   int
#define UINT32  unsigned int
#define INT64   long long
#define UINT64  unsigned long long
#define CHAR8   char
#define CHAR16  short
#define VOID    void

#define EFI_STATUS UINTN
#define EFI_HANDLE VOID *
#define EFI_EVENT  VOID *
#define EFI_LBA    UINT64
#define EFI_TPL    UINTN
#define EFI_PHYSICAL_ADDRESS UINT32


typedef struct _ELF_HEADER{
   char e_ident[16];
   short e_type;
   short e_machine;
   int e_version;
   int e_entry;
   int e_phoff;
   int shoff;
   int e_flags;
   short e_ehsize;
   short e_pehtsize;
   short e_phnum;
   short e_shetsize;
   short e_shnum;
   short e_shstrndx;
}ELF_HEADER;

typedef struct _PH_HEADER{
   int p_type;
   int p_offset;
   int p_vaddr;
   int p_paddr;
   int p_filesz;
   int p_memmsz;
   int p_flags;
   int p_align;
}PH_HEADER;