typedef struct _PIXEL{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reserved;
}PIXEL;

typedef struct _GRA_MANAGER{
    PIXEL *vram_baseaddr;
    int max_row;
    int max_col;
}GRAPHIC_MANAGER;

typedef struct _IDTR{
    unsigned int limit;
    unsigned int   base;
}IDTR;

typedef struct _GDTR{
    unsigned int limit;
    unsigned int   base;
}GDTR;

//8byte
typedef struct idt_gate{
    short offset_low;
    short selector;
    short attr;
    short offset_high;
}IDT_GATE;

//8byte
typedef struct _GDT_GATE{
    unsigned short limit_low;
    unsigned short offset_low;
    unsigned char offset_middle;
    unsigned char attr_low;
    unsigned char limit_high_and_attr_high;
    unsigned char offset_high;
}GDT_GATE;

void hlt();

void font_init();
extern unsigned char font_ASCII[128][16];
//extern unsigned const char font_A[16];

void sidt(unsigned int addr);
char in_8(int port);
void out_8(int port, char value);
int in_32(int port);
void out_32(int port, int value);
void task_switch(int eip, int cs);

void load_tr(unsigned int selector);

void create_gdt(unsigned int vector_number,unsigned int limit,unsigned int addr,unsigned int attr);
void create_idt(char vector_number, int offset, short attr, short selector);
//create_idt(0x21, (int)asm_inthandler21, 0x8e00, 2*8);
void asm_inthandler21();
//void asm_inthandler26();
void asm_inthandler48();

void inthandler21(int *esp);
//void inthandler26(int *esp);
void inthandler48(int *esp);
void cpuid();
void rdmsr();
void sti();
void cli();
void set_apictimer(unsigned int sec);

extern unsigned int timer_flg;

void init_pic();
void init_kbc();
int check_fifo();
void console_init();
//void init_timer_manager();
void add_timer_block(unsigned int now_count, unsigned int time, unsigned short selector);
void set_apictimer(unsigned int sec);

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1
#define DMA_BUFF        0x3000

//TSS
typedef struct _TSS{
    unsigned int backlink;
    unsigned int esp0;
    unsigned int ss0;
    unsigned int esp1;
    unsigned int ss1;
    unsigned int esp2;
    unsigned int ss2;
    unsigned int cr3;
    unsigned int eip;
    unsigned int eflags;
    unsigned int eax;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    unsigned int es;
    unsigned int cs;
    unsigned int ss;
    unsigned int ds;
    unsigned int fs;
    unsigned int gs;
    unsigned int ldt;
    unsigned int iomap;
}TSS;

#define AR_TSS 0x0089

//?????????????????
typedef struct _TIMER_BLOCK{
    unsigned int now_count;
    unsigned int time;//??????????
    unsigned short selector;//????TSS??????
}TIMER_BLOCK;

//timer?????
typedef struct _TIMER_MANAGER{
    int block_num;//TIMER_BLOCK?????
    int block_max;//TIMER_BLOCK????
    TIMER_BLOCK timer_block_list[2];
}TIMER_MANAGER;
unsigned int measure_hz(unsigned short addr);

void init_memory_table();
unsigned int alloc_memory(unsigned int size);

void console_main();

extern unsigned int timer_counter;