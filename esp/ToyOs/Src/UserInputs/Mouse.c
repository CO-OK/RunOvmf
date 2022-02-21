#include<Mouse.h>

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