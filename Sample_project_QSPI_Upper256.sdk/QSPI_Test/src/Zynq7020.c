#if(1)  //----------------------   S: Header Define What Is This File?
#endif  //----------------------   E: Header Define What Is This File?

#if(1)  //----------------------   S: Header Include
#include "Zynq7020.h"
#endif  //----------------------   E: Header Incloud

#if(1)  //----------------------   S: Interrupt
    void NVIC_SystemReset(void){
         {xil_printf("@@@@@@@@@ Zynq System Reset_1\n"); Delay_Xil_Printf;}
        usleep(10000);
        Xil_Out32(XPAR_XSLCR_0_BASEADDR + SLCR_UNLOCK_ADDRESS,SLCR_UNLOCK_KEY);
        Xil_Out32(XPAR_XSLCR_0_BASEADDR + SLCR_SYSTEM_RESET,0x01);
    }
 #endif  //----------------------   E: Interrupt
#if(1)  //----------------------   S: Flash
    void FlashErase(XQspiPs *QspiPtr, u32 sector){
        u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
        u8 ReadStatusCmd[] = { READ_STATUS_CMD, 0 };  /* must send 2 bytes */
        u8 FlashStatus[2];

        u32 Address = sector * SECTOR_SIZE;

        XQspiPs_PolledTransfer(&QspiInstance, &WriteEnableCmd, NULL, sizeof(WriteEnableCmd));


        if(FLASH_TYPE == SPANTION_TYPE)
            WriteBuffer[COMMAND_OFFSET]   = 0x17;
        else
            WriteBuffer[COMMAND_OFFSET]   = 0xC5;
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)(Address >> 24);

        XQspiPs_PolledTransfer(&QspiInstance, WriteBuffer, NULL,2);
        while (1) {
            XQspiPs_PolledTransfer(&QspiInstance, ReadStatusCmd, FlashStatus,sizeof(ReadStatusCmd));
            if ((FlashStatus[1] & 0x01) == 0) {
                break;
            }
        }

        XQspiPs_PolledTransfer(QspiPtr, &WriteEnableCmd, NULL, sizeof(WriteEnableCmd));

        WriteBuffer[COMMAND_OFFSET]   = SEC_ERASE_CMD;
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)(Address >> 16);
        WriteBuffer[ADDRESS_2_OFFSET] = (u8)(Address >> 8);
        WriteBuffer[ADDRESS_3_OFFSET] = (u8)(Address & 0xFF);
        XQspiPs_PolledTransfer(QspiPtr, WriteBuffer, NULL, SEC_ERASE_SIZE);
        while (1) {
            XQspiPs_PolledTransfer(QspiPtr, ReadStatusCmd, FlashStatus, sizeof(ReadStatusCmd));
            if ((FlashStatus[1] & 0x01) == 0) {
                break;
            }
        }
    }
    int FlashReadID(void)
    {
        int Status;
        WriteBuffer[COMMAND_OFFSET]   = READ_ID;
        WriteBuffer[ADDRESS_1_OFFSET] = 0x23;
        WriteBuffer[ADDRESS_2_OFFSET] = 0x08;
        WriteBuffer[ADDRESS_3_OFFSET] = 0x09;

        Status = XQspiPs_PolledTransfer(&QspiInstance, WriteBuffer, ReadBuffer, RD_ID_SIZE);
        return Status+(ReadBuffer[1]<<8)+ (ReadBuffer[2]<<16)+(ReadBuffer[3]<<24);
    }
    void FlashQuadEnable(XQspiPs *QspiPtr)
    {
        u8 WriteEnableCmd = {WRITE_ENABLE_CMD};
        u8 ReadStatusCmd[] = {READ_STATUS_CMD, 0};
        u8 QuadEnableCmd[] = {WRITE_STATUS_CMD, 0, 0};
        u8 FlashStatus[2];

        if (FLASH_TYPE == SPANTION_TYPE || FLASH_TYPE == WINBOND_TYPE || FLASH_TYPE == MICRON_TYPE)  {
            XQspiPs_PolledTransfer(QspiPtr, ReadStatusCmd, FlashStatus, sizeof(ReadStatusCmd));
            QuadEnableCmd[1] = FlashStatus[1];
            QuadEnableCmd[2] = 0x02;
            XQspiPs_PolledTransfer(QspiPtr, &WriteEnableCmd, NULL, sizeof(WriteEnableCmd));
            XQspiPs_PolledTransfer(QspiPtr, QuadEnableCmd, NULL, sizeof(QuadEnableCmd));
        }
        else
        {
            while(1)
                 {xil_printf("!!! FlashQuadEnable: Unsupported Flash Type !!!\r\n"); Delay_Xil_Printf;}
        }

    }
    void EEPROM1_Flash0_Read(uint16_t page_number, uint16_t sector_number, void* data, EEPROM_Mode_Type mode, uint32_t count, uint8_t MMC_1_Flash_0){
        uint16_t start_data=0;
        switch (mode){
            case MODE_8_BIT: break;
            case MODE_16_BIT: count=2*count; break;
            case MODE_32_BIT: count=4*count; break;
            case MODE_64_BIT: count=8*count; break;
        }

        if(MMC_1_Flash_0==0){
            u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
            u8 ReadStatusCmd[] = { READ_STATUS_CMD, 0 };
            u8 FlashStatus[2];

            //-----S: More Than 256
            if(page_number==1000){
                page_number=0;
                start_data=64*(sector_number % 4);
                sector_number=237+sector_number/4;
            }
            //-----E: More Than 256

            u32 Address = sector_number*SECTOR_SIZE + page_number * PAGE_SIZE;

            XQspiPs_PolledTransfer(&QspiInstance, &WriteEnableCmd, NULL, sizeof(WriteEnableCmd));

            if(FLASH_TYPE == SPANTION_TYPE)
                WriteBuffer[COMMAND_OFFSET]   = 0x17;
            else
                WriteBuffer[COMMAND_OFFSET]   = 0xC5;


            WriteBuffer[ADDRESS_1_OFFSET] = (u8)(Address >> 24);

            XQspiPs_PolledTransfer(&QspiInstance, WriteBuffer, NULL,2);
            while (1) {
                XQspiPs_PolledTransfer(&QspiInstance, ReadStatusCmd, FlashStatus,sizeof(ReadStatusCmd));
                if ((FlashStatus[1] & 0x01) == 0) {
                    break;
                }
            }

            WriteBuffer[COMMAND_OFFSET]   = READ_CMD;
            WriteBuffer[ADDRESS_1_OFFSET] = (u8)((Address & 0xFF0000) >> 16);
            WriteBuffer[ADDRESS_2_OFFSET] = (u8)((Address & 0xFF00) >> 8);
            WriteBuffer[ADDRESS_3_OFFSET] = (u8)(Address & 0xFF);
            XQspiPs_PolledTransfer(&QspiInstance, WriteBuffer, ReadBuffer, count + start_data + DATA_OFFSET);
            for(int i=start_data; i<count+start_data; i++){
                ((u8 *)data)[i-start_data]=ReadBuffer[i + DATA_OFFSET];
            }
        }
    }
    void Flash_Erase(uint16_t sector_address){
        FlashErase(&QspiInstance, sector_address);
    }
    void EEPROM1_Flash0_Write(uint16_t page_number, uint16_t sector_number, void* data_Write, EEPROM_Mode_Type mode, uint32_t count){
        int i;

        switch (mode){
            case MODE_8_BIT: break;
            case MODE_16_BIT: count=2*count; break;
            case MODE_32_BIT: count=4*count; break;
            case MODE_64_BIT: count=8*count; break;
        }

        u8 data_256[256]={0};
        u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
        u8 ReadStatusCmd[] = { READ_STATUS_CMD, 0 };
        u8 FlashStatus[2];

        u32 Address = sector_number*SECTOR_SIZE + page_number * PAGE_SIZE;

        XQspiPs_PolledTransfer(&QspiInstance, &WriteEnableCmd, NULL, sizeof(WriteEnableCmd));

        if(FLASH_TYPE == SPANTION_TYPE)
            WriteBuffer[COMMAND_OFFSET]   = 0x17;
        else
            WriteBuffer[COMMAND_OFFSET]   = 0xC5;
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)(Address >> 24);

        XQspiPs_PolledTransfer(&QspiInstance, WriteBuffer, NULL,2);
        while (1) {
            XQspiPs_PolledTransfer(&QspiInstance, ReadStatusCmd, FlashStatus,sizeof(ReadStatusCmd));
            if ((FlashStatus[1] & 0x01) == 0) {
                break;
            }
        }

        XQspiPs_PolledTransfer(&QspiInstance, &WriteEnableCmd, NULL, sizeof(WriteEnableCmd));
        WriteBuffer[COMMAND_OFFSET]   = WRITE_CMD;
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)((Address & 0xFF0000) >> 16);
        WriteBuffer[ADDRESS_2_OFFSET] = (u8)((Address & 0xFF00) >> 8);
        WriteBuffer[ADDRESS_3_OFFSET] = (u8)(Address & 0xFF);
        for(int i=0; i<count; i++){
			WriteBuffer[i+DATA_OFFSET]=((u8 *)data_Write)[i];
        }
        XQspiPs_PolledTransfer(&QspiInstance, WriteBuffer, NULL, count + DATA_OFFSET);
        while (1) {
            XQspiPs_PolledTransfer(&QspiInstance, ReadStatusCmd, FlashStatus,sizeof(ReadStatusCmd));
            if ((FlashStatus[1] & 0x01) == 0) {
                break;
            }
        }
    }
    void Flash_Init(void){
        XQspiPs_Config *QspiConfig;
        QspiConfig = XQspiPs_LookupConfig(XPAR_XQSPIPS_0_DEVICE_ID);
        XQspiPs_CfgInitialize(&QspiInstance, QspiConfig, QspiConfig->BaseAddress);
        XQspiPs_SetOptions(&QspiInstance, XQSPIPS_MANUAL_START_OPTION | XQSPIPS_FORCE_SSELECT_OPTION | XQSPIPS_HOLD_B_DRIVE_OPTION);
        XQspiPs_SetClkPrescaler(&QspiInstance, XQSPIPS_CLK_PRESCALE_8);
        XQspiPs_SetSlaveSelect(&QspiInstance);
        FlashIdNum = FlashReadID();
        FLASH_TYPE = (u8)(FlashIdNum >> 8);
        FlashQuadEnable(&QspiInstance);
    }

