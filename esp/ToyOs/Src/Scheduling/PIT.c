#include<PIT.h>
/*初始divisor*/
uint16_t Divisor=65535;
/*从开机以来的时间*/
double TimeSinceBoot=0;
/*晶振的基本频率*/
const uint64_t BaseFrequency = 1193182;

void Sleepd(double seconds){
    double startTime = TimeSinceBoot;
    while (TimeSinceBoot < startTime + seconds){
        asm("hlt");
    }
}
void Sleep(uint64_t milliseconds){
    Sleepd((double)milliseconds / 1000);
}

/*设置分频的计数*/
void SetDivisor(uint16_t divisor){
    if (divisor < 100) divisor = 100;
    Divisor = divisor;
    outb(0x40, (uint8_t)(divisor & 0x00ff));// Low byte
    IO_Wait();
    outb(0x40, (uint8_t)((divisor & 0xff00) >> 8));// High byte
}
/*得到现有频率*/
uint64_t GetFrequency(){
    return BaseFrequency/Divisor;
}
/*设置频率*/
void SetFrequency(uint64_t frequency){
    SetDivisor(BaseFrequency/frequency);
}
void Tick(){
    TimeSinceBoot+=1/((double)GetFrequency());
}