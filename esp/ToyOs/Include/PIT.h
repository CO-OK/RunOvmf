#pragma once
#include<Uefi.h>
#include<IO.h>

/*https://wiki.osdev.org/Programmable_Interval_Timer*/

/*端口*/
#define CHANNEL_0_DATA 0x40//Channel 0 data port (read/write)       
#define CHANNEL_1_DATA 0x41//Channel 1 data port (read/write)
#define CHANNEL_2_DATA 0x42//Channel 2 data port (read/write)
#define COMMAND 0x43//Mode/Command register (write only, a read is ignored)
/*从开机以来的时间*/
extern double TimeSinceBoot;


void Sleepd(double seconds);
void Sleep(uint64_t milliseconds);

/*设置分频的计数*/
void SetDivisor(uint16_t divisor);
/*得到现有频率*/
uint64_t GetFrequency();
/*设置频率*/
void SetFrequency(uint64_t frequency);
void Tick();