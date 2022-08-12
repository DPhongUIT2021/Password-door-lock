<<<<<<< HEAD
#ifndef _max1112_
#define _max1112_
/*
Requirement connect:
    + CS=P2.3; // Chip Select
    + SCLK=P2.4; //Serial CLK
    + DIN=P2.5 //Data Input of Max1112
    + DOUT=P2.6 // Data Output of Max1112
*/
#include<REG51.H>
#include"..\lib\delay_ms.h"

sbit CS = P2 ^ 3;
sbit SCLK = P2 ^ 4;
sbit DIN = P2 ^ 5;
sbit DOUT = P2 ^ 6;
sbit MSB_ACC = ACC ^ 7;
sbit LSB_ACC = ACC ^ 0;

unsigned char MAX1112_Out_digit(unsigned char ControlByte);

unsigned char MAX1112_Out_digit(unsigned char ControlByte)
{
    unsigned char i = 0;
    ACC = ControlByte;
    CS = 0;
    for (; i < 8;i++){
        DIN = MSB_ACC;
        SCLK = 0;
        delay_ms(1);
        SCLK = 1;
        delay_ms(1);
        ACC = ACC << 1;
    }
    CS = 1; // So present analog start convertion to digit
    SCLK = 0; // SCLK=0 during conver
    CS = 0;
    SCLK = 1;
    delay_ms(1);
    SCLK = 0;
    delay_ms(1);
    for (i = 0; i < 8;i++){
        SCLK = 1; // H-T-L
        delay_ms(1);
        SCLK = 0;
        delay_ms(1);
        LSB_ACC = DOUT;
        ACC = ACC << 1;
    }
    CS = 1; // deselect ADC
    return ACC;
}
=======
#ifndef _max1112_
#define _max1112_
/*
Requirement connect:
    + CS=P2.3; // Chip Select
    + SCLK=P2.4; //Serial CLK
    + DIN=P2.5 //Data Input of Max1112
    + DOUT=P2.6 // Data Output of Max1112
*/
#include<REG51.H>
#include"..\lib\delay_ms.h"

sbit CS = P2 ^ 3;
sbit SCLK = P2 ^ 4;
sbit DIN = P2 ^ 5;
sbit DOUT = P2 ^ 6;
sbit MSB_ACC = ACC ^ 7;
sbit LSB_ACC = ACC ^ 0;

unsigned char MAX1112_Out_digit(unsigned char ControlByte);

unsigned char MAX1112_Out_digit(unsigned char ControlByte)
{
    unsigned char i = 0;
    ACC = ControlByte;
    CS = 0;
    for (; i < 8;i++){
        DIN = MSB_ACC;
        SCLK = 0;
        delay_ms(1);
        SCLK = 1;
        delay_ms(1);
        ACC = ACC << 1;
    }
    CS = 1; // So present analog start convertion to digit
    SCLK = 0; // SCLK=0 during conver
    CS = 0;
    SCLK = 1;
    delay_ms(1);
    SCLK = 0;
    delay_ms(1);
    for (i = 0; i < 8;i++){
        SCLK = 1; // H-T-L
        delay_ms(1);
        SCLK = 0;
        delay_ms(1);
        LSB_ACC = DOUT;
        ACC = ACC << 1;
    }
    CS = 1; // deselect ADC
    return ACC;
}
>>>>>>> 7a2f3128544c57fe0f81796aee1e996fe16aea10
#endif