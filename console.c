#include "niwaka-os.h"
static void consbuf_to_vram();
static void console_print();
static void console_reset();
static void console_print();
static void console_setbuf(char *font, int col, int row);
static void new_prompt();
static void consbuf_to_vram_set(char *font, int col, int row);
char key_table[59]={0x00, 0x00, '1', '2', '3', '4', '5', '6', 0x37, 0x38, 0x39, 0x30,0x2d, 0x00, 0x00, 0x00, 0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x49,0x4f, 0x50, 0x40, 0x7b, 0x1c, 0x1d, 0x41, 0x53, 0x44, 0x46, 0x47, 0x48, 0x4a, 0x4b, 0x4c, 0x2b, 0x3a, 0x00, 0x2a, 0x7d, 0x5a, 0x58, 0x43, 0x56,0x42, 0x4e, 0x4d, 0x00, 0x2e, 0x00, 0x00,0x00, 0x00, 0x20};

typedef struct _CONSOLE{
    unsigned int x, y;               //今の座標
    unsigned int now_col, now_row;   //今、何行何列にいるのか
    unsigned int max_col, max_row;   //最大行と最大列
    PIXEL print_buf[800*600];         //コンソール画面の下書き
    unsigned int vram_start;         //VRAMの番地を用意する。
    unsigned int vram_end;           //実際は、vram_end-1が、最終番地
    unsigned int vram_high;          //画面の高さ
    unsigned int vram_side;          //画面の横
}CONSOLE;

//static CONSOLE *cons=0x00100000;
//static CONSOLE *cons;
static CONSOLE *cons;
static char prompt[16] ={0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x00,};

void console_main(){
	char data;
    char ascii_code;

	for(;;){
	
		data = check_fifo();
		if(data == -1){
            hlt();
			continue;
		}

        ascii_code = key_table[data];
        if((ascii_code >= '0' && ascii_code <= 'Z') || ascii_code==' '){
            consbuf_to_vram_set(font_ASCII[ascii_code], cons->now_col, cons->now_row);
            //console_setbuf(font_ASCII[ascii_code], cons->now_col, cons->now_row);
            //consbuf_to_vram();
            continue;
        }else if(data == 0x1c){//Enter keyを押したとき
            new_prompt();
            //最後の列まできた     
            if(cons->now_row >= cons->max_row){ //最後の行まできた
                cons->now_row = 0;
                console_reset();
            }
        }else if(data == 0x0e){
            //delete();//1文字削除
        }
    }
    return;
}

void delete(){
    consbuf_to_vram_set(font_ASCII[' '], cons->now_col-1, cons->now_row);
    return;
}

//コンソール初期化を行う。
void console_init(){
    cons=(CONSOLE*)alloc_memory(1920040);
    //cons = 0x00100000;
    //番地がなかった。
    cons->x=0;
    cons->y=0;
    cons->now_col=0;
    cons->now_row=1;
    cons->max_col=100;
    cons->max_row=37;
    cons->vram_start = *((int*)0x1010);
    cons->vram_end   = (cons->vram_start+800*600*4);
    cons->vram_high  = 600;
    cons->vram_side  = 800;
    //cons->vram_high  = *((int*)0x1000);
    //cons->vram_side  = *((int*)0x1008);

    //バッファの初期化を行う。
    for(int i=0; i < (cons->vram_high)*(cons->vram_side); i++){
        cons->print_buf[i].red   = 0x0;
        cons->print_buf[i].blue  = 0x0;
        cons->print_buf[i].green = 0x0;
    }

    console_print();
    console_main();
    return;
}

