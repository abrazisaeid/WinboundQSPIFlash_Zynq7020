#if(1)  //----------------------   S: Header Define What Is This File?
#endif  //----------------------   E: Header Define What Is This File?

#if(1)  //----------------------   S: Header Include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sleep.h"
#include "xgpiops.h"
#include "xil_types.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xparameters_ps.h"
#include "xqspips.h"
#include "stdbool.h"
#endif  //----------------------   E: Header Incloud

#if(1)  //----------------------   S: General
    #define MAX(a, b) (((a) > (b)) ? (a) : (b))
    #define IsFRA (Main_Version>200 && Main_Version<210)
    #define Const_For_Vivado 10000000
    #define Counter_If_Xil_Print  100
    #define Delay_Xil_Printf  for(int ooooo=0; ooooo<Counter_If_Xil_Print; ooooo++)


    typedef enum {
         DISABLE = 0,
         ENABLE = !DISABLE
    } FunctionalState;
    typedef enum { // ---------- AV_Delataphi -----------------------------
        __AV = 0,
        __Phi,
    } AV_Delataphi;
    typedef enum { // ---------- CaseNumbers -----------------------------
        CaseNumbers_None                         = 0,
        CaseNumbers_Start                        = 1,
        CaseNumbers_DC                           = 2,
        CaseNumbers_DCReset                      = 3,
        CaseNumbers_Sync                         = 4,
        CaseNumbers_Next_Actual                  = 6,
        CaseNumbers_HardwareConfig               = 8,
        CaseNumbers_GooseConfig                  = 9,
        CaseNumbers_SVConfig                     = 10,
        CaseNumbers_Alive                        = 12,
        CaseNumber_TestLAN                       = 13,
        CaseNumbers_FrameInfo                    = 16,
        CaseNumbers_StopTest                     = 64,
        CaseNumbers_FillRam                      = 123,
        CaseNumbers_FlashInBootLoaderMode        = 167,
        CaseNumbers_ReadIpConfig                 = 183,
        CaseNumbers_WriteIpConfig                = 199,
        CaseNumbers_ClearError                   = 2048,
        CaseNumbers_ClearCounterErrorOvercurrent = 3072,
        CaseNumbers_ResetHardware                = 4096,
        CaseNumbers_KeyTrigger                   = 8192,
        CaseNumbers_UploadCalibration            = 16384,
        CaseNumbers_UploadCalibration_FRA        = 16385,
        CaseNumbers_DownloadCalibration          = 32768,
        CaseNumbers_EraseCalibrationForFlash     = 32769,
        CaseNumbers_DC_GetActualValue            = 65537,
        CaseNumbers_CalibrationOptoHall          = 131072,
        CaseNumbers_FlashInProgramMode           = 123456123,
    } CaseNumbers;

    #define PercisionCompare                   0.000000001
    #define Number_Of_VI                       12
    #define FRA_Number_Of_Resistors            8
    #define FRA_Number_Of_Gain                 8
    #define AV_Phi                             2
    #define FRA_Number_Of_Frequncy             42

    uint32_t     Main_Version;

    void Correct_Serial_Number(void);
    uint32_t Correct_Serial_Number_Specific(int serial_Fuck);
    void Zynq7020_Init(void);
    void Detect_Main_Version(void);
    int8_t IsGreaterOrEqual(double x1, double x2);
    int8_t IsLessOrEqual(double x1, double x2);
    int8_t IsEqual(double x1, double x2);
    int32_t Calculate_Calib(double x1, double x2, double y1, double y2, double x);

#endif  //----------------------   E: General
#if(1)  //----------------------   S: Interrupt

    #define SLCR_LOCK_ADDRESS                                                   0x00000004
    #define SLCR_UNLOCK_ADDRESS                                                 0x00000008
    #define SLCR_LOCK_KEY                                                       0x767B
    #define SLCR_UNLOCK_KEY                                                     0xDF0D

    #define SLCR_SYSTEM_RESET                                                   0x00000200
    #define SLCR_A9_CPU_RST_CTRL                                                0x00000244
    #define SLCR_RS_AWDT_CTRL                                                   0x0000024C

    void                       NVIC_SystemReset(void);
