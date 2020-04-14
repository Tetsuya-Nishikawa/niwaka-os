//メモリーの管理を行う

//メモリーは、4294967296
//4KBごとに管理行う
static char memory_table[1024*1024];


//メモリーテーブルの初期化を行う
void init_memory_table(){
    int i;
    for(i=0; i<1024*1024; i++){
        memory_table[i] = 0;//0:未使用
    }
}

//メモリーの割り当てを行う
void alloc_memory(int size){
    int i, j, k;
    int L;
    L = size/(1024*4);
    if ((L%size)>0){
        L = L + 1;
    }
    for(i=0; i < (1024*1024)-L; i++){
        for(j=0; j < size; j++){
            if(memory_table[i+j]!=0){
                break;
            }
        }
        if(j==size){
            return (i+j)*(4*1024);
        }
    }
    return -1;//なかった
}

//メモリーの解放を行う
void free_memory(int addr){
    int index;
    index = addr/(4*1024);
    memory_table[index]=0;//メモリーの解放
    return;
}

