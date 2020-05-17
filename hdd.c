#include "niwaka-os.h"

//デバイスセレクションプロトコル
static void device_selection(unsigned char selector_number){
    //statusレジスタをリードする
    unsigned char flg;
    flg = in_8(STATUS_COMMAND_REGISTER);
    
    while ((flg&DEVICE_READY)!=0x0){
        flg = in_8(STATUS_COMMAND_REGISTER);
    }
    //デバイス番号をセットする
    out_8(DRIVE_HEAD_REGISTER, selector_number<<4);
    //out_8(DRIVE_HEAD_REGISTER, 0xA0);
    flg = in_8(STATUS_COMMAND_REGISTER);
    while ((flg&DEVICE_READY)!=0x0){
        flg = in_8(STATUS_COMMAND_REGISTER);
    }
    
    return;
}

//IRQを無効にする
static void disnable_hdd_irq(){
    unsigned char data=0x02;
    out_8(ALTERNATE_STATUS_DEVICE_CONTROL_REGISTER, data);
    return;
}

//コマンドを発行する
//1：成功
//0：失敗
static unsigned char issue_command(unsigned char features, unsigned char sector_count, unsigned char sector_number, unsigned char cylinder_low, unsigned char cylinder_high, unsigned char drive_head, unsigned char command){
    int i;
    out_8(ERROR_FEATURES_REGISTER, features);
    out_8(SECTOR_COUNT_REGISTER, sector_count);
    out_8(SECTOR_NUMBER_REGISTER, sector_number);
    out_8(CYLINDER_LOW_REGISTER, cylinder_low);
    out_8(CYLINDER_HIGH_REGISTER, cylinder_high);
    out_8(DRIVE_HEAD_REGISTER, drive_head);
    out_8(STATUS_COMMAND_REGISTER, command);
    //ステータスレジスタを読む
    unsigned char status;
    for(i=0; i<6; i++){
        if(i==0){
            status = in_8(STATUS_COMMAND_REGISTER);
            if(status&0x80==0x00){
                return 0;
            }
        }
        status = in_8(STATUS_COMMAND_REGISTER);
    }
    status = in_8(STATUS_COMMAND_REGISTER);
    if (status&ERROR==0x00){

        return 1;
    }else{
        return 0;
    }
}

//ソフトウェアリセット
static void reset_hdd(){
    device_selection(0);
    issue_command(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, EXECUTE_DEVICE_DIAGNOSTIC);
}

void init_hdd(){
    disnable_hdd_irq();
    reset_hdd();
    disnable_hdd_irq();
}