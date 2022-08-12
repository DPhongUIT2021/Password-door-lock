<<<<<<< HEAD
#ifndef _delay_ms_
#define _delay_ms_
#include<REG51.H>
void delay_ms(unsigned int ms);
void timer0_ISR(); // timer0 interrupt servive rountine
void delay_us(unsigned int us);

void delay_ms(unsigned int ms)
{
    unsigned int i;
    TMOD = 0x01; // Timer 0 mode 1 delay 1ms
    TH0 = 0xFC; //FC18->FFFF=1000 intruction*1us=1ms (Crytal=12MHz)
    TL0 = 0x18;
    for (i = 0; i < ms; i++)
    {
        do
        {
            TR0 = 1;
        } while (TF0 == 0);
        TR0 = 0;
        TF0 = 0;
    }
}

void delay_us(unsigned int us){
    TMOD = TMOD & 0xF0; // giu lai 4 bit dau
    TMOD = 0x02; // Timer 0 mode 2 auto reload 8 bit
    TL0 = 0xFE;
    TH0 = 0x00; // 2 intruction = 2us
    EA = 1;     // enable all interrupt
    ET0 = 1; // enable interrupt timer 0
    do{
        TR0 = 1; // start timer 0 ==> call vector location interrup 1
        us--;
    } while (us >= 0);
}

void timer0_ISR() interrupt 1{
    TF0 = 0; // clear flag timer 0
}

=======
#ifndef _delay_ms_
#define _delay_ms_
#include<REG51.H>
void delay_ms(unsigned int ms);
void timer0_ISR(); // timer0 interrupt servive rountine
void delay_us(unsigned int us);

void delay_ms(unsigned int ms)
{
    unsigned int i;
    TMOD = 0x01; // Timer 0 mode 1 delay 1ms
    TH0 = 0xFC; //FC18->FFFF=1000 intruction*1us=1ms (Crytal=12MHz)
    TL0 = 0x18;
    for (i = 0; i < ms; i++)
    {
        do
        {
            TR0 = 1;
        } while (TF0 == 0);
        TR0 = 0;
        TF0 = 0;
    }
}

void delay_us(unsigned int us){
    TMOD = TMOD & 0xF0; // giu lai 4 bit dau
    TMOD = 0x02; // Timer 0 mode 2 auto reload 8 bit
    TL0 = 0xFE;
    TH0 = 0x00; // 2 intruction = 2us
    EA = 1;     // enable all interrupt
    ET0 = 1; // enable interrupt timer 0
    do{
        TR0 = 1; // start timer 0 ==> call vector location interrup 1
        us--;
    } while (us >= 0);
}

void timer0_ISR() interrupt 1{
    TF0 = 0; // clear flag timer 0
}

>>>>>>> 7a2f3128544c57fe0f81796aee1e996fe16aea10
#endif