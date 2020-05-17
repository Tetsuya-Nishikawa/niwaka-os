#include "niwaka-os.h"

//デバイスセレクションプロトコル
void device_selection(unsigned char selector_number){
    //statusレジスタをリードする
    unsigned char flg;
    flg = in_8(STATUS_COMMAND_REGISTER);
    
    //0x5850
    while ((flg&DEVICE_READY)!=0x0){
        flg = in_8(STATUS_COMMAND_REGISTER);
    }
    //デバイス番号をセットする
    //out_8(DRIVE_HEAD_REGISTER, selector_number<<4);
    out_8(DRIVE_HEAD_REGISTER, 0xA0);
    flg = in_8(STATUS_COMMAND_REGISTER);
    while ((flg&DEVICE_READY)!=0x0){
        flg = in_8(STATUS_COMMAND_REGISTER);
    }
    
    return;
}

//ソフトウェアリセット
void reset_hdd(){
    device_selection(0);
    issue_command(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, EXECUTE_DEVICE_DIAGNOSTIC);
    //ここで割り込みを待つ

}

//コマンドを発行する
//1：成功
//0：失敗
unsigned char issue_command(unsigned char features, unsigned char sector_count, unsigned char sector_number, unsigned char cylinder_low, unsigned char cylinder_high, unsigned char drive_head, unsigned char command){
    out_8(ERROR_FEATURES_REGISTER, features);
    out_8(SECTOR_COUNT_REGISTER, sector_count);
    out_8(SECTOR_NUMBER_REGISTER, sector_number);
    out_8(CYLINDER_LOW_REGISTER, cylinder_low);
    out_8(CYLINDER_HIGH_REGISTER, cylinder_high);
    out_8(DRIVE_HEAD_REGISTER, drive_head);
    out_8(STATUS_COMMAND_REGISTER, command);
    //ステータスレジスタを読む
    unsigned char status;
    status = in_8(STATUS_COMMAND_REGISTER);
    if (status&ERROR==0x00){

        return 1;
    }else{
        return 0;
    }
}