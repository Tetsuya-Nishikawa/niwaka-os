#define CONFIG_ADDRESS 0x0CF8
#define CONFIG_DATA 0x0CFC

unsigned int BAR0;
unsigned int BAR1;
unsigned int __attribute__ ((aligned(64))) MMIO_BASE;

//コンテキストのポインタの配列
unsigned char *context_array[0x40];
void Init_ContextTable(unsigned long long *dcbaap);

typedef struct _USBCMD{
    unsigned int usbcmd_registers;
}USBCMD;

typedef struct _USBSTS{
    unsigned int usbsts_registers;
}USBSTS;

typedef struct _TRB{
    unsigned int parameter;
    unsigned int status;
    unsigned short type;
    unsigned short control;
}TRB;

typedef struct _LINK_TRB{
    unsigned int* ring_segment_pointer_lo;
    unsigned int* ring_segment_pointer_hi;
    unsigned int type1;
    unsigned int type2;
}LINK_TRB;

typedef struct _RING_MANAGER{
    TRB buf[8];
    int cycle_bit;
    int write_index;
}RING_MANAGER;

//xHCIのコンフィグレーション空間を探す。
void search(){

    RING_MANAGER  *ring_manager=0x0;

    unsigned int i, j;
    unsigned int data;
    unsigned int *test=0x300000;
    unsigned int k=0;
	unsigned int device_number;
	unsigned int buss_number;

    unsigned char *CapablitiyRegisterLength;
    unsigned int  *DoorbellOffset;
    unsigned int  *RuntimeRegisterSpaceOffset;
    unsigned int *OperationalRegisters;
    unsigned int *HCSPARAMS1;
    unsigned int *CONFIG;
    unsigned int *CRCR;

    for(i=0; i < 32; i++){
        for(j=0; j < 256; j++){
            out_32(CONFIG_ADDRESS, 0x80000000 | j << 16 |  i << 11 | 0x0 << 8);
            data = in_32(CONFIG_DATA);
            *(test+k) = data;
            k++;
			data = data & 0x0000ffff;
			if(data != 0x0000ffff){

            	out_32(CONFIG_ADDRESS, 0x80000000 | j << 16 |  i << 11 | 0x0 << 8 | 0x08);		
            	data = in_32(CONFIG_DATA);
				data = data & 0xFFFFFF00;
			
				if(data == 0x0c033000){
					*(test+3) = data;
					out_32(CONFIG_ADDRESS, 0x80000000 | j << 16 |  i << 11 | 0x0 << 8 | 0x10);		
					data = in_32(CONFIG_DATA);
					BAR0 = data;
					out_32(CONFIG_ADDRESS, 0x80000000 | j << 16 |  i << 11 | 0x0 << 8 | 0x14);		
					data = in_32(CONFIG_DATA);
					BAR1 = data;

					goto end;
				}		
			}
        }   
    }

	end:
		device_number = i;
		buss_number = j;

	*(test+1) = BAR0;
	*(test+2) = BAR1;
	MMIO_BASE = BAR0 & 0xfffffff0;
	//Capabilities_Pointer = out_32(CONFIG_ADDRESS, 0x80000000 | j << 16 |  i << 11 | 0x0 << 8 | 0x34);
	//Capabilities_Pointer = Capabilities_Pointer + ; 
    CapablitiyRegisterLength   = (unsigned char*)MMIO_BASE;
    DoorbellOffset             = (unsigned int *)(MMIO_BASE+0x14);
    RuntimeRegisterSpaceOffset = (unsigned int*)(MMIO_BASE+0x18);
    OperationalRegisters       = MMIO_BASE + *CapablitiyRegisterLength;
    HCSPARAMS1                 = (unsigned int *)(MMIO_BASE+0x04);
    CONFIG                     = ((unsigned int)OperationalRegisters) + 0x38;
    CRCR                       = ((unsigned int)OperationalRegisters) + 0x18;
    unsigned int *test10 = 0x0;
    *test10 = *HCSPARAMS1;
    for(;;){

    }
    reset_xhc(OperationalRegisters);
    Init_ContextTable(((unsigned int)OperationalRegisters) + 0x30);

    return;
}

void reset_xhc(unsigned int *OperationalRegisters){
    USBCMD *usbcmd;
    USBSTS *usbsts;

    usbcmd = OperationalRegisters;
    usbsts = OperationalRegisters + 1;

    //USBSTS.HCHが1であることを確認する。
    if((usbsts->usbsts_registers&0x00000001) != 0){
        usbcmd->usbcmd_registers = usbcmd->usbcmd_registers | 0x00000002;
        //USBCMD.HSCRSTが0になるまでまつ
        while((usbcmd->usbcmd_registers&0x00000002) != 0){

        }
        //USBSTS.CNRが0になるまでまつ
        while((usbsts->usbsts_registers&0x00000800) != 0){

        }

    }

    return;
}   

void Init_ContextTable(unsigned long long *dcbaap){
    int i;

    for(i=0; i < 0x40; i++){
        context_array[i] = 0;
    }

    *(dcbaap) = (unsigned int)context_array << 6;

    return;
}

void Make_CommandRing(){

}