#endif  //----------------------   E: Flash
#if(1)  //----------------------   S: Set For Bootloader
    u8 getJumptoApp(){
        return (Xil_In32(0xF800702C) >> 24);
    }
    u8 getNeedforUpdate(){
        return (Xil_In32(0xF800702C) >> 20)  & (~(0xFF << 4));
    }
    u8 getDongleFlag(){
        return (Xil_In32(0xF800702C) >> 16)  & (~(0xFFF << 4));
    }
    void clearJumptoApp(){
        Xil_Out32(0xF800702C, Xil_In32(0xF800702C) & (~(0xFF << 24)));  // JumptoApp = 0
         {xil_printf("@@@@@@@@@ App: JumpToApp clear to %x\n",(Xil_In32(0xF800702C) >> 24)); Delay_Xil_Printf;}
    }
    void setJumptoApp(){
        clearJumptoApp();
        Xil_Out32(0xF800702C,Xil_In32(0xF800702C) | (0x4A << 24));   // JumptoApp = 'J'
         {xil_printf("@@@@@@@@@ SSBL: JumpToApp set to %x\n",(Xil_In32(0xF800702C) >> 24)); Delay_Xil_Printf;}
    }
    void incrementJumptoApp(){
        JumpToApplication = (Xil_In32(0xF800702C) >> 24);
        JumpToApplication = JumpToApplication + 1;
        clearJumptoApp();
        Xil_Out32(0xF800702C,(Xil_In32(0xF800702C) | (JumpToApplication) << 24));   // JumptoApp++
         {xil_printf("@@@@@@@@@ SSBL: JumpToApp is incremented to %x\n",(Xil_In32(0xF800702C) >> 24)); Delay_Xil_Printf;}
    }
    void clearNeedforUpdate(){
        Xil_Out32(0xF800702C,Xil_In32(0xF800702C) & (~(0x00F << 20)));    // NeedforUpdate = 0
         {xil_printf("@@@@@@@@@ SSBL: NeedForUpdate clear to %x\n",(Xil_In32(0xF800702C) >> 20)  & (~(0xFF << 4))); Delay_Xil_Printf;}
    }
    void setNeedforUpdate(){
        clearNeedforUpdate();
        Xil_Out32(0xF800702C,Xil_In32(0xF800702C) | (0x00A << 20));    // NeedforUpdate = 0xA
         {xil_printf("@@@@@@@@@ SSBL: NeedForUpdate set to %x\n",(Xil_In32(0xF800702C) >> 20)  & (~(0xFF << 4))); Delay_Xil_Printf;}
    }
    void clearDongleFlag(){
        Xil_Out32(0xF800702C,Xil_In32(0xF800702C) & (~(0x000F << 16)));    // NeedforUpdate = 0
         {xil_printf("@@@@@@@@@ SSBL: DongleFlag clear to %x\n",(Xil_In32(0xF800702C) >> 16)  & (~(0xFFF << 4))); Delay_Xil_Printf;}
    }
    void setDongleFlag(){
        clearDongleFlag();
        Xil_Out32(0xF800702C,Xil_In32(0xF800702C) | (0x0005 << 16));   // JumptoApp = 5
         {xil_printf("@@@@@@@@@ SSBL: DongleFlag set to %x\n",(Xil_In32(0xF800702C) >> 16)  & (~(0xFFF << 4))); Delay_Xil_Printf;}
    }
#endif  //----------------------   E: Set For Bootloader
