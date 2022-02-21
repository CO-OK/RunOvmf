#pragma once
#include<IO.h>
#include<Bool.h>
#include<Color.h>
#include<BootConfig.h>
//UINT32 *VideoStart;

/*方便进行位操作*/
#define PS2_X_SIGN 0b00010000
#define PS2_Y_SIGN 0b00100000
#define PS2_X_OVERFLOW 0b01000000
#define PS2_Y_OVERFLOW 0b10000000
#define PS2_MID_BTN 0b00000100
#define PS2_RIGHT_BTN 0b00000010
#define PS2_LEFT_BTN 0b00000001

typedef struct{
    uint32_t X;
    uint32_t Y;
}MousePoint;

/*这个函数等待到可以向0x64端口输出数据*/
void MouseWaitUntilOutput();

/*这个函数等待到可以从0x60端口接受数据*/
void MouseWaitUntilInput();

/*向鼠标发送命令*/
void MouseWrite(uint8_t value);

/*读取鼠标发来的命令*/
uint8_t MouseRead();

/*初始化鼠标*/
void InitPS2Mouse();

/*处理鼠标发送过来的数据*/
void HandlePS2Mouse(uint8_t byte);
/*处理鼠标发送过来的数据*/
void ProcessMousePacked();