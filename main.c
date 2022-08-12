#include"..\lib\delay_ms.h"
#include"..\lib\lcd.h"
//#include"ds1302.h"
/*
Yeu cau ket noi button
Keypad with Port 2
*/
sbit Row_0 = P2 ^ 7;
sbit Row_1 = P2 ^ 6;
sbit Row_2 = P2 ^ 5;
sbit Row_3 = P2 ^ 4;
sbit Colum_0 = P2 ^ 3;
sbit Colum_1 = P2 ^ 2;
sbit Colum_2 = P2 ^ 1;


sbit buzzer = P3 ^ 2;
sbit door = P1 ^ 7;           
sbit check_opent = P1 ^ 6;     // neu dong lai cua check_opent=0; check_opent=1: dang dong cua, check_opent=0 : mo cua
unsigned int passWord = 1;    // passWord mac dinh
unsigned int passInput = 0;   // pass nguoi dung nhap
unsigned char count_setup_pass = 0; // bien dem dung de lap mk moi
unsigned char inKey;          // bien gia tri nhap tu ban phim
unsigned char count_incorrect_pass = 0; // bien dem so lan sai mat khau
unsigned char second_watting=15;                // bien dem 15s khi nhap sai qua so lan
//TIME *time;

void set_up_MK();
bit check_MK_cu();
void reset(); 
unsigned char inputKeyPad();
void overIncorrect();
//void buzzerFunc();
//void lcd_display_time();

void main()
{
    P2 = 0xFE; // make P2 an input	1111 1110
    door = 0;  
    check_opent = 1;
    buzzer = 0; 
    lcd_init(); 
    lcd_display_string("MK:");
    while (1)
    {
        inKey = inputKeyPad(); 
        if(count_setup_pass==3){
            set_up_MK();
        }
        if (inKey == 'O')
        { 
            // sound_temp();
            reset();
        }
        if (inKey == 'E')
        { 
            if (passInput == passWord)
            {
                // sound_temp();
                door = 1;      // Open door;
                lcd_cmd(0x01); 
                lcd_display_string("Waitting Open");
                delay_ms(10);
                while (door == 1)
                { // khi cua dang mo
                    lcd_cmd(0x01);
                    lcd_display_string("Open Door!!!"); 
                    delay_ms(5);
                    if (check_opent == 0)
                    {
                        reset();
                        break;
                    }
                }
            }
            else
            { // neu khong dung mk
                lcd_cmd(0x01);
                count_incorrect_pass++;
                lcd_display_string("MK sai lan ");
                lcd_display_number(count_incorrect_pass);
                delay_ms(5);
                if(count_incorrect_pass==3){
                    overIncorrect();
                }
            }
            reset();
        }
    }
}

// reset khi nhan nut 'On' va reset mang hinh sau khi nhap mat khau moi ,
void reset()
{
    passInput = 0; 
    door = 0;      
    inKey = 0;     
    lcd_cmd(0x01);
    lcd_display_string("MK:");
    //delay_ms(10);
}

bit check_MK_cu() // 1: dung; 0: sai
{                  
    lcd_cmd(0x01); 
    lcd_display_string("Nhap MK cu:");
    lcd_cmd(0xC0); // bat dau tu line 2 LCD

    while (1)
    {
        inKey = inputKeyPad(); 
        if (inKey == 'E')
        { //  neu ng dung nhap xong nhan E (dau"=")
            if (passInput == passWord)
            { 
                lcd_cmd(0x01);
                lcd_display_string("MK Dung"); 
                delay_ms(5);                   
                return 1;
            }else{
                lcd_cmd(0x01);
                lcd_display_string("MK sai"); 
                delay_ms(5);
                count_incorrect_pass++;
                if(count_incorrect_pass==3){
                    overIncorrect();
                }
                return 0;                 
            }
        }
    }
}

void set_up_MK()
{ 
    if (check_MK_cu() == 1)
    { 
        passInput = 0;
        lcd_cmd(0x01);
        lcd_display_string("Nhap MK moi:"); 
        lcd_cmd(0xC0);                      //	// forcus vao line2 LCD
        while (1)
        {
            inKey = inputKeyPad(); 
            if (inKey == 'E')     
            {
                passWord = passInput; 
                lcd_cmd(0x01);
                lcd_display_string("Da luu MK"); 
                delay_ms(5);
                break; 
            }
        }
    }
    count_setup_pass = 0;
    reset();
}

unsigned char inputKeyPad()
{
    unsigned char key = 13; //13 gia tri tam thoi
    Row_0 = 0;
    if (Colum_0 == 0)
        key = 7; 
    if (Colum_1 == 0)
        key = 8;
    if (Colum_2 == 0)
        key = 9;
    Row_0 = 1;

    Row_1 = 0;
    if (Colum_0 == 0)
        key = 4;
    if (Colum_1 == 0)
        key = 5;
    if (Colum_2 == 0)
        key = 6;
    Row_1 = 1;

    Row_2 = 0;
    if (Colum_0 == 0)
        key = 1;
    if (Colum_1 == 0)
        key = 2;
    if (Colum_2 == 0)
        key = 3;
    Row_2 = 1;

    Row_3 = 0;
    if (Colum_0 == 0)
        key = 'O'; // if button=ON
    if (Colum_1 == 0)
        key = 0;
    if (Colum_2 == 0){
        key = 'E'; // if "=": Enter
        //buzzerFunc();
    }
    Row_3 = 1;

    if (key == 'O')
    {
        count_setup_pass++; 
        //buzzerFunc();
    }
    if (key >= 0 && key < 10)
    {
        //buzzerFunc();           
        lcd_display_string("*"); 
        passInput = passInput * 10 + key; 
                                          
    }
    return key; 
}

void overIncorrect()
{
    lcd_cmd(0x01);
    lcd_display_string("Hay thu lai");
    lcd_begin_at_line(2, 4);
    lcd_display_string("sau   s");
    do
    { // count down 15s
        lcd_begin_at_line(2, 8);
        lcd_display_second(second_watting);
        second_watting--;
    } while (second_watting != 0);
    second_watting = 15;
    count_incorrect_pass = 0;
}

// void buzzerFunc() 
// {
//     // frequence 200Hz
//     // 2500 instruction dao 1 lan
//     buzzer = ~buzzer;
//     delay_ms(2);
// }

// void lcd_display_time()
// {
//     ds1302_GetTime(time);
//     lcd_display_second(time->hour);
//     lcd_data('-');
//     lcd_display_second(time->second);
//     lcd_data('-');
//     lcd_display_second(time->minute);   
// }