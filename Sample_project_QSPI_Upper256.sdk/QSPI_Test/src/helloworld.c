#include <stdio.h>
#include "zynq7020.h"
#include "xil_printf.h"
uint8_t Read[256], Write[256];

int main()
{
    xil_printf("Hello World\n\r");
    Flash_Init();
    uint32_t sector=510;
	for(int i=0; i<255; i++)
		Write[i]=254-i;

    EEPROM1_Flash0_Read (0,sector,Read ,MODE_8_BIT,250,0);
    Flash_Erase(sector);
	EEPROM1_Flash0_Write(0,sector,Write,MODE_8_BIT,250  );
    EEPROM1_Flash0_Read (0,sector,Read ,MODE_8_BIT,250,0);
    while(1){
    	sleep(1);
    	xil_printf("Man to while hastam.\r\n");
    }
    return 0;
}
