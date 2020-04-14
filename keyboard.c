#include "niwaka-os.h"
//keyboardに関するデバイスドライバ
#define PORT_KEYDAT 0x0060
#define PORT_KEYSTA 0x0064
#define PORT_KEYCMD 0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE 0x60
#define KBC_MODE 0x47

static char ReceivedIRQ1 = 0;
typedef struct _FIFO{
    int fifo_array[30];
    unsigned int rear;//次にデータを入れる場所
    unsigned int front;//次にデータを取り出す場所
    unsigned int data_num;
    unsigned int max_len;
}FIFO;
static FIFO *fifo=0x8500;

//要素を追加する。
static void push(FIFO *fifo, int element){
    //fifo_arrayは、一杯か?
    if((fifo->rear+1)==fifo->front){
        return;
    }
    //rearは、一番最後を示しているか?
    if(fifo->rear==fifo->max_len){
        fifo->rear = 0;
        fifo->fifo_array[fifo->rear] = element;
        fifo->rear++;
        fifo->data_num++;
        return;
    }
    fifo->fifo_array[fifo->rear] = element;
    fifo->rear++;
    fifo->data_num++;
    return;
}

//要素を取り出す。
static int pop(){
    int element;
    //要素は空?
    if(fifo->front==fifo->rear){
        return -1;
    }
    //frontは、一番最後を示していルカ?
    if(fifo->front==fifo->max_len){
        fifo->front = 0;
        element = fifo->fifo_array[fifo->front];
        fifo->front++;
        fifo->data_num--;
        return element;
    }
    element = fifo->fifo_array[fifo->front];
    fifo->front++;
    fifo->data_num--;
    return element;
}

static void init_fifo(){
    fifo->rear  = 0;
    fifo->front = 0;
    fifo->data_num   = 0;
    fifo->max_len = 30;
    return;
}

void inthandler21(int *esp){

    out_8(PIC0_OCW2, 0x61);
    ReceivedIRQ1=1;
    unsigned char data;
    data = in_8(PORT_KEYDAT);

    if(data <= 127){
        push(fifo, data);   
    }

    return;
}

//キーボードコントローラからの送信待機
static void wait_kbc(){

    for(;;){
        if((in_8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY)==0){
            break;
        }
    }
    return;
}

void init_kbc(){
    init_fifo();
    wait_kbc();
    out_8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_kbc();
    out_8(PORT_KEYDAT, KBC_MODE);

    return;
}

//キーバッファの数をチェックする。
int check_fifo(){

    if(fifo->data_num >= 1){
        return pop();
    }

    return -1;
}