#endif  //----------------------   E: Interrupt
#if(1)  //----------------------   S: Flash
    #define WRITE_STATUS_CMD            0x01
    #define WRITE_CMD                   0x02
    #define READ_CMD                    0x03
    #define WRITE_DISABLE_CMD           0x04
    #define READ_STATUS_CMD             0x05
    #define WRITE_ENABLE_CMD            0x06
    #define FAST_READ_CMD               0x0B
    #define DUAL_READ_CMD               0x3B
    #define QUAD_READ_CMD               0x6B
    #define BULK_ERASE_CMD              0xC7
    #define SEC_ERASE_CMD               0xD8
    #define READ_ID                     0x9F
    #define COMMAND_OFFSET              0
    #define ADDRESS_1_OFFSET            1
    #define ADDRESS_2_OFFSET            2
    #define ADDRESS_3_OFFSET            3
    #define DATA_OFFSET                 4
    #define DUMMY_OFFSET                4
    #define DUMMY_SIZE                  1
    #define RD_ID_SIZE                  4
    #define BULK_ERASE_SIZE             1
    #define SEC_ERASE_SIZE              4
    #define OVERHEAD_SIZE               4
    #define SECTOR_SIZE                 0x10000
    #define SECTOR_NUM                  0x200
    #define PAGE_SIZE                   0x100
    #define PAGE_NUM_IN_SECTOR          0x100
    #define Start_Sector_Calibration    (SECTOR_NUM-0x40)
    #define SPANTION_TYPE               0x01
    #define WINBOND_TYPE                0xEF
    #define MICRON_TYPE                 0x20

    typedef enum{
    MODE_8_BIT = 0,
    MODE_16_BIT,
    MODE_32_BIT,
    MODE_64_BIT
    } EEPROM_Mode_Type;
    typedef enum {
        XST_OK = (uint8_t)0x00, XST_ERROR = (uint8_t)0x01, XST_BUSY = (uint8_t)0x02, XST_TIMEOUT = (uint8_t)0x03, XST_UNKNOWN = (uint8_t)0x04
    } XST_StatusTypeDef;
    typedef struct {
        uint8_t                    startData;
        int32_t                    Thermal_Board_Controller[Number_Of_VI];

        uint32_t                   Serial;
        uint32_t                   ClockCalibration;
        uint8_t                    IPConfig[16];
        uint16_t                   Port;
        uint32_t                   MinVersion;
        uint8_t                    MAC[6];
        int32_t                    Analog_Output_DAs_Main_AddValue_Average[Number_Of_VI];
        int32_t                    Analog_Output_DAs_Main_AddValue[Number_Of_VI];
        int32_t                    Analog_Output_DAs_Main[4 * Number_Of_VI];
        int32_t                    Analog_Output_DAs_Cros[4 * Number_Of_VI];
        int32_t                    Analog_Output_ADs_DC_Main[4 * Number_Of_VI];
        int32_t                    Analog_Output_ADs_DC_Cros[4 * Number_Of_VI];
        int32_t                    Binary_Input[126];
        int32_t                    Analog_Input[72];
        int32_t                    BinaryOutput[32];
        int32_t                    Analog_Output_ADs_AC_Cros[2 * Number_Of_VI];
        int32_t                    Analog_Output_ADs_AC_Main[2 * Number_Of_VI];
        int32_t                    Analog_Output_Shunt[Number_Of_VI];
        int32_t                    Calibrate_Time_VA1_With_GPS;
        int32_t                    ExclusionSerial[16];
    } DeviceCfg;

    XQspiPs                             QspiInstance;
    unsigned                            char FLASH_TYPE;
    uint8_t                             ReadBuffer[SECTOR_SIZE + DATA_OFFSET];
    uint8_t                             WriteBuffer[SECTOR_SIZE + DATA_OFFSET];
    int32_t                             FlashIdNum;
    uint8_t                             MMC_Flash_MMCOk1_MMChNotOk0;
    uint8_t                             Adade_Khas_25_42_211_18[4];
    uint8_t                             MMC_Flash_FlashOk1_FlashNotOk0;
    uint8_t                             MMC_Flash_CompareOk1_CompareNotOk0;
    uint8_t                             New_SSBL;

    DeviceCfg                           ReadMyCfg, ReadMyCfg_Compare;

    void                                Flash_Init(void);
    void                                EEPROM1_Flash0_Write(uint16_t page_number, uint16_t sector_number, void* data_Write, EEPROM_Mode_Type mode, uint32_t count);
    void                                EEPROM1_Flash0_Read(uint16_t page_number, uint16_t sector_number, void* data, EEPROM_Mode_Type mode, uint32_t count, uint8_t MMC_1_Flash_0);
    void                                Flash_Erase(uint16_t sector_address);
    u8                                  EraseSector(uint32_t start_sec, uint32_t end_sec);
    void                                Init_MMC_Flash_Check_Validity_If_First_Write_To_Flash();

#endif  //----------------------   E: Flash
#if(1)  //----------------------   S: Set For Bootloader
    #define LAST_SSBL_SECTOR_NUM                    103
    #define LAST_SSBL_PAGE                          255
    #define DESTROYING_PARTITION_HEADER_SECTOR      167
    #define LINUX_PARTITION_HEADER_SECTOR           189
    #define BootloaderApp                           0

    uint8_t JumpToApplication, NeedForUpdate, DongleFlag, uartSend, Counter_Received_Pakect_LED_BI;

    u8 getJumptoApp();
    u8 getNeedforUpdate();
    u8 getDongleFlag();
    void clearJumptoApp();
    void setJumptoApp();
    void incrementJumptoApp();
    void clearNeedforUpdate();
    void setNeedforUpdate();
    void clearDongleFlag();
    void setDongleFlag();
#endif  //----------------------   E: Set For Bootloader
