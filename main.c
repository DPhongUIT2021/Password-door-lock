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
sbit door = P1 ^ 7;           // mo cua=1;	, d�ng cua = 0
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
void reset(); // cancle neu muon xoa nhap lai
unsigned char inputKeyPad();
void overIncorrect();
//void buzzerFunc();
//void lcd_display_time();

void main()
{
    P2 = 0xFE; // make P2 an input	1111 1110
    door = 0;  // cua dong == den tat,
    check_opent = 1;
    buzzer = 0; // make P2.0 an output
    lcd_init(); // clear display
    lcd_display_string("MK:");
    while (1)
    {
        inKey = inputKeyPad(); // nhap input tu ban  phim
        if(count_setup_pass==3){
            set_up_MK();
        }
        if (inKey == 'O')
        { // 0: Xoa nhap lai
            // sound_temp();
            reset();
        }
        if (inKey == 'E')
        { // neu nhan = E(enter)
            if (passInput == passWord)
            {
                // sound_temp();
                door = 1;      // Open door;
                lcd_cmd(0x01); // clear display
                lcd_display_string("Waitting Open");
                delay_ms(10);
                while (door == 1)
                { // khi cua dang mo
                    lcd_cmd(0x01);
                    lcd_display_string("Open Door!!!"); // canh bao cua dong mo
                    delay_ms(5);
                    if (check_opent == 0)
                    { // khi nhan nut button reset thi n� se reset
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
    passInput = 0; // luc nay gan =0 (  mac  dinh)
    door = 0;      // luc nay ladong cua
    inKey = 0;     // user nhap tu  ban ph�m
    lcd_cmd(0x01); // Clear display
    lcd_display_string("MK:");
    //delay_ms(10);
}

bit check_MK_cu() // 1: dung; 0: sai
{                  
    lcd_cmd(0x01); // clear display
    lcd_display_string("Nhap MK cu:");
    lcd_cmd(0xC0); // bat dau tu line 2 LCD
                   // ham nhap mkcu
    while (1)
    {
        inKey = inputKeyPad(); // user nhap vao
        if (inKey == 'E')
        { //  neu ng dung nhap xong nhan E (dau"=")
            if (passInput == passWord)
            { // neu input tr�ng kh�p  v�i passthi
                lcd_cmd(0x01);
                lcd_display_string("MK Dung"); // xuat ra mk dung
                delay_ms(5);                    //  delay
                return 1;
            }else{
                lcd_cmd(0x01);
                lcd_display_string("MK sai"); // nguoc lai thi xuat ra mk sai
                delay_ms(5);
                count_incorrect_pass++;
                if(count_incorrect_pass==3){
                    overIncorrect();
                }
                return 0;                     // return  0
            }
        }
    }
}

void set_up_MK()
{ // neu user nhap v�o nut On 3 lan( counter d�m l�n 3 )
    if (check_MK_cu() == 1)
    { // neu nhu check mk dung
        passInput = 0;
        lcd_cmd(0x01);
        lcd_display_string("Nhap MK moi:"); //  cho phep nhap  mk  moi
        lcd_cmd(0xC0);                      //	// forcus vao line2 LCD
        while (1)
        {
            inKey = inputKeyPad(); // bat dau nhap mk moi
            if (inKey == 'E')      // nhap xong neu nh�n E(d�u "=" )
            {
                passWord = passInput; // neu input nhap trung voi pass
                lcd_cmd(0x01);
                lcd_display_string("Da luu MK"); // xuat ra dung
                delay_ms(5);
                break; // luc nay neu ktra dung r break ra khoi vong lapp luon
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
        key = 7; // tuc la khi  row_0=0 giao voi colum_0=0 tai � so 7,l�c n�y  � s� 7 = 0 tuc la nhan 7
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
        count_setup_pass++; // dem de dung cho viec setup mk moi.
        //buzzerFunc();
    }
    if (key >= 0 && key < 10)
    {
        //buzzerFunc();            // n�u key tu 0 den 10 th�
        lcd_display_string("*"); // delay
        passInput = passInput * 10 + key; // nay de tinh so hang chuc,vd nhap so 1, xong tiep theo nhap so 2 thi ra 12 cho nen
                                          // o d�y ta cho 1*10 + 2 = 12 ..
    }
    return key; // tra v� key
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