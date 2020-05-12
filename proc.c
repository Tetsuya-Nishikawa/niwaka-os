#include"niwaka-os.h"
#define START_SEL 3
//プロセス構造体
typedef struct _PROC{
    unsigned int proc_id;
    unsigned int proc_flg;//1:使っていない 0:使っている
    unsigned int time;//持ち時間
    TSS tss;
}PROC;

typedef struct _PROC_MANAGER{
    //プロセス構造体の配列
    PROC ProcTable[100];
    unsigned int proc_num;//現在管理しているプロセスの数
    unsigned int proc_max_num;
    unsigned int proc_now;//現在動作しているプロセス
}PROC_MANAGER;

static PROC_MANAGER proc_manager;

static void init_tss(TSS *tss){
	tss->eip   = 0;
	tss->eflags = 0x00000202;
	tss->eax   = 0;
	tss->ecx   = 0;
	tss->edx   = 0;
	tss->ebx   = 0;
	tss->esp   = 0;
	tss->ebp   = 0;
	tss->esi   = 0;
	tss->edi   = 0;
	tss->es    = 8*1;
	tss->cs    = 8*2;
	tss->ss    = 8*1;
	tss->ds    = 8*1;
	tss->fs    = 8*1;
	tss->gs    = 8*1;
	tss->ldt   = 0;
	tss->iomap = 0x40000000;

}

//TSSを割り当てる
void alloc_tss(unsigned int eip){
    unsigned int i;

    for(i=1; i<proc_manager.proc_max_num; i++){
        if(proc_manager.ProcTable[i].proc_flg==1){
            proc_manager.ProcTable[i].proc_flg = 0;
            proc_manager.ProcTable[i].tss.eip = eip;
            proc_manager.ProcTable[i].tss.esp = alloc_memory(8*1024)+8*1024-8;
    
	        create_gdt(START_SEL+i, 103, (int)&(proc_manager.ProcTable[i].tss), AR_TSS);
            return;
        }
    }
    return;
}

void init_proc_manager(){
    unsigned int i;
    proc_manager.proc_num = 0;
    proc_manager.proc_max_num = 100;
    proc_manager.proc_now=0;

    for(i=0; i<proc_manager.proc_max_num;i++){
        proc_manager.ProcTable[i].proc_id = i+1;//プロセスID
        init_tss(&(proc_manager.ProcTable[i].tss));
        proc_manager.ProcTable[i].proc_flg = 1;
    }
    proc_manager.ProcTable[0].proc_flg = 0;
	create_gdt(3, 103, (int)&(proc_manager.ProcTable[0].tss), AR_TSS);
    load_tr(3*8);
    return;
}

//プロセスを変更する。hennkousuru.
//プロセススケジューラ
void sched_proc(){
    //proc_save(proc_now, proc_next);//proc_nowは、今実行しているプロセス、proc_nextは次のプロセス
}