//consoleのバッファ領域に１文字だけかく
static void console_setbuf(char *font, int col, int row){
	char data;
    int i,j;
    int x, y;
    
    x = 8*col;
    y = 16*row;

    for(i=0; i < 16; i++){
		data = font[i];

		if((data & 0x80) != 0){
                cons->print_buf[x+cons->vram_side*(i+y)].blue = 0xff;
                cons->print_buf[x+cons->vram_side*(i+y)].red = 0xff;
                cons->print_buf[x+cons->vram_side*(i+y)].green = 0xff;
		}
		if((data & 0x40) != 0){
                cons->print_buf[x+1+cons->vram_side*(i+y)].blue = 0xff;
                cons->print_buf[x+1+cons->vram_side*(i+y)].red = 0xff;
                cons->print_buf[x+1+cons->vram_side*(i+y)].green = 0xff;

		}
		if((data & 0x20) != 0){
                cons->print_buf[x+2+cons->vram_side*(i+y)].blue = 0xff;
                cons->print_buf[x+2+cons->vram_side*(i+y)].red = 0xff;
                cons->print_buf[x+2+cons->vram_side*(i+y)].green = 0xff;

		}
		if((data & 0x10)  != 0){
                cons->print_buf[x+3+cons->vram_side*(i+y)].blue = 0xff;
                cons->print_buf[x+3+cons->vram_side*(i+y)].red = 0xff;
                cons->print_buf[x+3+cons->vram_side*(i+y)].green = 0xff;

		}
		if((data & 0x08) != 0){
                cons->print_buf[x+4+cons->vram_side*(i+y)].blue = 0xff;
                cons->print_buf[x+4+cons->vram_side*(i+y)].red = 0xff;
                cons->print_buf[x+4+cons->vram_side*(i+y)].green = 0xff;

		}
		if((data & 0x04) != 0){
                cons->print_buf[x+5+cons->vram_side*(i+y)].blue = 0xff;
                cons->print_buf[x+5+cons->vram_side*(i+y)].red = 0xff;
                cons->print_buf[x+5+cons->vram_side*(i+y)].green = 0xff;

		}
		if((data & 0x02) != 0){
                cons->print_buf[x+6+cons->vram_side*(i+y)].blue = 0xff;
                cons->print_buf[x+6+cons->vram_side*(i+y)].red = 0xff;
                cons->print_buf[x+6+cons->vram_side*(i+y)].green = 0xff;

		}
		if((data & 0x01) != 0){
                cons->print_buf[x+7+cons->vram_side*(i+y)].blue = 0xff;
                cons->print_buf[x+7+cons->vram_side*(i+y)].red = 0xff;
                cons->print_buf[x+7+cons->vram_side*(i+y)].green = 0xff;

		}
	}
    
    cons->now_col++;
    //最後の列まできた
    if(cons->now_col == cons->max_col){
        cons->now_col = 0;
        cons->now_row++;        
        if(cons->now_row >= cons->max_row){ //最後の行まできたs
            cons->now_row = 1;
            console_reset();
        }
    }
}

//コンソール画面を描画する。
static void console_print(){
    console_setbuf(font_ASCII['O'], cons->now_col, cons->now_row);
    console_setbuf(font_ASCII['S'], cons->now_col, cons->now_row);
    console_setbuf(prompt, cons->now_col, cons->now_row);   

    consbuf_to_vram();

    return;
}

//最大行まできたら、リセットする。
static void console_reset(){
    int x;
    int y;

    cons->now_col = 0;
    cons->now_row = 1;
    PIXEL *vram,*vram_now;
    vram = cons->vram_start;

    for(int i=0; i < cons->vram_side*cons->vram_high; i++){

            vram_now = vram + i;
            vram_now->blue = 0x0;
            vram_now->red = 0x0;
            vram_now->green = 0x0;
        
    }
    consbuf_to_vram_set(font_ASCII['O'], cons->now_col, cons->now_row);
    consbuf_to_vram_set(font_ASCII['S'], cons->now_col, cons->now_row);
    consbuf_to_vram_set(prompt, cons->now_col, cons->now_row);

    return;
}

