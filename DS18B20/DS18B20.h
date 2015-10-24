#ifndef DS18B20_H
#define	DS18B20_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <p18f2550.h>

#define _XTAL_FREQ 8000000     
#define USE_AND_MASKS
    

#define Write_Data LATA0
#define Read_Data RA0
#define Direction_Data TRISA0
#define ReadROM 0x33
#define MatchROM 0x55
#define SkipROM 0xCC
#define SearchROM 0xF0
#define AlarmSearch 0xEC
#define ConvertT 0x44
#define ReadScratchpad 0xBE
#define WriteScratchpad 0x4E
#define CopyScratchpad 0x48
#define RecallEE 0xB8
#define ReadPowerSupply 0xB4
#define DevicesNumber 2

extern unsigned char DeviceAddress[DevicesNumber][8];
    
    
 /*Prototypes des fonctions*/
unsigned char init_sensor(void);
void write_Bit(unsigned char WriteArg);
void write_Byte(unsigned char data);
unsigned char read_Bit(void);
unsigned char read_Byte(void);
unsigned char address_Device(unsigned char Adress[]);
unsigned char search_Devices(void);
float read_temp(unsigned char DeviceAdress);
void delay100ms(void);
void delay50ms(void);
void delay750ms(void);



#ifdef	__cplusplus
}
#endif

#endif	/* DS18B20_H */

