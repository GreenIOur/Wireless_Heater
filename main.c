
#include <stdio.h>
#include <stdlib.h>
#include "config/config_pic18f4550_20mhz.h"
#include "DS18B20/DS18B20.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    //general MCU configurations
    
    OSCCONbits.SCS = 0; //20 Mhz ext OSC
    ADCON1bits.PCFG = 0xF; //ANSEL digital pins
    ei();     // This is like fliping the master switch to enable interrupt
    
    unsigned char reponse=1;
    float temperature;
    
    reponse=init_sensor();
    if(reponse==0)
    {
        unsigned char Sucess;
        Sucess=search_Devices();
        do
        {
         temperature=read_temp(DeviceAddress[0]);
        }while(1);
    };

    return (EXIT_SUCCESS);
}

