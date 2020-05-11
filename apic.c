#include "niwaka-os.h"

unsigned int timer_flg=0;

volatile static int *lvt_timer             = 0xfee00320;
volatile static unsigned int *initial_count = 0xfee00380;
volatile static unsigned int *current_count = 0xfee00390;
volatile static unsigned int count_max      = 0xffffffff;
volatile static int *divide_configuration  = 0xfee003e0;
volatile static int *eoi                   = 0xfee000b0;
volatile static int hz=0;
static TIMER_MANAGER timer_manager;

typedef struct _APIC_TIMER{
    char *lvt_timer;
    unsigned int *initial_count;
    unsigned int *current_count;
    unsigned int count_max;
    char *divide_configuration;
    unsigned int hz;
}APIC_TIMER;

void inthandler48(int *esp){
    *eoi = 1;//何かしら書き込む
    int *test=0x100;
    *test = 0xaaffaaff;
    //マルチタスク
    if(timer_flg==1){
        timer_flg=0;
        task_switch(0, 3*8);
    }
    else{
        *test = 0xaaaaaaaa;
        timer_flg=1;
        task_switch(0, 4*8);
    }
    return;
}

/***
void inthandler48(int *esp){
    *eoi = 1;//何かしら書き込む
    int i;

    for(i=0;i < timer_manager.block_num; i++){
        timer_manager.timer_block_list[i].now_count--;
    }

    return;
}
***/

//周波数の測定を行う。
unsigned int measure_hz(unsigned short addr){
    unsigned int end_count;
    *divide_configuration = 0x23;//分周比 1対1
    *lvt_timer = 1<<16 | 48;
    *initial_count = count_max;
	wait(1, addr);//1秒待機する。
    end_count = *current_count;
    hz = (count_max-end_count);
	return hz;
}

//apicタイマーの割り込み設定をする。
void set_apictimer(unsigned int sec){
    timer_manager.block_num = 0;
    timer_manager.block_max = 2;
    *lvt_timer = 0x2 << 16 | 48;
    *initial_count = sec*hz;//1秒間にhz回カウントするので、secとかければいい
    return;
}


//timer_blockをtimer_managerに追加する
void add_timer_block(unsigned int now_count, unsigned int time, unsigned short selector){
    TIMER_BLOCK timer_block;//とりあえずこれで
    timer_block.now_count = now_count;
    timer_block.selector = selector;
    timer_block.time = time;
    if(timer_manager.block_max>timer_manager.block_num){
        timer_manager.timer_block_list[timer_manager.block_num] = timer_block;
        timer_manager.block_num++;
    }
    return;
}
