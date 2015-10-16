#include "DS18B20.h"

unsigned char DeviceAddress[DevicesNumber][8];

unsigned char init_sensor(void)
{
    unsigned char reponse=1;
    Direction_Data=0; //output
    Write_Data=0;
    __delay_us(480);
    Write_Data=1;
    __delay_us(64);
    Direction_Data=1;  //input
    reponse= Read_Data;
    __delay_us(416);
    Direction_Data=0;
    Write_Data=1;
    return reponse;
}

void write_Bit(unsigned char WriteArg)
{
    if (WriteArg==1)
    {
        Direction_Data=0; //output
        Write_Data=0;
        __delay_us(6);
        Direction_Data=0; //output
        Write_Data=1;
        __delay_us(64);
    }
    else
    {
        Direction_Data=0; //output
        Write_Data=0;
        __delay_us(60);
        Direction_Data=0; //output
        Write_Data=1;
        __delay_us(10);
    }
}

void write_Byte(unsigned char data)
{
    for(int i=0;i<8;i++)
    {
        write_Bit(data & 0x01);
        data>>=1;
    }
}

unsigned char read_Bit(void) 
{
    unsigned char result = 0x00;

    Direction_Data=0; //output
    Write_Data=0;
    __delay_us(6);
    Direction_Data=0; //output
    Write_Data=1;
    __delay_us(9);
    Direction_Data=1;  //input
    result= Read_Data;
    __delay_us(55);
    return result;
}

unsigned char read_Byte(void)
{
    unsigned char data=0;
    for (int i=0;i<8;i++)
    {
        data>>=1;
        if(read_Bit())
        {
        data|=0x80;
        }
    }
    return data;
}

unsigned char address_Device(unsigned char Adress[])
{
   if( init_sensor()==0)
   {
       write_Byte(MatchROM);
       for(int i=0;i<8;i++)
       {
           write_Byte(Adress[i]);
       }
       return 1; //sucessful
   }else 
   {
       return 0;
   }
}

unsigned char search_Devices(void)
{
    unsigned char list_Devices[8];
    unsigned char LastConflictBitNum=0;
    for(int i=0;i<DevicesNumber;i++){
         unsigned char mask=0x01;
         for(int k=0;k<8;k++)
         {
         list_Devices[k]=0x00;
         }
         if (init_sensor())
         {
             return 0;
         }
         write_Byte(SearchROM);
         int bitnum=1;
         unsigned char CurrentConflictBitPos=0;
         while(bitnum<65)
         {
             unsigned char SelectedBit=0;
             unsigned char InputBitStatus=0x00;
             if (read_Bit()==1)
             {
                 InputBitStatus|=2;
             }
             if(read_Bit()==1)
             {
                 InputBitStatus|=1;
             }
        /* description for values of InputBitStatus: */
        /* 00    There are devices connected to the bus which have conflicting */
        /*       bits in the current ROM code bit position. */
        /* 01    All devices connected to the bus have a 0 in this bit position. */
        /* 10    All devices connected to the bus have a 1 in this bit position. */
        /* 11    There are no devices connected to the 1-wire bus. */
             if (InputBitStatus==3)
             {
                 return 0;
             }
             if(InputBitStatus>0)
             {
                 SelectedBit=InputBitStatus>>1;
             }
             else
             {
                 if(bitnum==LastConflictBitNum)
                 {
                     SelectedBit=1;
                 }
                 else
                 {
                     SelectedBit=0;
                     CurrentConflictBitPos=bitnum;
                 }
             }
             write_Bit(SelectedBit);
             mask=0x01;
             mask<<=(bitnum-1)%8;
             if (SelectedBit==1)
             {
             list_Devices[(bitnum-1)/8]|=mask;
             }
        bitnum++;
        }
         LastConflictBitNum=CurrentConflictBitPos;
         for(int j=0;j<8;j++){
         DeviceAddress[i][j]=list_Devices[j];
         }
    }
    return 1;
}

float read_temp(unsigned char DeviceAddress)
{
    unsigned char templ,temph;
    int temp;
    float temperature;
    
    address_Device(DeviceAddress);
    write_Byte(ConvertT);
    delay750ms();
    init_sensor();
    address_Device(DeviceAddress);
    write_Byte(ReadScratchpad);
    templ= read_Byte();
    temph=read_Byte();
    temp=temph;
    temp<<=8;
    temp|=templ;
    temperature=temp*0.0625;
    return temperature;
}

void delay100ms(void) 
{
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
}

void delay50ms(void) 
{
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);

}
void delay750ms(void) 
{
    delay100ms();
    delay100ms();
    delay100ms();
    delay100ms();
    delay100ms();
    delay100ms();
    delay100ms();
    delay50ms();
}

