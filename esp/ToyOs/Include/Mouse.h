#pragma once
#include<IO.h>

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