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

	//周波数を測る
	int apic_hz;//apic_timerの周波数
	apic_hz = measure_hz(*acpi_timer_addr);
	*(hz+1) = apic_hz;

	//初期化を行う。
	init();

	//drow_rect(640, 0, 800, 20);
	int data;

	TSS task_a, task_b, task_c;

	alloc_tss(console_init);
	alloc_tss(print_time);
	set_apictimer(1);
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
	init_proc_manager();
	//console_init();

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

