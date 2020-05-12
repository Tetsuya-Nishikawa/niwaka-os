//プロセス関連

//レジスタ群
typedef struct _REGISTERS{
    int EAX;//Aレジスタ
    int EBX;//Bレジスタ
}REGISTER;

//プロセス構造体
typedef struct _PROC{
    //スタックポインタ
    //プロセスID
    int sleep_mode;//スリープモード
}PROC;

typedef struct _PROC_MANAGER{
    //プロセス構造体の配列
    PROC ProcTable[100];
    unsigned proc_num;
    unsigned proc_max_num;
}PROC_MANAGER;

//プロセスを変更する。hennkousuru.
//プロセススケジューラ
void sched_proc(){
    //proc_save(proc_now, proc_next);//proc_nowは、今実行しているプロセス、proc_nextは次のプロセス
}