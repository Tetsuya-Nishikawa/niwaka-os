#include "niwaka-os.h"
//static GRAPHIC_MANAGER* graphic_manager=0x100c;
void wait(int sec, unsigned short addr);
static void init();
static void blue();
static void black();

void niwaka_main(){

	volatile int *lvt_timer = 0x01000000;
	*lvt_timer = 0xffffffff;
    int i,j;
	for(i=0; i < 10000; i++){
		*(lvt_timer+i) = 0xffffffff;
	}
	int *a = 0x0;
	*a = &lvt_timer;
	
    unsigned int *vram_x        = 0x1000;
    unsigned int *vram_y        = 0x1008;
    unsigned int *vram_baseaddr = 0x1010;
	unsigned int *acpi_timer_addr = 0x2000;
	unsigned int *hz            = 0x2008;
    unsigned int max_row;
    unsigned int max_col;
    unsigned int base_addr;
    max_row = *vram_y;
    max_col = *vram_x;
    base_addr = *vram_baseaddr;
    PIXEL *vram=base_addr;
    PIXEL *vram_now;
	//graphic_manager->vram_baseaddr=*vram_baseaddr;
	//graphic_manager->max_col = *vram_x;
	//graphic_manager->max_row = *vram_y;

	//周波数を測る
	int apic_hz;//apic_timerの周波数
	apic_hz = measure_hz(*acpi_timer_addr);
	*(hz+1) = apic_hz;

	//初期化を行う。
	init();

	//drow_rect(640, 0, 800, 20);
	int data;

	TSS task_a, task_b;
	//コンソール用のTSS
	TSS task_console;
	task_console.ldt = 0;
	task_console.iomap = 0x40000000;
	create_gdt(5, 103, (int)&task_console, AR_TSS);
	task_console.eip   = console_init;
	task_console.eflags = 0x00000202;
	task_console.eax   = 0;
	task_console.ecx   = 0;
	task_console.edx   = 0;
	task_console.ebx   = 0;
	//task_console.esp   = 0xa00000;
	task_console.esp   = alloc_memory(0x10000);
	task_console.ebp   = 0;
	task_console.esi   = 0;
	task_console.edi   = 0;
	task_console.es    = 8*1;
	task_console.cs    = 8*2;
	task_console.ss    = 8*1;
	task_console.ds    = 8*1;
	task_console.fs    = 8*1;
	task_console.gs    = 8*1;
	task_console.ldt   = 0;
	task_console.iomap = 0x40000000;

	task_a.ldt = 0;
	task_a.iomap = 0x40000000;
	
	create_gdt(3, 103, (int)&task_a, AR_TSS);
	create_gdt(4, 103, (int)&task_b, AR_TSS);

	load_tr(3*8);

	task_b.eip   = black;
	task_b.eflags = 0x00000202;
	task_b.eax   = 0;
	task_b.ecx   = 0;
	task_b.edx   = 0;
	task_b.ebx   = 0;
	//task_b.esp   = 0x10000;
	task_b.esp   = alloc_memory(0x1000);
	task_b.ebp   = 0;
	task_b.esi   = 0;
	task_b.edi   = 0;
	task_b.es    = 8*1;
	task_b.cs    = 8*2;
	task_b.ss    = 8*1;
	task_b.ds    = 8*1;
	task_b.fs    = 8*1;
	task_b.gs    = 8*1;
	task_b.ldt   = 0;
	task_b.iomap = 0x40000000;

	//(int vector_number, int limit, int addr, int attr)
	set_apictimer(4);

	for(;;){
	
		for(i=0; i < max_col; i++){
			for(j=0; j < max_row; j++){
				vram_now = vram+(max_col*j)+i;
				vram_now->blue=0x0;
				vram_now->red=0x0;
				vram_now->green=0xff;
			}
		}
	}
	for(i=0; i < max_col; i++){
		for(j=0; j < max_row; j++){
			vram_now = vram+(max_col*j)+i;
			vram_now->blue=0x0;
			vram_now->red=0x0;
			vram_now->green=0x0;
		}
	}
    for(;;){
        hlt();
    }
}

