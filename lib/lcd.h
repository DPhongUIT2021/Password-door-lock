#ifndef _lcd_
#define _lcd_

#include <REG51.H>
#include <string.h>
#include"delay_ms.h"
#include"ds1302.h"
/* yeu cau ket noi LCD
sfr lcdData = P0;
sbit RS = P1 ^ 7;
sbit RW = P1 ^ 6;
sbit E = P1 ^ 5;
sbit flag_busy = P0 ^ 7;
lcdData with D0..D7 of LCD
*/
sfr lcdData = 0x80; // port 0
sbit RS = P1 ^ 0;
sbit RW = P1 ^ 1;
sbit E = P1 ^ 2;

// define function

void lcd_cmd(unsigned char); // ham gui lenh (instruction input ) ra LCD
void lcd_data(unsigned char); // ham gui du lieu ( data ) ra LCD
void lcd_setup();      //  setup LCD
void lcd_begin_at_line(unsigned char line, unsigned char start);
void lcd_display_string(unsigned char *str); // ham in chuoi kí tu ra LCD
void lcd_display_number(unsigned char number); // if(number=09) display 9
void lcd_display_second(unsigned char second); // 00->99; if second=1s --> 01

//  implemente function
void lcd_cmd(unsigned char value)
{
    lcdData = value; 
    RS = 0;     
    RW = 0;
    E = 1;
    delay_ms(1);
    E = 0;
}

void lcd_data(unsigned char value)
{
    lcdData = value;
    RS = 1;
    RW = 0;
    E = 1;
    delay_ms(1);
    E = 0;
}

void lcd_init()
{
    lcd_cmd(0x38); // init LCD 2 line 5x7 matrix dot
    lcd_cmd(0x0C); //display on, cursor off
    lcd_cmd(0x01); // clear LCD
    lcd_cmd(0x06); // shift cursor right, increase
}

void lcd_begin_at_line(unsigned char line, unsigned char start)
{
    start = start & 0x0F;
    if (line == 1){
        lcd_cmd((lcdData = 0x80) | start);
    }
    if (line == 2){
        lcd_cmd((lcdData = 0xC0) | start);
    }
}

void lcd_display_string(unsigned char *str)
{
    unsigned char i;
    delay_ms(5);
    for (i=0 ; i < strlen(str); i++)
    {
        lcd_data(str[i]);
    }
    delay_ms(10);
}

void lcd_display_number(unsigned char number){
    unsigned char a2, a1, a0;
    a0 = number % 10;
    number /= 10;
    a1 = number % 10;
    number /= 10;
    a2 = number % 10;
    if (a2 != 0){
        lcd_data(a2 | 0x30);
    }
    if (a1 != 0){
        lcd_data(a1 | 0x30);
    }  
    lcd_data(a0 | 0x30);
    delay_ms(10);
}

void lcd_display_second(unsigned char second)
{
    unsigned char a1, a0;
    a0 = second % 10;
    second /= 10;
    a1 = second % 10;
    lcd_data(a1 | 0x30);
    lcd_data(a0 | 0x30);
    delay_ms(10);
}

#endif