#include<Mouse.h>

uint8_t MousePointer[] = {
    0b11111111, 0b11100000, 
    0b11111111, 0b10000000, 
    0b11111110, 0b00000000, 
    0b11111100, 0b00000000, 
    0b11111000, 0b00000000, 
    0b11110000, 0b00000000, 
    0b11100000, 0b00000000, 
    0b11000000, 0b00000000, 
    0b11000000, 0b00000000, 
    0b10000000, 0b00000000, 
    0b10000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
};


void MouseWaitUntilOutput(){
    /*
        这个函数的目的 bytes cannot be read from port 0x60 until bit 0 (value=1) of port 0x64 is set.
    */
    uint64_t time_out=100000;
    while(time_out--){
        if((inb(0x64)&2)==0)
            return;
    }
}

void MouseWaitUntilInput(){
    /*
        bytes cannot be read from port 0x60 until bit 0 (value=1) of port 0x64 is set.
    */
   uint64_t time_out=100000;
    while(time_out--){
        if((inb(0x64)&1)==1)
            return;
    }
}

void InitPS2Mouse(){
    
    /*
        https://wiki.osdev.org/Mouse_Input#Initializing_a_PS2_Mouse
    */
    outb(0x64,0xA8);//Aux Input Enable Command 启动PS2的辅助输入功能

    MouseWaitUntilOutput();///*这个函数等待到可以向0x64端口输出数据*/

    /*
        On some systems, the PS2 aux port is disabled at boot. 
        Data coming from the aux port will not generate any interrupts. 
        To know that data has arrived, you need to enable the aux port to generate IRQ12. 
        There is only one way to do that, which involves getting/modifying the "compaq status" byte. 
        You need to send the command byte 0x20 ("Get Compaq Status Byte") to the PS2 controller on port 0x64. 
        The very next byte returned should be the Status byte.
    */
    /*
        60h 	data port from the keyboard controller
        64h 	command port for keyboard controller - use to enable/disable kbd interrupts, etc. 
    */
    outb(0x64,0x20);

    /*
        bytes cannot be read from port 0x60 until bit 0 (value=1) of port 0x64 is set.
    */
    MouseWaitUntilInput();
    /*
        The very next byte returned should be the Status byte.
    */
    uint8_t Status=inb(0x60);
    /*
        After you get the Status byte, you need to set bit number 1 (value=2, Enable IRQ12), 
        and clear bit number 5 (value=0x20, Disable Mouse Clock). 
        Then send command byte 0x60 ("Set Compaq Status") to port 0x64, 
        followed by the modified Status byte to port 0x60. This might generate a 0xFA ACK byte from the keyboard. 
    */
    Status|=0b00000010;
    //Status&=0b11101111;
    MouseWaitUntilOutput();
    outb(0x64,0x60);
    MouseWaitUntilOutput();
    outb(0x60,Status);

    MouseWrite(0xF6);//Set Defaults Disables streaming, sets the packet rate to 100 per second, and resolution to 4 pixels per mm. 

    MouseRead();// Get ACK

    // Enable Mouse
    MouseWrite(0xF4);//Enable Packet Streaming 	The mouse starts sending automatic packets when the mouse moves or is clicked. 
    MouseRead();// Get ACK
}

void MouseWrite(uint8_t value){
    /*
        value : 命令字
    */
    /*
        Sending a command or data byte to the mouse (to port 0x60) must be preceded by sending a 0xD4 byte to port 0x64 (with appropriate waits on port 0x64, bit 1, 
        before sending each output byte).
    */
    MouseWaitUntilOutput();
    outb(0x64,0xD4);
    MouseWaitUntilOutput();
    outb(0x60,value);

    
}

uint8_t MouseRead(){
    MouseWaitUntilInput();
    return inb(0x60);
}


/*四个byte为一个完整的包*/
uint8_t MouseCycle = 0;//同步控制
uint8_t Packet[4]={};//一个完整的包
bool PacketReady=false;
void HandlePS2Mouse(uint8_t byte){
    /*
        https://wiki.osdev.org/Mouse_Input#Mouse_Packet_Info
    */
    switch (MouseCycle)
    {
    case 0:
        if(PacketReady) break;
        if(byte&0b00001000==0)break;//包被破坏
        /* code */
        Packet[0]=byte;
        MouseCycle++;
        break;
    case 1:
        if(PacketReady) break;
        Packet[1]=byte;
        MouseCycle++;
        break;
    case 2:
        if(PacketReady) break;
        Packet[2]=byte;
        MouseCycle=0;
        PacketReady=true;
        break;

    default:
        break;
    }
}

POINT MousePosition;
/*用来清楚之前的*/
POINT MousePositionOld;
void ProcessMousePacked(){
    /*包没有准备好就返回*/
    if(!PacketReady)return;

    bool xSigned,ySigned,xOverflow,yOverflow;

    xSigned = Packet[0] & PS2_X_SIGN;
    ySigned = Packet[0] & PS2_Y_SIGN;
    xOverflow = Packet[0] & PS2_X_OVERFLOW;
    yOverflow = Packet[0] & PS2_Y_OVERFLOW;

    //printf("x=%dy=%d\n",xSigned,ySigned);
    if(xSigned!=0){
        //向左是负
        Packet[1] = 256-Packet[1];
        MousePosition.X-=Packet[1];
        //if(xOverflow) MousePosition.X-=255;
    }
    else{
        MousePosition.X+=Packet[1];
        //if(xOverflow) MousePosition.X+=255;
    }

    if(ySigned!=0){
        //向上是负
        Packet[2] = 256-Packet[2];
        MousePosition.Y+=Packet[2];
        //if(yOverflow) MousePosition.Y+=255;

    }
    else{
         MousePosition.Y-=Packet[2];
        //if(yOverflow) MousePosition.Y-=255;

    }
    extern VIDEO_CONFIG *VideoConfig;
    if(MousePosition.X<0)MousePosition.X=0;
    if(MousePosition.X>VideoConfig->HorizontalResolution)MousePosition.X=VideoConfig->HorizontalResolution-1;

    if(MousePosition.Y<0)MousePosition.Y=0;
    if(MousePosition.Y>VideoConfig->VerticalResolution)MousePosition.Y=VideoConfig->VerticalResolution-1;

    ClearMouseCursor(MousePointer,MousePositionOld);
    DrawOverlayMouseCursor(MousePointer,MousePosition,Red);

    // if (MousePacket[0] & PS2Leftbutton){
    //         GlobalRenderer->PutChar('a', MousePosition.X, MousePosition.Y);
    //     }
    //     if (MousePacket[0] & PS2Middlebutton){
            
    //     }
    //     if (MousePacket[0] & PS2Rightbutton){
    //         uint32_t colour = GlobalRenderer->Colour;
    //         GlobalRenderer->Colour = 0x0000ff00;
    //         GlobalRenderer->PutChar('a', MousePosition.X, MousePosition.Y);
    //         GlobalRenderer->Colour = colour;
    //     }
    if((Packet[0]&PS2_LEFT_BTN)!=0){
        _putchar(VideoConfig->FrameBufferBase,'b',MousePosition.X/17,MousePosition.Y/32,White,Black);
    }
    if((Packet[0]&PS2_MID_BTN)!=0){

    }
    if((Packet[0]&PS2_RIGHT_BTN)!=0){
        _putchar(VideoConfig->FrameBufferBase,'b',MousePosition.X/17,MousePosition.Y/32,White,Black);
    }

    PacketReady=false;
    MousePositionOld=MousePosition;
} 


