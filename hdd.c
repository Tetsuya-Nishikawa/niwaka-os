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
    //割り込み不可
    //disnable_hdd_irq();
    out_8(ALTERNATE_STATUS_DEVICE_CONTROL_REGISTER, 0x02);
    out_8(ERROR_FEATURES_REGISTER, features);
    out_8(SECTOR_COUNT_REGISTER, sector_count);
    out_8(SECTOR_NUMBER_REGISTER, sector_number);
    out_8(CYLINDER_LOW_REGISTER, cylinder_low);
    out_8(CYLINDER_HIGH_REGISTER, cylinder_high);
    out_8(DRIVE_HEAD_REGISTER, drive_head);
    out_8(STATUS_COMMAND_REGISTER, command);
    //ステータスレジスタを読む
    unsigned char status;
    status = in_8(ERROR_FEATURES_REGISTER);
    status = in_8(SECTOR_COUNT_REGISTER);
    status = in_8(SECTOR_NUMBER_REGISTER);
    status = in_8(CYLINDER_LOW_REGISTER);
    status = in_8(CYLINDER_HIGH_REGISTER);
    status = in_8(DRIVE_HEAD_REGISTER);
    status = in_8(STATUS_COMMAND_REGISTER);

    if ((status&ERROR)==0x00){

        return 1;
    }else{
        while(1){

        }
        return 0;
    }
}

//ソフトウェアリセット
static void reset_hdd(){
    unsigned char status;
    device_selection(0);
    issue_command(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, EXECUTE_DEVICE_DIAGNOSTIC);

}

//セクターを読み込む
//read_sectors(1, 1, 0, 0, 0x00, buff_address);
//シリンダ番号とヘッダ番号は0から
//セクタ番号は1から
void write_sectors(unsigned char sectors, unsigned char sector_number, unsigned char cylinder_low, unsigned char cylinder_high, unsigned char device_head, short *buff){
    unsigned char status;
    unsigned short data;
    unsigned int i;
    for(i=0; i<256; i++){
        *(buff+i) = 0x1111;
    }
    device_selection(0);
    issue_command(0x00, sectors, sector_number, cylinder_low, cylinder_high, device_head, WRITE_SECTORS);
    //in_8(ALTERNATE_STATUS_DEVICE_CONTROL_REGISTER);
    while(1){
        phase1:
            status = in_8(STATUS_COMMAND_REGISTER);
            if((status&0x81)==0x00){
                goto phase2;//ここ改良が必要。エラー処理がだめ
            }
        phase2:
            status = in_8(STATUS_COMMAND_REGISTER);
            if((status&0x88)==0x08){
                goto result;
            }else{
                goto phase1;
            }
    }

    result:
        for(i=0; i<256; i++){
            out_16(DATA_REGISTER, *(buff+i));
        }
        in_8(ALTERNATE_STATUS_DEVICE_CONTROL_REGISTER);
        in_8(STATUS_COMMAND_REGISTER);
    return;
}

//セクターを読み込む
//read_sectors(1, 1, 0, 0, 0x10, buff_address);
//シリンダ番号とヘッダ番号は0から
//セクタ番号は1から
void read_sectors(unsigned char sectors, unsigned char sector_number, unsigned char cylinder_low, unsigned char cylinder_high, unsigned char device_head, short *buff){
    unsigned char status;
    unsigned short data;

    unsigned int i;
    device_selection(0);
    issue_command(0x00, sectors, sector_number, cylinder_low, cylinder_high, device_head, READ_SECTORS);
    in_8(ALTERNATE_STATUS_DEVICE_CONTROL_REGISTER);
    while(1){
        phase1:
            status = in_8(STATUS_COMMAND_REGISTER);
            if((status&0x81)==0x00){
                goto phase2;//ここ改良が必要。エラー処理がだめ
            }
        phase2:
            status = in_8(STATUS_COMMAND_REGISTER);
            if((status&0x88)==0x08){
                goto result;
            }else{
                goto phase1;
            }
    }
    result:
        //512byte読み込む
        for(i=0; i<256; i++){
            data = in_16(DATA_REGISTER);
            *(buff+i) = data;
        }
        status = in_8(ALTERNATE_STATUS_DEVICE_CONTROL_REGISTER);
        status = in_8(STATUS_COMMAND_REGISTER);
    return;
}

void init_hdd(){
    disnable_hdd_irq();
    reset_hdd();
    read_sectors(1, 1, 0, 0, 0x00, 0x100);
    //write_sectors(1, 1, 0, 0, 0x00, 0x500);
    read_sectors(1, 1, 0, 0, 0x00, 0x3000);
    while(1){

    }
    return;
}