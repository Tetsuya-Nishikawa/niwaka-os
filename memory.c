#include "niwaka-os.h"

//メモリーの管理を行う
typedef struct _MemoryManager{
    unsigned long int size;//
    unsigned char MemoryTable[655360];//1:使用中 0:未使用
    unsigned int memoryblock_size;
    unsigned int memoryblock_num;
}MemoryManager;

typedef struct _MemoryBlock{
    unsigned int start_addr;//確保したメモリのスタート番地
    unsigned int block_size;//確保したメモリのサイズ
    unsigned int block_used;//このブロックは使用中かどうか
}MemoryBlock;

/***新メモリマネージャー
typedef struct _MemoryManager{
    unsigned long int size;
    unsigned MemoryBlock MemoryTable[655360];
    unsigned int memoryblock_size;
    unsigned int memoryblock_num;
}MemoryManager;
***/

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
unsigned int alloc_memory(unsigned int size){
    unsigned int i, j, k;
    unsigned int L;
    unsigned int start;
    L = size/(mem.memoryblock_size);
    if ((L%size)>0){
        L = L + 1;
    }

    for(i=0; i < (mem.memoryblock_num)-L+1; i++){
        for(j=0; j < L; j++){
            if(mem.MemoryTable[i+j]!=0){
                break;
            }
        }
        if(j==L){
            start = i;
            for(k=0; k < L; k++){
                mem.MemoryTable[i] = 1;
                i++;
            }
            //return 0xa0000;
            return start*(mem.memoryblock_size);
            //return 0x00100000;

            /***
            メモリの登録
            for(i=0; i<mem.memoryblock_size; i++){
                if(mem.MemoryTable[i].MemoryBlock.memory_used!=1){
                    mem.MemoryTable[i].MemoryBlock.memory_used=1;
                    mem.MemoryTable[i].MemoryBlock.start_addr = start*(mem.memoryblock_size);
                }
            }
            ***/
        }
    }
    return 0xa0000;//なかった
    //return 0x00100000;
}

//void alloc_stack();//スタック領域のメモリを確保する。

//void free_memory(unsigned int addr);//メモリの解放
//a0010080