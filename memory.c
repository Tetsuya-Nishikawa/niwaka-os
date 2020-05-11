#include "niwaka-os.h"

//メモリーの管理を行う
typedef struct _MemoryManager{
    unsigned long int size;//
    unsigned char MemoryTable[655360];//1:使用中 0:未使用
    unsigned int memoryblock_size;
    unsigned int memoryblock_num;
}MemoryManager;
//a0006020
//メモリーは、4294967296
//4KBごとに管理行う
static MemoryManager mem;

//メモリーテーブルの初期化を行う
void init_memory_table(){
    int i;
    mem.size = 2684354560;//約2GB
    mem.memoryblock_size = 0x1000;
    mem.memoryblock_num = 655360;
    for(i=0; i<mem.memoryblock_num; i++){
        mem.MemoryTable[i] = 0;
    }
    for(i=0; i<256; i++){
        mem.MemoryTable[i] = 1;
    }
    return;
}

//メモリーの割り当てを行う
unsigned int alloc_memory(int size){
    int i, j, k;
    int L;
    int start;
    L = size/(mem.memoryblock_size);
    if ((L%size)>0){
        L = L + 1;
    }

    for(i=0; i < (mem.memoryblock_num)-L; i++){
        for(j=0; j < L; j++){
            if(mem.MemoryTable[i+j]!=0){
                break;
            }
        }
        if(j==L){
            start = i;
            //return (i+j)*(4*1024);//番地を返す
            for(k=0; k < L; k++){
                mem.MemoryTable[i] = 1;
                i++;
            }
            unsigned *test = 0x100;
            *test = start*(mem.memoryblock_size);
            return start*(mem.memoryblock_size);
            //return 0x00100000;
        }
    }
    return 1;//なかった
    //return 0x00100000;
}

//a0010080