static void init(){
	cli();
	init_kbc();
	font_init();

	//0x21は、33
	create_idt(0x21, (int)asm_inthandler21, 0x8e00, 2*8);
	//create_idt(0x26, (int)asm_inthandler26, 0x8e00, 2*8);
	//apicタイマー用のセグメント
	create_idt(48,   (int)asm_inthandler48, 0x8e00, 2*8);

	//init_timer_manager();
	init_pic();
	out_8(PIC0_IMR, 0xfd);
	sti();
	init_memory_table();
	console_init();
}

void wait(int sec, unsigned short addr){
	unsigned int start_time;
	unsigned int end_time;

	start_time = in_32(addr);
	end_time   = start_time + 3579545*sec;
	end_time   = end_time & 0x00ffffff;

	if(end_time < start_time){
		while(in_32(addr) >= start_time){
		
		}
	}
	while(in_32(addr) < end_time){
	
	}
}

//経過時間を表示する。
void print_time(){
	static int time;
	set_apictimer(1);
	for(;;){
		//print_font(char *font, int x, int y);
	}

	return;
}

void init_mem_page(){
	int i;

	for(;;){

	}
}

//ブルースクリーン
static void blue(){
	int temp=timer_flg;
    int *vram_x        = 0x1000;
    int *vram_y        = 0x1008;
    int *vram_baseaddr = 0x1010;
	unsigned int *acpi_timer_addr = 0x2000;
	unsigned int *hz            = 0x2008;
    int max_row;
    int max_col;
    int base_addr;
    max_row = *vram_y;
    max_col = *vram_x;
    base_addr = *vram_baseaddr;
    PIXEL *vram=base_addr;
    PIXEL *vram_now;
	//graphic_manager->vram_baseaddr=*vram_baseaddr;
	//graphic_manager->max_col = *vram_x;
	//graphic_manager->max_row = *vram_y;
	int i, j;

	while(1){
		for(i=0; i < max_col; i++){
			for(j=0; j < max_row; j++){
				vram_now = vram+(max_col*j)+i;
				vram_now->blue=0x0;
				vram_now->red=0x0;
				vram_now->green=0x0;
			}
		}

	}
	return;
}

//ブラックスクリーン
static void black(){
	unsigned int temp=timer_flg;
	unsigned int *vram_x        = 0x1000;
	unsigned int *vram_y        = 0x1008;
	unsigned int *vram_baseaddr = 0x1010;
	unsigned int *acpi_timer_addr = 0x2000;
	unsigned int *hz            = 0x2008;
	unsigned int max_row;
	unsigned int max_col;
	unsigned int base_addr;
    max_row = *vram_y;
    max_col = *vram_x;
    base_addr = *vram_baseaddr;
    PIXEL *vram=base_addr;
    PIXEL *vram_now;
	//graphic_manager->vram_baseaddr=*vram_baseaddr;
	//graphic_manager->max_col = *vram_x;
	//graphic_manager->max_row = *vram_y;
	int i,j;

	while(1){

		for(i=0; i < max_col; i++){
			for(j=0; j < max_row; j++){
				vram_now = vram+(max_col*j)+i;
				vram_now->blue=0xff;
				vram_now->red=0x0;
				vram_now->green=0x0;
			}
		}
	}
	return;
}
int pow(int a, int b){
    int i;
    int temp = a;
    if(b==0){
        return 1;
    }
    for(i=0; i < b; i++){
        a = a*temp;
    }
    return a;
}
void char_to_int(char s[5]){
    int i;
    int j;
    int ans=0;
    i = 0;
    for(j=4; j > -1; j--){
        if(s[j]=='0'){
            ans = ans + 0*pow(10, i);
        }
        if(s[j]=='1'){
            ans = ans + 1*pow(10, i);
        }
        if(s[j]=='2'){
            ans = ans + 2*pow(10, i);
        }
        if(s[j]=='3'){
            ans = ans + 3*pow(10, i);
        }
        if(s[j]=='4'){
            ans = ans + 4*pow(10, i);
        }
        if(s[j]=='5'){
            ans = ans + 5*pow(10, i);
        }
        if(s[j]=='6'){
            ans = ans + 6*pow(10, i);
        }
        if(s[j]=='7'){
            ans = ans + 7*pow(10, i);
        }
        if(s[j]=='8'){
            ans = ans + 8*pow(10, i);
        }
        if(s[j]=='9'){
            ans = ans + 9*pow(10, i);
        }
        i++;
    }
    return;
}