//指定した部分だけ描画する。
static void consbuf_to_vram_set(char *font, int col, int row){
	char data;
    int i,j;
    int x, y;
    
    x = 8*col;
    y = 16*row;
    PIXEL *vram=cons->vram_start;
    PIXEL *vram_now;

    for(i=0; i < 16; i++){
		data = font[i];

		if((data & 0x80) != 0){
                vram_now = vram + x + cons->vram_side * (i+y);
                vram_now->blue = 0xff;
                vram_now->red = 0xff;
                vram_now->green = 0xff;           
		}
		if((data & 0x40) != 0){
                vram_now = vram + x + 1 +  cons->vram_side * (i+y);
                vram_now->blue = 0xff;
                vram_now->red = 0xff;
                vram_now->green = 0xff;    
		}
		if((data & 0x20) != 0){
                vram_now = vram + x + 2 +  cons->vram_side * (i+y);
                vram_now->blue = 0xff;
                vram_now->red = 0xff;
                vram_now->green = 0xff;    
		}
		if((data & 0x10)  != 0){
                vram_now = vram + x + 3 +  cons->vram_side * (i+y);
                vram_now->blue = 0xff;
                vram_now->red = 0xff;
                vram_now->green = 0xff;    
		}
		if((data & 0x08) != 0){
                vram_now = vram + x + 4 +  cons->vram_side * (i+y);
                vram_now->blue = 0xff;
                vram_now->red = 0xff;
                vram_now->green = 0xff;    
		}
		if((data & 0x04) != 0){
                vram_now = vram + x + 5 +  cons->vram_side * (i+y);
                vram_now->blue = 0xff;
                vram_now->red = 0xff;
                vram_now->green = 0xff;    
		}
		if((data & 0x02) != 0){
                vram_now = vram + x + 6 +  cons->vram_side * (i+y);
                vram_now->blue = 0xff;
                vram_now->red = 0xff;
                vram_now->green = 0xff;    
		}
		if((data & 0x01) != 0){
                vram_now = vram + x + 7 +  cons->vram_side * (i+y);
                vram_now->blue = 0xff;
                vram_now->red = 0xff;
                vram_now->green = 0xff;    
		}
	}
    
    cons->now_col++;
    //最後の列まできた

    if(cons->now_col == cons->max_col){
        cons->now_col = 0;
        cons->now_row++;        
        if(cons->now_row >= cons->max_row){ //最後の行まできたs
            cons->now_row = 1;
            console_reset();
        }
    }
    return;
}

//コンソールのバッファからVRAMへ
static void consbuf_to_vram(){
    int i;

    PIXEL *vram=cons->vram_start;
    PIXEL *vram_now;

    for(i=0; i < cons->vram_side*cons->vram_high; i++){

            vram_now = vram + i;
            vram_now->blue = cons->print_buf[i].blue;
            vram_now->red = cons->print_buf[i].red;
            vram_now->green = cons->print_buf[i].green;        
    }
    return;
}

static void new_prompt(){
    cons->now_row++;
    cons->now_col=0;
    //最終列まで来た時
    if(cons->now_row >= cons->max_row){
        //コンソールリセットをする。
        console_reset(cons);
        return;
    }
    //console_setbuf(font_ASCII['O'], cons->now_col, cons->now_row);
    //console_setbuf(font_ASCII['S'], cons->now_col, cons->now_row);
    //console_setbuf(prompt, cons->now_col, cons->now_row);   
    consbuf_to_vram_set(font_ASCII['O'], cons->now_col, cons->now_row);
    consbuf_to_vram_set(font_ASCII['S'], cons->now_col, cons->now_row);
    consbuf_to_vram_set(prompt, cons->now_col, cons->now_row);

    //consbuf_to_vram(cons);
    return;
}

int strncmp(char *buff1, char *buff2, int size){

    for(int i=0; i < size; i++){
        if(*(buff1)!=*(buff2)){
            return -1;
        }
    }

    return 1;
}


//0c006080  2048 OBJECT  GLOBAL DEFAULT    4 font_ASCII
//201351296
//1056
// 0c004158    16 OBJECT  GLOBAL DEFAULT    3 font_A
//k2599317
//niwka_dev Tetunosuke3822

/***整数を文字列に治す方法に使えるかも?
N,K = map(int, input().split())

count = 0
while N>0:
    N = N//K
    count = count + 1
print(count)
***/

//口を閉じる
