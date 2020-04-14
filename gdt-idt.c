#include "niwaka-os.h"

IDTR* idt_register=0x5000;
GDTR* gdt_register=0x5030;

//gdtを初期化する。
void gdt_init(){
    return;
}

//gdtrにロードする。
void gdtr_load(){
    gdt_register->base=0xbff7ed90;
    gdt_register->limit=0x3f;
    return;
}

//gdtのディスクリプたを作成する。
void create_gdt(unsigned int vector_number,unsigned int limit,unsigned int addr,unsigned int attr){
    GDT_GATE desc;
    gdtr_load();
        
    desc.limit_low = (short)(limit&0x0000ffff);
    desc.limit_high_and_attr_high = ((char)((limit&0x000f0000)>>16)) | ((char)((attr&0x00000f00)>>4));
    desc.attr_low = (char)((attr&0x000000ff));
    desc.offset_low = (short)((addr&0x0000ffff));
    desc.offset_middle = (char)((addr&0x00ff0000)>>16);
    desc.offset_high = (char)((addr&0xff000000)>>24);

    *((GDT_GATE *)gdt_register->base+vector_number)=desc;
    int *test=0x500;
    *(GDT_GATE*)test = desc;

    return;
}

//idtrの値を入手する。
static void idtr_store(IDTR* idtr){
    char *idtr_memory = 0x4000;
    //sidt(idtr_memory);
    //bfc5e010
    //7ff
    //idtr->limit = *((short*)(idtr_memory));
    //idtr->base  = *((int*)((int)idtr_memory+2));
    idtr->limit = 0x7ff;
    idtr->base  = 0xbfc5e010;
    //idtr->base  = 0x8000;
}

//IDTに登録するディスクリプタを作成する。
void create_idt(char vector_number,int offset,short attr,short selector){
    IDT_GATE desc;
    idtr_store(idt_register);

    desc.offset_high = (short)((offset & 0xFFFF0000) >> 16);
    desc.attr        = (short)attr;
    desc.selector    = (short)selector;
    desc.offset_low  = (short)(offset & 0x0000FFFF);

    //指定したvector numberの要素に、descriptorを書き込む。
    IDT_GATE *pointer;
    pointer = idt_register->base;   //IDTのベースアドレスをIDTRに代入

    *(pointer+vector_number) = desc;

    return;
}   