
#include <stdio.h>
#include <stdlib.h>
#include "config/config_pic18f4550_20mhz.h"
#include "DS18B20/DS18B20.h"
#include "NRF_24L01/nRF2401.h"

union
{
    float temperature;
    unsigned char value[sizeof(float)];
} temp_union;


void fill_SendBuff(float temperature, unsigned char* buffer);
/*
 * 
 */
int main(int argc, char** argv) {
    
    //debug
    TRISAbits.TRISA1 = 0;
    //Variables declaration
    unsigned char tx_buf[MAX_PAYLOAD];
    unsigned char rx_buf[MAX_PAYLOAD];
    unsigned char ds18b20_response = 1;
    unsigned char nrf24l01_response = 1;
    float temperature;
    
    //general MCU configurations
    
    OSCCONbits.IRCF = 0xF; //8 Mhz ext OSC
    ADCON1 |= 0xF; //ANSEL digital pins
    ei();     // This is like fliping the master switch to enable interrupt
    
    //NRF configurations
    
    TRIS_CE = 0;
    TRIS_CSN = 0;
    TRIS_IRQ = 1;
    TRIS_SCK = 0;
    TRIS_MISO = 1;
    TRIS_MOSI = 0;
    
    RA1=1; 
    nrf_init();
    delay50ms();
    nrf_txmode(); // set the NRF module to send temperature OTA
    
    //DS18B20 configurations
    ds18b20_response=init_sensor();
    
    
    if(ds18b20_response==0)
    {
        unsigned char Sucess;
        Sucess=search_Devices();
        do
        {
         
         temp_union.temperature=read_temp(DeviceAddress[0]);
         fill_SendBuff(temp_union.temperature,&tx_buf);
         nrf24l01_response = nrf_send(&tx_buf,&rx_buf);
         if(nrf24l01_response){RA1 = 1;}else{RA1 = 0;}
         delay750ms();
        }while(1);
    };

    return (EXIT_SUCCESS);
}

void fill_SendBuff(float temperature, unsigned char* buffer){
    unsigned char i;
    
    for(i = 0; i< sizeof(float); i++){
        buffer[i] = temp_union.value[i];
    }
    for( i = sizeof(float) ; i < MAX_PAYLOAD ; i++){
        buffer[i] = 0;
    }
    
}