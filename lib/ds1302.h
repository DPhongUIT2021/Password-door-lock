/*
Yeu cau ket noi
P1->3=RST (CE)
P1->4=CLK
P1->5=IO
*/
//#pragma once
#ifndef _ds1302_h
#define _ds1302_h
#include <REG51.H>

#define ds1302_second_write 0x80
#define ds1302_second_read 0x81
#define ds1302_minute_write 0x82
#define ds1302_minute_read 0x83
#define ds1302_hour_wirte 0x84
#define ds1302_hour_read 0x85

sbit ds1302_RST = P1 ^ 3;
sbit ds1302_CLK = P1 ^ 4;
sbit ds1302_IO = P1 ^ 5;

sbit ACC_0 = ACC ^ 0; // sfr ACC=0xE0;
sbit ACC_7 = ACC ^ 7;

typedef struct TIME{
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char arrayTime[8];
}TIME;

void ds1302_write_byte(unsigned char);
unsigned char ds1302_read_byte();
void ds1302_write(unsigned char, unsigned char);
unsigned char ds1302_read(unsigned char);
void ds1302_init();
void ds1302_getTime(TIME *);
unsigned char bcd_to_decimal(unsigned char);
void ds1302_getTime_char(TIME *);

void ds1302_write_byte(unsigned char cmd)
{
    unsigned char i;
    ACC = cmd;
    for (i = 0; i < 8;i++)
    {
        ds1302_IO = ACC_0;
        delay_ms(1);
        ds1302_CLK = 1;
        delay_ms(1);
        ds1302_CLK = 0;
        ACC = ACC >> 1;
    }
}

unsigned char ds1302_read_byte()
{
    unsigned char i;
    for (i = 0; i < 8;i++)
    {
        ACC = ACC >> 1;
        ACC_7 = ds1302_IO; // read: D0 fist
        delay_ms(1);
        ds1302_CLK = 1;
        delay_ms(1);
        ds1302_CLK = 0;
    }
    return ACC;
}

void ds1302_write(unsigned char cmd, unsigned char dat)
{
    ds1302_RST = 1;
    ds1302_CLK = 0;
    ds1302_write_byte(cmd);
    ds1302_write_byte(dat);
    ds1302_CLK = 1;
    ds1302_RST = 0;
}

unsigned char ds1302_read(unsigned char cmd)
{
    ds1302_RST = 1;
    ds1302_CLK = 0;
    ds1302_write_byte(cmd);
    cmd = ds1302_read_byte(); // cmd(valueal temp): value read from ds1302
    ds1302_CLK = 1;
    ds1302_RST = 0;
    return cmd;
}

void ds1302_init(){
    ds1302_RST = 0;
    ds1302_CLK = 0;
    ds1302_write(0x8E, 0);    // write protect bit=0(Bit 7) and Bit6-Bit0=0
    ds1302_write(0x90, 0xA6); // trickle charger Resitor and Diode Select; TCS=1010-DS=01(1 dioed)-RS=10(4kOhm)
    if ((ds1302_read(ds1302_second_read) & 0x80) != 0)
    { // setup second=0
        ds1302_write(0x80, 0);
    }
    delay_ms(1);
}

unsigned char bcd_to_decimal(unsigned char bcd)
{
    return ((bcd & 0x70) >> 4) * 10 + (bcd & 0x0F);
}

void ds1302_getTime(TIME*time)
{
    time->second = bcd_to_decimal(ds1302_read(ds1302_second_read));
    time->minute = bcd_to_decimal(ds1302_read(ds1302_minute_read));
    time->hour = bcd_to_decimal(ds1302_read(ds1302_hour_read));
}

void ds1302_getTime_char(TIME*time){
    ds1302_getTime(time);
    time->arrayTime[0] = time->hour / 10 + '0';
    time->arrayTime[1] = time->hour % 10 + '0';
    time->arrayTime[2] = ':';
    time->arrayTime[3] = time->minute / 10 + '0';
    time->arrayTime[4] = time->minute % 10 + '0';
    time->arrayTime[5] = ':';
    time->arrayTime[6] = time->second / 10 + '0';
    time->arrayTime[7] = time->second % 10 + '0';
}

#endif